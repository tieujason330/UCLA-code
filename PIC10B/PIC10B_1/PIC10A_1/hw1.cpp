/* Jason Tieu
   304047667
   PIC 10B hw1.cpp

   This program creates polynomials using user inputted coefficients. Then program 
   add, subtract, and multiply the two polynomials together to produce different
   polynomials that are displayed to the user.
*/
#include <iostream>
#include <vector>

using namespace std;

/** A polynomial class that takes input from the user to create polynomials with inputted coefficients. */
class Polynomial
{
public:
	Polynomial(); //default constructor
	Polynomial(vector<int> coeffs); //constructor w/parameters that sets the private variable coefficient = input parameter coeffs

	//Accessors - constant because no changes
	int Degree() const; //returns degree of the polynomial
	int Coefficient(int k) const; //returns the coeff of x^k
	void print() const; //prints out the polynomial

	//Mutators
	void constantMultiply(int x); //multiplies polynomial by integer constant, x
	void Transform(); //transforms the polynomial (derivative)

private:
	vector<int> coefficient;
};

//Polynomial non-member functions
void getPoly(Polynomial& p);
Polynomial Addition(const Polynomial& p1,const Polynomial& p2);
Polynomial Subtraction(const Polynomial& p1, const Polynomial& p2);
Polynomial Multiplication(const Polynomial& p1, const Polynomial& p2);

int main()
{
	vector<int> coeffs;
	Polynomial poly1(coeffs), poly2(coeffs);
	
	cout << "Welcome! Please input the coefficients of the first polynomial." << endl <<
		"When you are finished, enter -123456789." << endl;
	getPoly(poly1);
	cout << endl << "Your first polynomial is ";
	poly1.print();
	cout << endl;

	cout << endl << "Please input the coefficients of the second polynomial." << endl;
	getPoly(poly2);
	cout << endl << "Your second polynomial is ";
	poly2.print();
	cout << endl;
	
	cout << endl << "The sum of these polynomials is" << endl << endl;
	Addition(poly1, poly2).print();
	cout << endl;
	
	cout << endl << "The first minus the second is" << endl << endl;
	Subtraction(poly1, poly2).print();
	cout << endl;

	cout << endl << "The first multiplied by the second is: " << endl << endl;
	Multiplication(poly1,poly2).print();
	cout << endl;

	// Test to see if constantMultiply works.
	/*
	cout << endl << "poly1 * constant 5 : " << endl << endl;
	poly1.constantMultiply(5);
	poly1.print();
	cout << endl;
	*/

	// Test to see if Transform works.
	/*
	cout << endl << "Derivative of poly1 : " << endl << endl;
	poly1.Transform();
	cout << endl;
	*/

	cout << endl << "Thanks for using my program!" << endl << endl;

	system("pause");
	return 0;
}

/** A Polynomial constructor with parameters that sets private variable coefficient to input parameter coeffs
	
	@param coeff is the inputted coefficient
*/
Polynomial::Polynomial(vector<int> coeff)
{
	coefficient = coeff;
}

/** A function that returns the degree of a polnomial

	@return coefficient.size() - 1 since degree is 1 less than the number of coefficients
*/
int Polynomial::Degree() const
{
	return coefficient.size() - 1;
	//if there were 7 coefficients, degree would be 6 since first is constant (x^0)
}

/** function that returns the kth value of the vector coefficient
	
	@param k represents the element number of coefficient
	@return coefficient[k]
*/
int Polynomial::Coefficient(int k) const
{
	return coefficient[k]; //returns element # 
}

/** This function prints out the polynomial using a series of if statements that determine
	how each coefficient should be outputted depending on the inputted coefficient.
*/
void Polynomial::print() const
{	
	bool firstTerm = 1;

	for (int i = 0; i < coefficient.size(); i++)
	{
		if (coefficient[i] != 0)
		{
			if (coefficient[i] == 0)
			{
				cout << "";
			}
			if (coefficient[i] == 1)
			{
				if (i == 0)
					cout << coefficient[i];
				if (i == 1)
				{
					if (coefficient[i-1] == 0)
						cout << "x";
					else
						cout << "+x";
				}
				if (i > 1)
				{
					if (firstTerm == 1)
						cout << "x^" << i;
					else
						cout << "+x^" << i;
				}
			}
			if (coefficient[i] == -1)
			{
				if (i == 0)
					cout << coefficient[i];
				if (i == 1)
					cout << "-x";
				else if (i != 1 && i != 0)
					cout << "-x^" << i;
			}
			else if (coefficient[i] > 1)
			{
				if (firstTerm == 1 && i == 1)
					cout << coefficient[i] << "x";
				else if (firstTerm == 0 && i == 1)
					cout << "+" << coefficient[i] << "x";

				else if (firstTerm == 1 && i > 1)
					cout << coefficient[i] << "x^" << i;
				else if (firstTerm == 0 && i > 1)
					cout << "+" << coefficient[i] << "x^" << i;

				else if (i == 0)
					cout << coefficient[i];
			}
			if (coefficient[i] < -1)
			{
				if (i == 0)
					cout << coefficient[i];
				if (i == 1)
					cout << coefficient[i] << "x";
				if (i > 1)
					cout << coefficient[i] << "x^" << i;
			}
			firstTerm = 0;
		}
	}
	return;
}

