#include "Database.h"
#include "MultiMap.h"
#include "Tokenizer.h"
#include "http.h"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <iostream>
using namespace std;

Database::Database()
:m_Row_cnt(0)
{}

Database::~Database()
{
	m_fieldIndex.clear();
	m_schema.clear();
	m_rows.clear();
}

bool Database::specifySchema(const std::vector<FieldDescriptor>& schema)
{
	bool index_exists = false;
	//first reset database
	m_fieldIndex.clear();
	m_schema.clear();
	m_rows.clear();

	//for each FieldDescriptor in the schema
	//check to see if it is indexed 
	//if it is then set index_exists to true and push a new MultiMap to m_fieldIndex
	for (int i = 0; i < schema.size(); i++)
	{
		//push in a new MultiMap for each schema field
		m_fieldIndex.push_back(new MultiMap);
		if (schema[i].index == it_indexed)
		{
			index_exists = true;
		}
	}

	//if an index exists, then set m_schema to schema, otherwise it is blank
	if (index_exists)
		m_schema = schema;

	//set member variable m_schemaSpecified to index_exits
	m_schemaSpecified = index_exists;

	return index_exists;
}

bool Database::addRow(const std::vector<std::string>& rowOfData)
{
	//if a schema isn't specified then do no add row and return false
	//if row size isn't equal to schema size, return false
	if (!m_schemaSpecified || rowOfData.size() != m_schema.size())
		return false;
	//add row to vector of rows
	m_rows.push_back(rowOfData);

	//for each field that is indexed, must be added to the appropriate m_fieldIndex
	for (int i = 0; i < rowOfData.size(); i++)
	{
		//if the field descriptor at index i of m_schema is indexed
		if (m_schema[i].index == it_indexed)
			//insert the data at index i of rowOfData to the ith MultiMap of m_fieldIndex
			//insert into an indexed field of the MultiMap
			m_fieldIndex[i]->insert(rowOfData[i], m_Row_cnt);
	}
	m_Row_cnt++;
	return true;
}

bool Database::loadFromURL(std::string url)
{
	string page; //stores the pages content
	vector<FieldDescriptor> newSchema;
	vector<std::string> newRow;
	int i = 0;
	if (HTTP().get(url, page))//get the content from the url and store it in page
	{
		//first breaks up each line
		std::string delimiters = "\n";
		std::string tokenizeMe = page;

		Tokenizer t(tokenizeMe, delimiters);
		string w;

		//now we tokenize each line
		while (t.getNextToken(w))//breaks up lines
		{
			//single line of a file that will be tokenized
			tokenizeMe = w;
			Tokenizer t(tokenizeMe, delimiters);
			string w;
			//first get the schema
			while (t.getNextToken(w))
			{
				//the first row is the schema settings
				if (i == 0)
				{
					FieldDescriptor s;
					//check if string contains a '*'
					//if so then that means it is indexed
					if (w.find('*') != std::string::npos)
					{
						//get rid of the * at the end
						s.name = w.substr(0, w.size() - 1);
						s.index = it_indexed;
						newSchema.push_back(s);
					}
					//if doesn't contain a '*' then not indexed
					else
					{
						s.name = w;
						s.index = it_none;
						newSchema.push_back(s);
					}
				}
				//set new schema in Database only on the next line
				if (i == 1 && newRow.empty())
				{
					bool checkSchema = specifySchema(newSchema);
					//if schema is invalid then return false
					if (checkSchema == false) return false;
				}

				//everything else is a row
				if (i != 0)
					newRow.push_back(w);
			}
			//add the new row to the Database
			if (!newRow.empty())
			{
				bool checkRow = addRow(newRow);
				newRow.clear(); //clear row vector for next line
				//if row is invalid then return false
				if (checkRow == false) return false;
			}
			i++;
		}
		return true;
	}
	else
		return false;
}

bool Database::loadFromFile(std::string filename)
{
	cout << filename << endl;
	std::ifstream infile(filename);
	vector<FieldDescriptor> newSchema;
	
	vector<std::string> newRow;
	std::string delimiters = ",";//comma
	std::string tokenizeMe;
	int i = 0;
	if (infile.good())//check if file is valid
	{
		std::string line;
		//reading in line by line
		while(infile >> line)
		{
			//single line of a file that will be tokenized
			tokenizeMe = line;
			Tokenizer t(tokenizeMe, delimiters);
			string w;
			//first get the schema
			while (t.getNextToken(w))
			{
				//the first row is the schema settings
				if (i == 0)
				{
					FieldDescriptor s;
					//check if string contains a '*'
					//if so then that means it is indexed
					if (w.find('*') != std::string::npos)
					{
						//get rid of the * at the end
						s.name = w.substr(0, w.size() - 1);
						s.index = it_indexed;
						newSchema.push_back(s);
					}
					//if doesn't contain a '*' then not indexed
					else
					{
						s.name = w;
						s.index = it_none;
						newSchema.push_back(s);
					}
				}
				//set new schema in Database only on the next line
				if (i == 1 && newRow.empty())
				{
					bool checkSchema = specifySchema(newSchema);
					//if schema is invalid then return false
					if (checkSchema == false) return false;
				}
				
				//everything else is a row
				if (i != 0)
					newRow.push_back(w);
			}
			//add the new row to the Database
			if (!newRow.empty())
			{
				bool checkRow = addRow(newRow);
				newRow.clear(); //clear row vector for next line
				//if row is invalid then return false
				if (checkRow == false) return false;
			}
			i++;
		}
		return true;
	}
	else return false; //file is invalid
}

