/*
Jason Tieu
304047667
This project deals with sets and their operations
*/

#include<iostream>
#include<set>
using namespace std;

set<int> SetUnion(const set<int>& s1, const set<int>& s2);
set<int> SetIntersection (const set<int>& s1, const set<int>& s2);
set<int> SetDifference (const set<int>& s1, const set<int>& s2);
set<int> SymmetricDifference(const set<int>& s1, const set<int>& s2);
set<set<int>> PowerSet(const set<int>& s);
set<int> Complement(const set<int>& s1, const int n);

//SetUnion operators
set<int> operator+(const set<int> s1, const set<int> s2);
set<int> operator+(const set<int> s, const int n);
set<int> operator+(const int n, const set<int> s);
//SetDifference operators
set<int> operator-(const set<int> s1, const set<int> s2);
set<int> operator-(const set<int> s, const int n);
set<int> operator-(const int n, const set<int> s);
//SetIntersection operators
set<int> operator^(const set<int> s1, const set<int> s2);
set<int> operator^(const set<int> s, const int n);
set<int> operator^(const int n, const set<int> s);
//SymmetricDifference operators
set<int> operator%(const set<int> s1, const set<int> s2);
set<int> operator%(const set<int> s, const int n);
set<int> operator%(const int n, const set<int> s);
//Compliment operator
set<int> operator~(const set<int> s);

//outputs set of elements in set form (ex {1,3,4,6,7}
ostream& operator<<(ostream& out, const set<int>& s);
//inputs the set of elements one integer at a time
istream& operator>>(istream& in, set<int>& s);
//outputs a set of sets in set form (ex {{1,2},{1,3}}
ostream& operator<<(ostream& out, const set<set<int>>& s);

int largest(const set<int>& s);

int main()
{
	set<int> A, B;

	cout << "Please input a set of nonnegative numbers for a set (Enter -1 when you are finished):" << endl;
	cin >> A;
	cout << endl << "We'll refer to this set as A from now on." << endl;
	cout << "A = " << A << endl;
	cout << "~A = " << ~A << endl << endl;

	cout << "Please input a set of nonnegative numbers for a set (Enter -1 when you are finished):" << endl;
	cin >> B;
	cout << endl << "We'll refer to this set as B from now on." << endl;
	cout << "B = " <<  B << endl;
	cout << "~B = " << ~B << endl << endl;

	cout << "Now we will do some arithmetic:" << endl;
	cout << "A+B = " << A+B << endl;
	//issues with cout << A^B;
	set<int> ss = A^B;
	cout << "A^B = " << ss << endl;
	cout << "A-B = " << A-B << endl;
	cout << "B-A = " << B-A << endl;
	cout << "A%B = " << A%B << endl;
	cout << endl << "The power set of A^B is" << endl;
	cout << PowerSet(A^B);

	cout << endl;
	system("pause");
	return 0;
}
/**
	SetUnion: set<int> x set<int> ==> set<int> takes the union of the set elements
	union combines all elements of A and B
	@param s1, s2 are the sets being compared
	@return s new set containing union
*/
set<int> SetUnion(const set<int>& s1, const set<int>& s2)
{
	set<int> s;
	for (set<int>::iterator it = s1.begin(); it != s1.end(); ++it)
		s.insert(*it);
	for (set<int>::iterator it = s2.begin(); it != s2.end(); ++it)
		s.insert(*it);
	return s;
}

/**
	SetIntersection is the set of all objects that are in BOTH A AND B
	@param s1, s2 are the sets being compared
	@return s new set containing intersection
*/
set<int> SetIntersection (const set<int>& s1, const set<int>& s2)
{
	set<int> s;
	for (set<int>::iterator it1 = s1.begin(); it1 != s1.end(); ++it1)
	{
	for (set<int>::iterator it2 = s2.begin(); it2 != s2.end(); ++it2)
		if (*it1 == *it2)
			s.insert(*it1);
	}
	return s;
}

