/* 
Jason Tieu
304047667
In this project, I used operator overloading to allow the Fraction and Rational classes to use
operators such as +, *, ++, and etc. This overloading allows for much simpler to understand code,
and also to manipulate operators into taking in user constructed objects as arguments.
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
	double evaluateAt(double x) const;

	//member operators
	Polynomial& operator++();//prefix
	Polynomial operator++(int unused);//postfix
	Polynomial& operator--();
	Polynomial operator--(int unused);
	Polynomial& operator+=(const Polynomial& p);
	Polynomial& operator-=(const Polynomial& p);
	Polynomial& operator*=(const Polynomial& p);

private:
	vector<int> coefficient;
};

//Polynomial non-member functions
void getPoly(Polynomial& p);

Polynomial operator+(const Polynomial& p1,const Polynomial& p2);
Polynomial operator-(const Polynomial& p1, const Polynomial& p2);
Polynomial operator*(const Polynomial& p1, const Polynomial& p2);

//Polynomial boolean operators
bool operator==(const Polynomial& p1, const Polynomial& p2);
bool operator!=(const Polynomial& p1, const Polynomial& p2);
bool operator<(const Polynomial& p1, const Polynomial& p2);
bool operator>(const Polynomial& p1, const Polynomial& p2);

class Rational
{
public:
	Rational();//default constructor
	Rational(Polynomial p);//constructor that sets numerator = p, denominator = 1
	Rational(Polynomial pN, Polynomial pD);//constuctor w/ parameters for numerator and denominator

	double evaluateAt(double x) const;
	void print();
	Polynomial getNumerator() const;//for nonmember functions to access
	Polynomial getDenominator() const;
	
	//member operators
	Rational& operator++();//prefix
	Rational operator++(int unused);//postfix
	Rational& operator--();
	Rational operator--(int unused);
	Rational& operator+=(const Rational& r);
	Rational& operator-=(const Rational& r);
	Rational& operator*=(const Rational& r);
	Rational& operator/=(const Rational& r);

private:
	Polynomial numerator, denominator;
};

//Rational nonmember functions
Rational operator+(const Rational& r1, const Rational& r2);
Rational operator-(const Rational& r1, const Rational& r2);
Rational operator*(const Rational& r1, const Rational& r2);
Rational operator/(const Rational& r1, const Rational& r2);

int main()
{
	vector<int> coeffs;
	Polynomial poly1(coeffs), poly2(coeffs);
	
	cout << "Welcome! Please input the coefficients of the first polynomial, p" << endl <<
		"When you are finished, enter -123456789." << endl;
	getPoly(poly1);
	cout << endl << "Your first polynomial is p(x) = ";
	poly1.print();
	cout << endl;

	cout << endl << "Please input the coefficients of the second polynomial, q" << endl;
	getPoly(poly2);
	cout << endl << "Your second polynomial is q(x) = ";
	poly2.print();
	cout << endl;

	cout << endl << "p(x)+q(x) = ";
	Polynomial polyAdd = poly1 + poly2;
	polyAdd.print();
	cout << endl;

	cout << endl << "p(x)-q(x) = ";
	Polynomial polySub = poly1 - poly2;
	polySub.print();
	cout << endl;

	cout << endl << "p(x)*q(x) = ";
	(poly1*poly2).print();
	cout << endl;

	cout << endl << "p(x)/q(x) = ";
	Rational(poly1, poly2).print();
	cout << endl;

	cout << endl << "p(x)/q(x)+p(x)*q(x) = ";
	(Rational(poly1, poly2) + Rational(poly1 * poly2)).print();
	cout << endl;

	cout << endl << "p(x)+1 = ";
	Polynomial p1 = poly1;
	++p1;
	p1.print();
	cout << endl;

	cout << endl << "p(x)+2 = ";
	++p1;
	p1.print();
	cout << endl;

	cout << endl << "(p(x)/q(x)) * (1+x^2-3x^4) = ";
	vector<int> v;
	v.push_back(1) ; v.push_back(0); v.push_back(1) ; v.push_back(0); v.push_back(-3);
	(Rational(poly1 * Polynomial(v), poly2)).print();
	cout << endl;

	cout << endl << "Does p(x) equal q(x) ? ";
	if (poly1 == poly2)
		cout << "Yes." << endl;
	else
		cout << "No." << endl;

	cout << endl << "p(x) < q(x)?";
	if (poly1 < poly2)
		cout << " Yes." << endl;
	else
		cout << " No." << endl;
	
	cout << endl;

	//cout << poly1.evaluateAt(5) << endl;
	//cout << r.evaluateAt(5);

	system("pause");
	return 0;
}
/** Polynomial default constructor that sets initial coefficient value to 0.
*/
Polynomial::Polynomial()
{
	coefficient.push_back(0);
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
/** This function evalutes the polynomial at a given x value.
	
	@param x represents the value being used
	@return result is the double value of the polynomial evaluated at a given double
*/
double Polynomial::evaluateAt(double x) const
{
	double result = 0;
	for (int i = 0; i < coefficient.size(); i++)
	{
		double x_to_i = 1;
		for (int j = 0; j < i; j++)
			x_to_i *= x;
	result += coefficient[i]* x_to_i;
	}
	return result; 
}

/** This function is the prefix version of the increment operator

	@return *this is the object that called this function
*/
Polynomial& Polynomial::operator++()
{
	coefficient[0]++;
	return *this;
}

/** This function is the postfix version of the increment operator

	@param int unused is used to just differentiate the definition of prefix and postfix
	@return Polynomial(v) is a copy of the object calling the function, since this function is returning
	the original copy and then incrementing.
*/
Polynomial Polynomial::operator++(int unused)
{
	vector<int> clone(coefficient);//copy w/ original value of object calling function
	coefficient[0]++;
	return Polynomial(clone);
}

/** This function is the prefix version of the decrement operator

	@return *this is the object calling this function.
*/
Polynomial& Polynomial::operator--()//more efficient than postfix since not making a copy
{
	coefficient[0]--;
	return *this;
}

/** This function is the postfix version of the decrement operator

	@param int unused is used to just differentiate the definition of prefix and postfix
	@return Polynomial(v) is a copy of the object calling the function, since this function is returning
	the original copy and then decrementing.
*/
Polynomial Polynomial::operator--(int unused)
{
	vector<int> clone(coefficient);
	coefficient[0]--;
	return Polynomial(clone);
}

/** This function adds another polynomial to the Polynomial calling the function

	@param Polynomial& p is the polynomial added to the object calling the function
	@return *this is the newly updated object that called the function
*/
Polynomial& Polynomial::operator+=(const Polynomial& p)
{
	(*this) = (*this) + p;
	return *this;
}

/** This function subtracts another polynomial to the Polynomial calling the function

	@param Polynomial& p is the polynomial subtracted to the object calling the function
	@return *this is the newly updated object that called the function
*/
Polynomial& Polynomial::operator-=(const Polynomial& p)
{
	(*this) = (*this) - p;
	return *this;
}

/** This function multiplies another polynomial to the Polynomial calling the function

	@param Polynomial& p is the polynomial multiplied to the object calling the function
	@return *this is the newly updated object that called the function
*/
Polynomial& Polynomial::operator*=(const Polynomial& p)
{
	(*this) = (*this) * p;
	return *this;
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

/** Overloading the + operator allows adding two polynomials
	
	@param p1 is the first polynomial being used
	@param p2 is the second polynomial being used
	@return newCoeff is the result of the two
*/
Polynomial operator+(const Polynomial& p1, const Polynomial& p2)
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

/** Overloading the - operator allows subtracting two polynomials
	
	@param p1 is the first polynomial being used
	@param p2 is the second polynomial being used
	@return newCoeff is the result of the two
*/
Polynomial operator-(const Polynomial& p1, const Polynomial& p2)
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

/** Overloading the * operator allows multiplying two polynomials
	
	@param p1 is the first polynomial being used
	@param p2 is the second polynomial being used
	@return newCoeff is the result of the two
*/
Polynomial operator*(const Polynomial& p1, const Polynomial& p2)
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

/** This function checks if two polynomials are equal.

	@param p1 being tested against p2
	@param p2 being tested against p1
	@return true if the two polynomials are equal 
	@return false if not equal
*/
bool operator==(const Polynomial& p1, const Polynomial& p2)
{
	if (p1.Degree() != p2.Degree())
		return false;
	for (int i = 0; i <= p1.Degree(); i++)
	{
		if (p1.Coefficient(i) != p2.Coefficient(i))
			return false;
	}
	return true;
}

/** This function checks if two polynomials are unequal.

	@param p1 being tested against p2
	@param p2 being tested against p1
	@return true if the two polynomials are unequal
	@return false if equal
*/
bool operator!=(const Polynomial& p1, const Polynomial& p2)
{
	if (p1.Degree() != p2.Degree())
		return true;
	for (int i = 0; i <= p1.Degree(); i++)
	{
		if (p1.Coefficient(i) != p2.Coefficient(i))
			return true;
	}
	return false;
}

/** This function checks if p1 is less than p2.

	@param p1 being tested against p2
	@param p2 being tested against p1
	@return true if p1 < p2
	@return false if p1 is not less than p2
*/
bool operator<(const Polynomial& p1, const Polynomial& p2)
{
	if (p1.Degree() < p2.Degree())
		return true;
	return false;
}

/** This function checks if p1 is greater than p2.

	@param p1 is being tested against p2
	@param p2 is tested against p1
	@return true if p1 > p2
	@return false if p1 is not greater than p2
*/
bool operator>(const Polynomial& p1, const Polynomial& p2)
{
	if (p1.Degree() > p2.Degree())
		return true;
	return false;
}

/** Default constructor for Rational class that has the denominator set to 1
*/
Rational::Rational()
{
	vector<int> v;
	v.push_back(1);
	denominator = Polynomial(v);
}

/** Constructor that sets a polynomial to the numerator and the denominator to 1.

	@param Polynomial p is the polynomial being set to the numerator.
*/
Rational::Rational(Polynomial p)
{
	numerator = p;
	vector<int> v;
	v.push_back(1);
	denominator = Polynomial(v);
}

/** Constructor that sets two polynomials to the numerator and the denominator.

	@param pN is the polynomial being set to the numerator.
	@param pD is the polynomial being set to the denominator.
*/
Rational::Rational(Polynomial pN, Polynomial pD)
{
	numerator = pN;
	denominator = pD;
}

/** This function evalutes the rational at a given x value.
	
	@param x represents the value being used
	@return result is the double value of the rational evaluated at a given double
*/
double Rational::evaluateAt(double x) const
{
	return numerator.evaluateAt(x) / denominator.evaluateAt(x);
}

/** This function prints the Rational, which comprises of poly1 and poly2.
*/
void Rational::print()
{
	numerator.print();//numerator is a polynomial, so it is calling print() from polynomial class
	cout << " / ";
	denominator.print();
}

/** This function accesses the private variable numerator for nonmember functions
	
	@return numerator for nonmember functions
*/
Polynomial Rational::getNumerator() const
{
	return numerator;
}

/** This function accesses the private variable denominator for nonmember functions

	@return denominator for nonmember functions
*/
Polynomial Rational::getDenominator() const
{
	return denominator;
}

/** This function is the prefix version of the increment operator

	@return *this is the object calling the function.
*/
Rational& Rational::operator++()
{
	numerator += denominator;
	return *this;
}

/** This function is the postfix version of the increment operator

	@param int unused is used to just differentiate the definition of prefix and postfix
	@return r is a copy of the object calling the function, since this function is returning
	the original copy and then incrementing.
*/
Rational Rational::operator++(int unused)
{
	Rational clone(numerator, denominator);
	numerator += denominator;
	return clone;
}

/** This function is the prefix version of the decrement operator

	@return *this is the object calling the function.
*/
Rational& Rational::operator--()
{
	numerator -= denominator;
	return *this;
}

/** This function is the postfix version of the decrement operator

	@param int unused is used to just differentiate the definition of prefix and postfix
	@returnr  is a copy of the object calling the function, since this function is returning
	the original copy and then decrementing.
*/
Rational Rational::operator--(int unused)
{
	Rational clone(numerator, denominator);
	numerator -= denominator;
	return clone;
}

/** This function adds another rational to the rational calling the function

	@param Polynomial& p is the polynomial added to the object calling the function
	@return *this is the newly updated object that called the function
*/
Rational& Rational::operator+=(const Rational& r)
{
	numerator = numerator * r.denominator + denominator * r.numerator;
	denominator *= r.denominator;
	return *this;
}

/** This function subtracts another rational to the rational calling the function

	@param Polynomial& p is the polynomial subtracted to the object calling the function
	@return *this is the newly updated object that called the function
*/
Rational& Rational::operator-=(const Rational& r)
{
	numerator = numerator * r.denominator - denominator * r.numerator;
	denominator *= r.denominator;
	return *this;
}

/** This function multiplies another rational to the rational calling the function

	@param Polynomial& p is the polynomial multiplied to the object calling the function
	@return *this is the newly updated object that called the function
*/
Rational& Rational::operator*=(const Rational& r)
{
	numerator *= r.numerator;
	denominator *= r.denominator;
	return *this;
}

/** This function divides the object calling the function by another rational

	@param Polynomial& p is the polynomial divided to the object calling the function
	@return *this is the newly updated object that called the function
*/
Rational& Rational::operator/=(const Rational& r)
{
	numerator *= r.denominator;
	denominator *= r.numerator;
	return *this;
}

/** Overloading the + operator allows us to add two rationals together
	
	@param r1 is the first rational being used
	@param r2 is the second rational being used
	@return Rational is the sum of the two
*/
Rational operator+(const Rational& r1, const Rational& r2)
{
	return Rational(r1.getNumerator() * r2.getDenominator() + r1.getDenominator() * r2.getNumerator(), r1.getDenominator() * r2.getDenominator());
}

/** Overloading the - operator allows us to subtract one rational from another
	
	@param r1 is the first rational being used
	@param r2 is the second rational being used
	@return Rational is the result of the two
*/
Rational operator-(const Rational& r1, const Rational& r2)
{
	return Rational(r1.getNumerator() - r2.getDenominator() - r1.getDenominator() * r2.getNumerator(), r1.getDenominator() * r2.getDenominator());
}

/** Overloading the * operator allows the multiplication between two rationals
	
	@param r1 is the first rational being used
	@param r2 is the second rational being used
	@return Rational is the result of the two
*/
Rational operator*(const Rational& r1, const Rational& r2)
{
	return Rational(r1.getNumerator() * r2.getNumerator() , r1.getDenominator() * r2.getDenominator());
}

/** Overloading the / operator allows the division between two rationals
	
	@param r1 is the first rational being used
	@param r2 is the second rational being used
	@return Rational is the result of the two
*/
Rational operator/(const Rational& r1, const Rational& r2)
{
	return Rational(r1.getNumerator() * r2.getDenominator() , r1.getDenominator() * r2.getNumerator());
}