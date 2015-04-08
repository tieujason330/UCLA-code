#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

int main() { 
	string myName = "Blake Hunter", fullName;
	int size = myName.length(); //return length of string - 12.
	//length is a member function of the string class.
	string firstName = myName.substr(0, 5); //syntax .substr(starting location, length)
	string lastName = myName.substr(6,6); 

	//other string functions
	fullName = myName.insert(5, "ster"); //BlakesterHunter
	fullName.erase(6,3) //Blakes

	cout << setprecision(2) << 1.20; //1.2
	cout << fixed << setprecision(2) << "$" << 1.2; //$1.20

	return 0;
}