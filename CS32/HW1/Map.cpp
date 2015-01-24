#include"Map.h"
#include<iostream>
using namespace std;

Map::Map()
{
	m_size = 0;
}

bool Map::empty() const
// Return true if the map is empty, otherwise false.
{
	if (m_size == 0)
		return true;
	else
		return false;
}

int Map::size() const
// Return the number of key/value pairs in the map.
{
	return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full.
{
	if (contains(key) || size() == DEFAULT_MAX_ITEMS)
		return false;
	Pair n_pair;
	n_pair.m_key = key;
	n_pair.m_value = value;

	m_pairs[m_size] = n_pair;
	m_size++;
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
			m_size--;

			for (i; i < m_size; i++)
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
	int smaller_size, remainder;
	bool other_smaller;

	//check which is the smaller one
	//prevents copying undefined elements
	if (m_size > other.size())
	{
		smaller_size = other.size();
		remainder = m_size - other.size();
		other_smaller = true;
	}
	else
	{
		smaller_size = m_size;
		remainder = other.size() - m_size;
		other_smaller = false;
	}

	Pair p;

	for (int x = 0; x < smaller_size; x++)
	{
		p = m_pairs[x];
		m_pairs[x] = other.m_pairs[x];
		other.m_pairs[x] = p;
	}

	//copy non-overlapping elements
	//fix the sizes after copying
	if (other_smaller)
	{
		for (int x = 0; x < m_size; x++)
		{
			p = m_pairs[x];
			other.m_pairs[x] = p;
		}
		m_size -= remainder;
		other.m_size += remainder;
	}
	else
	{
		for (int x = 0; x < other.m_size; x++)
		{
			p = other.m_pairs[x];
			m_pairs[x] = p;
			other.m_size -= remainder;
			m_size += remainder;
		}
	}


}
