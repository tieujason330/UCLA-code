/*
#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Map m;  // maps strings to doubles
	assert(m.empty());
	ValueType v = -1234.5;
	assert(!m.get("abc", v) && v == -1234.5); // v unchanged by get failure
	m.insert("xyz", 9876.5);
	assert(m.size() == 1);
	KeyType k = "hello";
	assert(m.get(0, k, v) && k == "xyz"  &&  v == 9876.5);
	cout << "Passed all tests" << endl;
}
*/
/*
#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
Map m;  // maps ints to strings
assert(m.empty());
ValueType v = "Ouch";
assert(!m.get(42, v) && v == "Ouch"); // v unchanged by get failure
m.insert(123456789, "Wow!");
assert(m.size() == 1);
KeyType k = 9876543;
assert(m.get(0, k, v) && k == 123456789 && v == "Wow!");
cout << "Passed all tests" << endl;
}
*/