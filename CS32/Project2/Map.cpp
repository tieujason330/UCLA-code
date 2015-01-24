#include"Map.h"
#include<iostream>
using namespace std;

Map::Map()
{
	m_head = NULL;
	m_size = 0;
}

Map::Map(const Map& other)
{
	m_head = 0;
	m_size = 0;
	Node* newNode = new Node;
	for (newNode = other.m_head; newNode != NULL; newNode = newNode->m_next)
	{
		if (newNode != NULL)
		{
			insert(newNode->m_key, newNode->m_value);
		}
	}
}

Map::~Map()
{ 
	Node* p;
	while (m_head != NULL)
	{
		p = m_head->m_next; // p equals to head's next node
		delete m_head; //delete head
		m_head = p; // set head to p
	}
}


Map& Map::operator=(const Map& other)
{
	Map temp(other);
	m_head = temp.m_head;
	return (*this);
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
	if (contains(key))
		return false;
	
	Node* p;
	for (p = m_head; p != NULL && p->m_next != NULL; p = p->m_next)
		;

	if (p !=  NULL)
	{
		Node* newNode = new Node;
		newNode->m_key = key;
		newNode->m_value = value;
		newNode->m_prev = p;
		newNode->m_next = NULL;
		p->m_next = newNode;
		m_size++;
		return true;
	}
	else
	{
		Node* newNode = new Node;
		newNode->m_key = key;
		newNode->m_value = value;
		newNode->m_next = NULL;
		newNode->m_prev = NULL;//**
		m_head = newNode;
		m_size++;
		return true;
	}
	
}

bool Map::update(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
{
	if (!contains(key))
		return false;

	Node* p;
	for (p = m_head; p != NULL && p->m_key != key; p = p->m_next)
		;
	if (p!=NULL)
		p->m_value = value;

	return true;
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
	if (!contains(key))
		return false;
	Node* p;
	for (p = m_head; p != NULL && p->m_key != key; p->m_prev = p, p = p->m_next)
		;
	if (p != NULL)
	{
		m_head = p->m_next;

		//only if p->m_prev is not null, change it
		if (p->m_prev != NULL)
			p->m_prev->m_next = p->m_next;
		//only if p->m_next is not null, change it
		if (p->m_next != NULL)
			p->m_next->m_prev = p->m_prev;

	}
	delete p;
	m_size--;
	return true;
}

bool Map::contains(const KeyType& key) const
// Return true if key is equal to a key currently in the map, otherwise
// false.
{
	Node* p;
	for (p = m_head; p != NULL && p->m_key != key; p = p->m_next)
		;
	if (p == NULL)
		return false;
	if (p != NULL)
		return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
{
	if (!contains(key))
		return false;

	Node* p;
	for (p = m_head; p != NULL && p->m_key != key; p = p->m_next)
		;
	value = p->m_value;
	return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
{
	if (i < 0 || i > size())
		return false;

	int current_spot = 0;
	Node * p;

	for (p = m_head; p != NULL && current_spot != i; p = p->m_next)
		current_spot++;

	key = p->m_key;
	value = p->m_value;
	return true;
}

void Map::swap(Map& other)
// Exchange the contents of this map with the other one.
{
	Node* p;
	p = m_head;
	m_head = other.m_head;
	other.m_head = p;

}

bool combine(const Map& m1, const Map& m2, Map& result)
{
	KeyType k, k_check;
	ValueType v, v_check;
	bool check = true;
	for (int i = 0; i < m1.size(); i++)
	{
		m1.get(i, k, v);
		if (!m2.contains(k))
			result.insert(k, v);
		else
		{
			m2.get(k, v_check);
			if (v == v_check)
				result.insert(k, v);
			else
				check = false;
		}
	}
	for (int j = 0; j < m2.size(); j++)
	{
		m2.get(j, k, v);
		if (!m1.contains(k))
			result.insert(k, v);
		else
		{
			m1.get(k, v_check);
			if (v == v_check)
				result.insert(k, v);
			else
				check - false;
		}
	}
	return check;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
	KeyType k;
	ValueType v;
	for (int i = 0; i < m1.size(); i++)
	{
		m1.get(i, k, v);

		if (!m2.contains(k))
			result.insert(k, v);
	}
}