/**
	SetDifference is a set containing elements in A that are NOT in B (order matters)
	@param s1, s2 are the sets being compared
	@return s new set containing difference
*/
set<int> SetDifference (const set<int>& s1, const set<int>& s2)
{
	set<int> s;
	for (set<int>::iterator it = s1.begin(); it != s1.end(); ++it)
	{
		if (s2.count(*it) == 0)
			s.insert(*it);
	}
	return s;
}

/**
	SymmetricDifference it the set of all objects that appear in one of A or B, but not in both
	@param s1, s2 are sets being compared
	@return s new set containing SymmetricDifference
*/
set<int> SymmetricDifference(const set<int>& s1, const set<int>& s2)
{
	set<int> s;
	for (set<int>::iterator it = s1.begin(); it != s1.end(); ++it)
	{
		if (s2.count(*it) == 0)
			s.insert(*it);
	}
	for (set<int>::iterator it = s2.begin(); it != s2.end(); ++it)
	{
		if (s1.count(*it) == 0)
			s.insert(*it);
	}
	return s;
}

/**
	PowerSet: set<int> => set<set<int>> => returns a set of sets of integers
	@param s is the set 
	@ return ps is set of all sets
*/
set<set<int>> PowerSet(const set<int>& s)
{
	//base case
	set<set<int>> ps;
	if (s.size() == 0)
	{
		ps.insert(set<int>());
		return ps;
	}

	//recursion

	//get the smaller set
	set<int>::iterator it = s.begin();
	int n = *it;
	set<int> s1 = s;
	s1.erase(n);

	//recursively called on the smaller set
	set<set<int>> ps1 = PowerSet(s1);

	//insert n into each of the set in ps1
	set<set<int>> ps2;
	for (set<set<int>>::iterator it = ps1.begin(); it != ps1.end(); ++it)
	{
		set<int> ss = *it;
		ss.insert(n);
		ps2.insert(ss);
	}

	//the result is the union of ps1 and ps2
	for (set<set<int>>::iterator it = ps1.begin(); it != ps1.end(); ++it)
		ps.insert(*it);
	for (set<set<int>>::iterator it = ps2.begin(); it != ps2.end(); ++it)
		ps.insert(*it);

	return ps;
}

/**
	Takes the complement of the first argument with respect to set {1,2,3,...n}, where n must be positive
	@param s1 is first argument
	@param n is second argument
	@return s is new set containg complement
*/
set<int> Complement(const set<int>& s1, const int n)
{
	set<int> s;
	for (int i = 1; i<=n; i++)
	{
		if (s1.count(i) == 0)
			s.insert(i);
	}
	return s;
}

/**
	operator+: set<int> x set<int> => set<int> takes the union of the sets.
	@param s1, s2 are the sets
	@return the union of the sets
*/
set<int> operator+(const set<int> s1, const set<int> s2)
{
	return SetUnion(s1, s2);
}
/**
	operator+: set<int> x int => set<int> takes the union of the set with the
	int second argument.
	@param set is first argument
	@param int is second argument
	@return union of the sets
*/
set<int> operator+(const set<int> s, const int n)
{
	set<int> s1;
	s1.insert(n);
	return SetUnion(s,s1);
}

/**
	operator+: int x set<int> => set<int> takes the union of the set with the
	int first argument.
	@param int is first argument
	@param set is second argument
	@return union of the sets
*/
set<int> operator+(const int n, const set<int> s)
{
	set<int> s1;
	s1.insert(n);
	return SetUnion(s1,s);
}

/**
	operator-: set<int> x set<int> => set<int> takes the set difference of the
	sets.
	@param s1, s2 are arguments that are sets
	@return set that is the difference of the sets
*/
set<int> operator-(const set<int> s1, const set<int> s2)
{
	return SetDifference(s1, s2);
}

/**
	operator-: set<int> x int => set<int> takes the set difference of the set with
	the int second argument.
	@param set is first argument
	@param int is second argument
	@return set that is the difference of the sets
*/
set<int> operator-(const set<int> s, const int n)
{
	set<int> s1;
	s1.insert(n);
	return SetDifference(s,s1);
}
/**
	operator-: int x set<int> => set<int> takes the set difference of the set with
	the int first argument.
	@param int is first argument
	@param set is second argument
	@return set that is the difference of the sets
*/
set<int> operator-(const int n, const set<int> s)
{
	set<int> s1;
	s1.insert(n);
	return SetDifference(s1,s);
}