/** This function multiplies the coefficients of a polynomial with a constant

	@param x is a constant that is used to augment the polynomial
*/
void Polynomial::constantMultiply(int x)
{
	for(int i = 0; i < coefficient.size(); i++)
	{
		coefficient[i] *= x;
	}
	return;
}

/** A function that takes the derivative of the polynomial. 
Due to complications, used a modified version of my Polynomial print() function in here 
*/
void Polynomial::Transform()
{
	//for(int n = 1, i=0; n < coefficient.size(); n++, i++)
	//{
		//coefficient[i] *= i;
		bool firstTerm = 1;

	for (int i = 0; i < coefficient.size(); i++)
	{
		coefficient[i] *= i;
		if (coefficient[i] != 0)
		{
			if (coefficient[i] == 0)
			{
				cout << "";
			}
			if (coefficient[i] == 1)
			{
				if (i == 0 || i == 1)
					cout << coefficient[i];
			}
			
			if (coefficient[i] == -1)
			{
				if (i != 1 && i != 0)
					cout << "-x^" << i-1;
			}
			else if (coefficient[i] > 1)
			{
				if (firstTerm == 1 && i == 1)
					cout << coefficient[i];
				else if (firstTerm == 0 && i == 1)
					cout << "+" << coefficient[i];
				else if (firstTerm == 1 && i > 1)
				{
					--i;
					if (i > 1)
						cout << coefficient[i] << "x^" << i-1;
					else if (i == 1)
						cout << coefficient[i];
				}
				else if (firstTerm == 0 && i > 1)
				{
					if (i > 1 && i > 2)
						cout << "+" << coefficient[i] << "x^" << i-1;
					else if (i == 2)
						cout << "+" << coefficient[i] << "x";
				}
			}
			if (coefficient[i] < -1)
			{
				if (i == 1)
					cout << coefficient[i];
				if (i > 1)
					cout << coefficient[i] << "x^" << i-1;
			}
			firstTerm = 0;
		}
	}
 
	return;
}

/** This function retrieves the input from the user and stores in the vector v.
	
	@param poly, passing by reference the Polynomial poly in order to directly access and manipulate it
*/
void getPoly(Polynomial& poly)
{
	int input;
	vector<int> v;

	while(cin >> input)
	{
		v.push_back(input);

		if (input == -123456789)
		{
			v.pop_back();
			break;
		}
	}
	poly = Polynomial(v);

	return;
}

/** This function adds the two polynomials together

	@param p1, p2 - both parameters are constant since they are not changed
	@return v - returns a new vector comprising of the addition of the coefficients of the two polynomials
*/
Polynomial Addition(const Polynomial& p1, const Polynomial& p2)
{
	int newDegree;
	int* p; 
	p = &newDegree;// p = address of newDegree

	if (p1.Degree() >= p2.Degree())
		*p = p1.Degree(); //newDegree = p1.Degree()
	else if (p1.Degree() < p2.Degree())
		*p = p2.Degree();

	vector<int> newCoeff(newDegree + 1, 0);

	for (int i = 0; i <= p1.Degree() ; ++i)
			newCoeff[i] += p1.Coefficient(i);
	for (int j = 0; j <= p2.Degree() ; ++j)
			newCoeff[j] += p2.Coefficient(j);
	return newCoeff;
}

/** This function subtracts the two polynomials together

	@param p1, p2 - both parameters are constant since they are not changed
	@return v - returns a new vector comprising of the subtraction of the coefficients of the two polynomials
*/
Polynomial Subtraction(const Polynomial& p1, const Polynomial& p2)
{
	int newDegree;
	int* p; 
	p = &newDegree;// p = address of newDegree

	if (p1.Degree() >= p2.Degree())
		*p = p1.Degree(); //newDegree = p1.Degree()
	else if (p1.Degree() < p2.Degree())
		*p = p2.Degree();

	vector<int> newCoeff(newDegree + 1, 0);

	for (int i = 0; i <= p1.Degree() ; ++i)
			newCoeff[i] += p1.Coefficient(i);
	for (int j = 0; j <= p2.Degree() ; ++j)
			newCoeff[j] -= p2.Coefficient(j);
	return newCoeff;
}

/** This function multiplies the two polynomials together

	@param p1, p2 - both parameters are constant since they are not changed
	@return v - returns a new vector comprising of the product of the two polynomials
*/
Polynomial Multiplication(const Polynomial& p1, const Polynomial& p2)
{	
	int newDegree = p1.Degree() + p2.Degree();
	vector<int> newCoeff(newDegree + 1, 0);

	for (int i = 0; i < p1.Degree() + 1 ; i++)
	{
		for (int j = 0; j < p2.Degree() + 1; j++)
			newCoeff[i + j] += p1.Coefficient(i) * p2.Coefficient(j);
	}
	return newCoeff;
}