/* 
Jason Tieu
304047667
This project revolved around streams and their usage. By overloading the << and >> operators, I was able to print out
user created objects as if they were pre-existing objects. This allowed for simpler and easier-to-read code. Also,
the output is displayed in a .txt using the fstream library.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

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

private:
	vector<int> coefficient;
};

//Polynomial non-member functions
istream& operator>>(istream& in, Polynomial& p);
ostream& operator<<(ostream& out, const Polynomial& p);

Polynomial operator+(const Polynomial& p1,const Polynomial& p2);
Polynomial operator-(const Polynomial& p1, const Polynomial& p2);
Polynomial operator*(const Polynomial& p1, const Polynomial& p2);

class Rational
{
public:
	Rational();//default constructor
	Rational(Polynomial p);//constructor that sets numerator = p, denominator = 1
	Rational(Polynomial pN, Polynomial pD);//constuctor w/ parameters for numerator and denominator
	//for nonmember functions to access
	Polynomial getNumerator() const;
	Polynomial getDenominator() const;
	

private:
	Polynomial numerator, denominator;
};

//Rational nonmember functions
istream& operator>>(istream& in, Rational& r);
ostream& operator<<(ostream& out, const Rational& r);

Rational operator+(const Rational& r1, const Rational& r2);
Rational operator-(const Rational& r1, const Rational& r2);
Rational operator*(const Rational& r1, const Rational& r2);
Rational operator/(const Rational& r1, const Rational& r2);

int main()
{
	Polynomial p, q;
	Rational r;
	ofstream fout;
	cout << "Welcome! Please input the coefficients of the first polynomial, p." << endl;
	cout << "When you are finished, enter -123456789." << endl;
	cin >> p;
	cout << endl << "Your first polyomial is p(x) = " << p << "." << endl;

	cout << endl << "Please input the coefficients of the second polynomial, q." << endl;
	cin >> q;
	cout << endl << "Your second polynomial is q(x) = " << q << "." << endl;

	cout << endl << "Please enter coefficients for the numerator and denominator, separated by -123456789." << endl;
	cin >> r;
	cout << "Your rational function is r(x) = " << r << "." << endl;

	cout << "Creating file ... ";
	string name = "JasonTieuhw4Output.txt";
	fout.open(name);
	
	fout << "p(x) = " << p << endl;
	fout << "q(x) = " << q << endl;
	fout << "r(x) = " << r << endl;

	fout << endl << "p(x)+q(x) = " << p+q << endl;
	fout << "p(x)-q(x) = " << p-q << endl;
	fout << "p(x)*q(x) = " << p*q << endl;
	fout << "p(x)/q(x) = " << p/q << endl;

	fout << endl << "r(x)*p(x) = " << r*Rational(p) << endl;
	fout << "r(x)/q(x) = " << r/Rational(q) << endl;
	fout << "r(x)*p(x)/q(x) = " << r*Rational(p,q);

	fout.close();
	cout << "Done." << endl;
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

/** This function retrieves the input from the user and stores in the vector v using the >> operator (for Polynomials) Note: had to use in instead of cin

	@param in is the input stream object
	@param p, passing by reference the Polynomial poly in order to directly access and manipulate it
	@return in is the input stream object
*/
istream& operator>>(istream& in, Polynomial& p)
{
	int input;
	vector<int> v;

	while(in >> input)
	{
		v.push_back(input);

		if (input == -123456789)
		{
			v.pop_back();
			break;
		}
	}
	p = Polynomial(v);
	return in;
}

/** This function displayed the polynomial using << via operator overloadng. Note: had to use out instead of cout

	@param out is the ouput stream object
	@param p is the polynomial object being displayed
	@return out is the output stream object
*/
ostream& operator<<(ostream& out, const Polynomial& p)
{
	bool firstTerm = 1;

	for (int i = 0; i < p.Degree() + 1; i++)//Degree() + 1 is the size
	{
		if (p.Coefficient(i) != 0)
		{
			if (p.Coefficient(i) == 0)
			{
				cout << "";
			}
			if (p.Coefficient(i) == 1)
			{
				if (i == 0)
					out << p.Coefficient(i);
				if (i == 1)
				{
					if (p.Coefficient(i-1) == 0)
						out << "x";
					else
						out << "+x";
				}
				if (i > 1)
				{
					if (firstTerm == 1)
						out << "x^" << i;
					else
						out << "+x^" << i;
				}
			}
			if (p.Coefficient(i) == -1)
			{
				if (i == 0)
					out << p.Coefficient(i);
				if (i == 1)
					out << "-x";
				else if (i != 1 && i != 0)
					out << "-x^" << i;
			}
			else if (p.Coefficient(i) > 1)
			{
				if (firstTerm == 1 && i == 1)
					out << p.Coefficient(i) << "x";
				else if (firstTerm == 0 && i == 1)
					out << "+" << p.Coefficient(i) << "x";

				else if (firstTerm == 1 && i > 1)
					out << p.Coefficient(i) << "x^" << i;
				else if (firstTerm == 0 && i > 1)
					out << "+" << p.Coefficient(i) << "x^" << i;

				else if (i == 0)
					out << p.Coefficient(i);
			}
			if (p.Coefficient(i) < -1)
			{
				if (i == 0)
					out << p.Coefficient(i);
				if (i == 1)
					out << p.Coefficient(i) << "x";
				if (i > 1)
					out << p.Coefficient(i) << "x^" << i;
			}
			firstTerm = 0;
		}
	}
	return out;
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

/** This function inputs the numerator and denominator of a Rational using the >> operators previously defined for Polynomials

	@param in is the input stream object
	@param r is the Rational object
	@return in is the input stream object
*/
istream& operator>>(istream& in, Rational& r)
{
	Polynomial num, denom;
	in >> num >> denom;
	r = Rational(num,denom);
	return in;
}

/** This function displays the Rational object by overloading the << operators

	@param out is the output stream object
	@param r is the Rational object being displayed
	@return out is the output stream object
*/
ostream& operator<<(ostream& out, const Rational& r)
{
	out << r.getNumerator() << " / " << r.getDenominator();
	return out;
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