/**
	operator^: set<int> x set<int> => set<int> takes the intersection of the sets.
	@param s1, s2 are the set arguments
	@return set with intersection of the sets
*/
set<int> operator^(const set<int> s1, const set<int> s2)
{
	return SetIntersection(s1, s2);
}

/**
	operator^: set<int> x int => set<int> takes the intersection of the set with
	the int second argument.
	@param set is first argument
	@param int is second argument
	@return set with intersection of the sets
*/
set<int> operator^(const set<int> s, const int n)
{
	set<int> s1;
	s1.insert(n);
	return SetIntersection(s,s1);
}

/**
	operator^: int x set<int> => set<int> takes the intersection of the set with
	the int first
	@param int is first argument
	@param set is second argument
	@return set with intersection of the sets
*/
set<int> operator^(const int n, const set<int> s)
{
	set<int> s1;
	s1.insert(n);
	return SetIntersection(s1,s);
}

/**
	operator%: set<int> x set<int> => set<int> takes the symmetric difference
	of the sets.
	@param s1, s2 are set arguments
	@return set with symmetricdifference of the sets
*/
set<int> operator%(const set<int> s1, const set<int> s2)
{
	return SymmetricDifference(s1, s2);
}

/**
	operator%: set<int> x int => set<int> takes the symmetric difference of the
	set with the int second argument.
	@param set is first argument
	@param int is second argument
	@return set with symmetricdifference of the sets
*/
set<int> operator%(const set<int> s, const int n)
{
	set<int> s1;
	s1.insert(n);
	return SymmetricDifference(s,s1);
}

/**
	operator%: int x set<int> => set<int> takes the symmetric difference of the
	set with the int first argument.
	@param int is first argument
	@param set is second argument
	@return set with symmetricdifference of the sets
	
*/
set<int> operator%(const int n, const set<int> s)
{
	set<int> s1;
	s1.insert(n);
	return SymmetricDifference(s1,s);
}

/**
	operator: set<int> => set<int> takes the complement with respect to the set
	@param set is the argument
	@return complement of the set
*/
set<int> operator~(const set<int> s)
{
	int n = largest(s);
	return Complement(s, n);
}

/**
	operator<<: ostream x set<int> => ostream outputs the set of elements in set
	form
	@param out is output object
	@param set is object being printed
	@return out is output object
*/
ostream& operator<<(ostream& out, const set<int>& s)
{
	int size = s.size();
	if (!size)
	{
		out << "{}";
		return out;
	}
	//{1,2,3}
	set<int>::iterator it = s.begin();
	out << "{";
	for (int i = 0; i <size-1; i++)
	{
		out << *(it++) << ", ";
	}
	out << *it << "}";
	return out;
}

/**
	operator>>: istream x set<int> => istream inputs the set of elements one integer at a time.
	@param in is input object
	@param set is object being inputted
	@return in is input object
*/
istream& operator>>(istream& in, set<int>& s)
{
	while(1)
	{
		int n;
		in >> n;
		if (n != -1)
			s.insert(n);
		else
			break;
	}
	return in;
}

/**
	operator<<: ostream x set<set<int>> => ostream outputs a set of sets in set
	form
	@param out is output object
	@set<set<int>> is set of sets being printed
	@return out is output object
*/
ostream& operator<<(ostream& out, const set<set<int>>& s)
{
	int size = s.size();
	if (!size)
	{
		out << "{}";
		return out;
	}
	//{1,2,3}
	set<set<int>>::iterator it = s.begin();
	out << "{";
	for (int i = 0; i < size-1; i++)
	{
		out << *(it++) << ", ";
	}
	out << *it << "}";
	return out;
}

/**
	largest finds the largest value of a set - helper function for complement
	@param set is the argument
	@return int is largest value in set
*/
int largest(const set<int>& s)
{
	if (!s.size())
		return -1;
	set<int>::iterator it = s.end();
	--it;//decrements because points at null, decrement to last element
	return *it;
}