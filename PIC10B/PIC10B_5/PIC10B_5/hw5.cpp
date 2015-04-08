/*
Jason Tieu
4-30-13
304047667
For this project, we utilized recursion to compute the number of integer paritions of a chosen number n 
with each element at most k (which starts from 1 to n).
*/
#include<iostream>
#include<fstream>
using namespace std;

/** This is a recursive function that computes the number of integer partitions of n with each element at most k
	@param k is the max that each element can be
	@param n is the number being partitioned
	@return p(k, n - k) + p(k - 1, n) is the recursive formula for getting the partitions
*/
int p(const int k, const int n)
{
	//p(k, n) = 0 for k < 0 or n < 0. I.e., any negative input is automatically 0.
	if (k < 0 || n < 0)
		return 0;
	//p(0,0) = 1
	if (k == 0 && n == 0)
		return 1;
	//p(0,n) = 0 for all n > 0
	if ( k == 0 && n > 0)
		return 0;
	//p(k,0) = 1 for all k > 0
	if (k > 0 && n == 0)
		return 1;
	//p(k,n) = p(n,n) when k >= n
	if (k > n)
		return p(n,n);

	return p(k, n - k) + p(k - 1, n);
}

int main()
{
	cout << "Welcome! Please input a number m: ";
	int m;
	cin >> m;
	cout << endl;
	for (int i = 1; i <= m; i++)
		cout << "p(" << i << "," << m << ") = " << p(i,m) << endl;
	
	cout << endl << "Creating file ... ";

	ofstream out;
	out.open("pknTable.txt");

	if (out.fail())
		cout << "Error!" << endl;

	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			out << p(i,j) << " ";
		}
		out << endl;
	}

	out.close();
	cout << "Done." << endl;
	system("pause");
	return 0;
}