int Database::getNumRows() const
{
	//return number of rows
	return m_rows.size();
}

bool Database::getRow(int rowNum, std::vector<std::string>& row) const
{
	//if rowNum is invalid, then return false
	if (rowNum >= getNumRows())
		return false;

	//set row parameter to ROW at index rowNum of m_rows
	row = m_rows[rowNum];
	//return true
	return true;
}

int	Database::search(const std::vector<SearchCriterion>& searchCriteria,
	const std::vector<SortCriterion>& sortCriteria,
	std::vector<int>& results)
{

	//make a temp vector for the FIRST search field
	std::vector<int> temp_results,
		//other search fields
		other_results;
	//if user doesn't specify at least one search criterion, set results to empty and return error
	if (searchCriteria.empty())
	{
		results.clear();
		return ERROR_RESULT;
	}
	int temp_index; //store index of which element of the row is the field
	std::string temp_fieldname; //stores fieldname of the search Criteria
	
	for (int i = 0; i < searchCriteria.size(); i++)
	{
		bool fieldNameExists = false;

		temp_fieldname = searchCriteria[i].fieldName;

		//if any search criteria lack both minimum and maximum values, return error
		if (searchCriteria[i].minValue == "" && searchCriteria[i].maxValue == "")
		{
			//set results to empty
			results.clear();
			return ERROR_RESULT;
		}

		//search through m_schema to find the index of the current search criteria
		//to look at specific index of rows
		for (int j = 0; j < m_schema.size(); j++)
		{
			//fieldname must be indexed
			if (temp_fieldname == m_schema[j].name && m_schema[j].index == it_indexed )
			{
				fieldNameExists = true;
				temp_index = j;
				break;
			}
		}

		//if fieldname of search criteria doesn't exist in schema then return error
		if (fieldNameExists == false)
		{
			results.clear();
			return ERROR_RESULT;
		}

		//now check rows and get the ones that match the search criteria
		for (int k = 0; k < m_rows.size(); k++)
		{
			//set cur_field to the name of the field within the vector of vector of strings
			string cur_field = m_rows[k][temp_index];
			//MultiMap::Iterator iter = m_fieldIndex[temp_index]->findEqual(m_rows[k][temp_index]);

			//if search criteria minvalue isn't empty and maxvalue isn't empty
			if (searchCriteria[i].minValue != "" && searchCriteria[i].maxValue != "")
			{
				//check to see if the string of the vector within the row vector is within the max/min range
				if (cur_field >= searchCriteria[i].minValue && cur_field <= searchCriteria[i].maxValue)
				{
					//if the first search field, push into vector of results for first one
					if (i == 0)
						temp_results.push_back(k);
					else
						other_results.push_back(k);
				}
			}
			//if search criteria minvalue isn't empty but max value is
			if (searchCriteria[i].minValue != "" && searchCriteria[i].maxValue == "")
			{
				if (cur_field >= searchCriteria[i].minValue)
				{
					//if the first search field, push into vector of results for first one
					if (i == 0)
						temp_results.push_back(k);
					else
						other_results.push_back(k);
				}
			}
			//if search criteria minvalue is empty but max value isn't
			if (searchCriteria[i].minValue == "" && searchCriteria[i].maxValue != "")
			{
				if (cur_field <= searchCriteria[i].maxValue)
				{
					//if the first search field, push into vector of results for first one
					if (i == 0)
						temp_results.push_back(k);
					else
					{
						other_results.push_back(k);
					}
				}
			}
		}
		
	}
	//now we have to combine temp_results and other_results to only have the ones in both (intersection)
	std::vector<int> combined_results;
	if (searchCriteria.size() > 1)
	{
		for (int x = 0; x < temp_results.size(); x++)
		{
			//make sure to not add copies to other results
			bool isCopy = false;
			for (int j = 0; j < other_results.size(); j++)
			{
				//if the int is in both vectors and isn't a copy, add it to the combined vector
				if (temp_results[x] == other_results[j] && isCopy == false)
				{
					combined_results.push_back(temp_results[x]);
					//set isCopy to true to prevent any copies of the same element from being pushed into the combined vector
					isCopy = true;
				}
			}
		}
	}
	//if there is only one search criteria then set combined results to only temp results
	else if (searchCriteria.size() == 1)
		combined_results = temp_results;

	//if sort criteria is empty then set results to the combined results
	//and return the number of results found
	if (sortCriteria.empty())
	{
		results = combined_results;
		return combined_results.size();
	}
	
	//make a vector of the sort criteria indexes 
	vector<int> numofSortCriteria;

	//now sort the combined results
	for (int i = 0; i < sortCriteria.size(); i++)
	{
		bool sortNameExists = false;
		temp_fieldname = sortCriteria[i].fieldName;

		//search through m_schema to find the index of the current sort criteria
		//to look at specific index of rows
		for (int j = 0; j < m_schema.size(); j++)
		{
			//fieldname must be indexed
			if (temp_fieldname == m_schema[j].name)
			{
				sortNameExists = true;
				temp_index = j;
				numofSortCriteria.push_back(temp_index);
				break;
			}
		}
		//if fieldname of sort criteria doesn't exist in schema then return error
		if (sortNameExists == false)
		{
			results.clear();
			return ERROR_RESULT;
		}
	}

	//for each Sort Criteria field
	
	vector<int>::iterator idx = numofSortCriteria.begin();
	//std::vector<SortCriterion>::const_iterator sortCriteria_iter = sortCriteria.begin();
	vector<string> vec_Rowfields;

	//for each row# in combined_results
	for (int x = 0; x < combined_results.size(); x++)
	{
		//make a temp row and get the row at proper row#
		vector<string> temp_string;
		getRow(combined_results[x], temp_string);
		//now push back the field of the row into the vector of Row fields
		vec_Rowfields.push_back(temp_string[*idx]);
	}

	//vector<string> subvector;
	bool multi_copies = false;
	int temp_i = 0;
	OrderingType temp_OT;

	sort_function(idx, vec_Rowfields , multi_copies, temp_i, temp_OT, 
		searchCriteria, sortCriteria, numofSortCriteria);

	
	//make a vector of the proper order of rows
	vector<int> FINAL_RESULTS;
	for (int w = 0; w < vec_Rowfields.size(); w++)
	{
		int cur_val, next_val = 0;
		
		//get MultiMap Iterators to the last sort Criteria MultiMap fields
		MultiMap::Iterator temp_iter = m_fieldIndex[*(numofSortCriteria.end() - 1)]->findEqual(vec_Rowfields[w]);
		
		cur_val = temp_iter.getValue();

		if (w + 1 < vec_Rowfields.size())
		{
			temp_iter.next();
			FINAL_RESULTS.push_back(cur_val);
		}
		else if(w + 1 >= vec_Rowfields.size())
			FINAL_RESULTS.push_back(cur_val);

	}
	/*
	//assert(FINAL_RESULTS.size() == combined_results.size());
	
	assert(!vec_Rowfields.empty());
	
	vector<string>::iterator vit = vec_Rowfields.begin();
	assert(vit != vec_Rowfields.end());

	cout << endl << "----Reordered-----" << endl;
	for (vit; vit != vec_Rowfields.end(); vit++)
	{
		cout << *vit << endl;
	}
	cout << endl;
	cout << "------------------" << endl;
	*/
	//assert(FINAL_RESULTS.size() == combined_results.size());
	results = FINAL_RESULTS;
	return combined_results.size();
	
}

