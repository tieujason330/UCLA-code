#include"newMap.h"
#include<iostream>
using namespace std;

Map::Map(int amount)
:m_size_amount(amount), m_size_used(0)
{
	if (amount < 0)
	{
		cout << "Error: array has too few elements." << endl;
		exit(0);
	}
	m_pairs = new Pair[amount];
}

//Destructor
Map::~Map()
{
	delete[] m_pairs;
}

//Copy Constructor
Map::Map(const Map& other)
{
	m_size_amount = other.m_size_amount;
	m_size_used = other.m_size_used;
	m_pairs = new Pair[m_size_amount];

	for (int i = 0; i < m_size_used; i++)
	{
		m_pairs[i] = other.m_pairs[i];
	}
}

Map& Map::operator=(const Map& other)
{
	//to prevent aliasing
	if (m_pairs == other.m_pairs)
		return (*this);

	m_size_amount = other.m_size_amount;
	m_size_used = other.m_size_used;

	delete[] m_pairs;
	m_pairs = new Pair[m_size_amount];
	for (int i = 0; i < m_size_used; i++)
	{
		m_pairs[i] = other.m_pairs[i];
	}
	return (*this);
}

bool Map::empty() const
// Return true if the map is empty, otherwise false.
{
	if (m_size_used == 0)
		return true;
	else
		return false;
}

int Map::size() const
// Return the number of key/value pairs in the map.
{
	return m_size_used;
}

bool Map::insert(const KeyType& key, const ValueType& value)
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full.
{
	if (contains(key) || size() == m_size_amount)
		return false;
	Pair n_pair;
	n_pair.m_key = key;
	n_pair.m_value = value;

	m_pairs[m_size_used] = n_pair;
	m_size_used++;
	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
{
	if (!contains(key))
		return false;

	for (int i = 0; i < size(); i++)
	{
		if (m_pairs[i].m_key == key)
		{
			m_pairs[i].m_value = value;
			return true;
		}
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full.
{
	if (contains(key))
		return update(key, value);
	else
		return insert(key, value);
}

bool Map::erase(const KeyType& key)
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
{
	for (int i = 0; i < size(); i++)
	{
		if (m_pairs[i].m_key == key)
		{
			m_size_used--;

			for (i; i < m_size_used; i++)
			{
				m_pairs[i] = m_pairs[i + 1];
			}
			return true;
		}
	}
	return false;
}

bool Map::contains(const KeyType& key) const
// Return true if key is equal to a key currently in the map, otherwise
// false.
{
	for (int i = 0; i < size(); i++)
	{
		if (m_pairs[i].m_key == key)
		{
			return true;
		}
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
{
	for (int i = 0; i < size(); i++)
	{
		if (m_pairs[i].m_key == key)
		{
			value = m_pairs[i].m_value;
			return true;
		}
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
{
	if (i >= 0 && i <= size())
	{
		key = m_pairs[i].m_key;
		value = m_pairs[i].m_value;
		return true;
	}
	return false;
}

void Map::swap(Map& other)
// Exchange the contents of this map with the other one.
{
	int temp_size_used = m_size_used;
	m_size_used = other.m_size_used;
	other.m_size_used = temp_size_used;

	int temp_size_amount = m_size_amount;
	m_size_amount = other.m_size_amount;
	other.m_size_amount = temp_size_amount;

	Pair* p = m_pairs;
	m_pairs = other.m_pairs;
	other.m_pairs = p;
}
