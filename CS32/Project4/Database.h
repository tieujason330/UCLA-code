#include <string>
#include <vector>
#include <iostream>
using namespace std;

class MultiMap;

class Database
{
public:
	enum IndexType { it_none, it_indexed };
	enum OrderingType { ot_ascending, ot_descending };

	struct FieldDescriptor
	{
		std::string	name;
		IndexType index;
	};

	struct SearchCriterion
	{
		std::string	fieldName;
		std::string	minValue;
		std::string	maxValue;
	};

	struct SortCriterion
	{
		std::string	fieldName;
		OrderingType ordering;
	};

	static const int ERROR_RESULT = -1;

	//default constructor containing no rows and no field descriptiosn in its schema
	//must run in O(1) time
	Database();

	//For a Database containing F fields in its schema and N rows, must run in O(FN) time
	~Database();

	//every time specifySchema is called, it must first reset database
	//discards all existing field descriptions in its schema, any existing rows, and any indexes
	//returns true if at least one indexed field, false if none
	bool specifySchema(const std::vector<FieldDescriptor>& schema);
	
	//used to add a new data record (row) into database
	//cannot add a row until a schema has been specified
	//row must have same number of fields as current schema (in same order)
	//returns true if successful in adding, false otherwise
	bool addRow(const std::vector<std::string>&	rowOfData);

	//loads a schema and data records from a webpage
	//if N is number of data records and schema has F indexed fields, runs at average O(FN logN) time
	bool loadFromURL(std::string url);

	//loads a schema and data records from a data file
	//if N is number of data records and schema has F indexed fields, runs at average O(FN logN) time
	bool loadFromFile(std::string filename);

	//returns the number of rows currently in database, runs at O(1) time
	int	getNumRows() const;

	//puts data from row at position rowNum in the database into the row vector parameter
	//runs in O(F) time, where F is the number of fields in each row
	//if rowNum is invalid, then getRow returns false and not change row parameter, otherwise return true
	bool getRow(int	rowNum, std::vector<std::string>& row) const;

	//searches for all rows that satisfy specific criteria
	//criterion must include name of one or more fields to be searched (matches exactly with an indexed field name in schema),
	//a minimum value for this field, and a maximum value for this field
	//for each criterion, user may omit one or the other of the min/max value, but cannot omit both (empty string "")
	//omitting min values indicate that criterion includes all rows with field value that is <= max
	//ommitting max value indicates that criterion includes all rows with a field value that is >= min
	int	search(const std::vector<SearchCriterion>& searchCriteria,
		const std::vector<SortCriterion>& sortCriteria,
		std::vector<int>& results);

private:
	bool m_schemaSpecified;
	int m_Row_cnt;
	std::vector<MultiMap*> m_fieldIndex; //vector of multimap ptrs for indexed fields
	std::vector<FieldDescriptor> m_schema; //vector for m_schema's fields
	std::vector<vector<std::string>> m_rows; //vector of vector of strings (vector of rows)

	//	To prevent Databases from being	copied or assigned,	declare	these members	
	//	private	and	do not implement them.	
	Database(const Database& other);
	Database& operator=(const Database&	rhs);

	//recursive sort function
	//takes in arguments to be used to help sorting
	void sort_function(vector<int>::iterator& idx, vector<string>& vec_Rowfields
		, bool& multi_copies, int& temp_i, OrderingType& temp_OT, const std::vector<SearchCriterion>& searchCriteria,
		const std::vector<SortCriterion>& sortCriteria, vector<int>& numofSortCriteria);
};

//function for descending sorting
bool isGreaterthan(string& a, string& b);