bool isGreaterthan(string& a, string& b)
{
	return (a > b);
}

//recursive sort function
//takes in arguments to be used to help sorting
void Database::sort_function(vector<int>::iterator& idx, vector<string>& vec_Rowfields
	, bool& multi_copies, int& temp_i, OrderingType& temp_OT, const std::vector<SearchCriterion>& searchCriteria,
	const std::vector<SortCriterion>& sortCriteria, vector<int>& numofSortCriteria)
{
	//get the OrderingType for current sort Criteria
	if (temp_i < sortCriteria.size())
		temp_OT = sortCriteria[temp_i].ordering;
	temp_i++;
	//for first sort criteria
	if (idx == numofSortCriteria.begin())
	{
		//order the MultiMap holding the correct field index

		if (temp_OT == ot_descending)
			sort(vec_Rowfields.begin(), vec_Rowfields.end(), isGreaterthan);
		if (temp_OT == ot_ascending)
			sort(vec_Rowfields.begin(), vec_Rowfields.end()); //default is in ascending order

		vector<string>::iterator rowField_iter = vec_Rowfields.begin();

		//now we must check for any matches that are equal
		for (int y = 0; y != vec_Rowfields.size(); y++)
		{
			//make a MultiMap iterator to find the correctly stored field
			MultiMap::Iterator map_iter = m_fieldIndex[*idx]->findEqual(rowField_iter[y]);

			for (int z = 0; z < vec_Rowfields.size(); z++)
			{
				if (y != z && vec_Rowfields[y] == vec_Rowfields[z])
				{
					multi_copies = true;
					vector<string>::iterator first = vec_Rowfields.begin() + y;
					//if yth element doesnt equal to next element
					if (z + 1 < vec_Rowfields.size() && vec_Rowfields[y] != vec_Rowfields[z + 1])
					{
						vector<string>::iterator last = vec_Rowfields.begin() + z;
						//put the copies into a sub vector
						//vector<string> subvector(&vec_Rowfields[y], &vec_Rowfields[z]);
						vector<string> subvector(first, last);
						//call the sort function on the subvecto
						sort_function(idx, subvector, multi_copies, temp_i, temp_OT,
							searchCriteria, sortCriteria, numofSortCriteria);
					}
				}
			}
		}
	}
}

