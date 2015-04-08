/*
Jason Tieu
304047667
5-7-13
For this assignment, we had to use our code from hw3.cpp and modify it. We were sort the Point2D objects and it's derived classes
in the order of:
Point2D < ColorPoint2D < WeightedPoint2D.
We were required to overload the < operator to compare the objects' length squared, angles, color (in terms of alphabetical order), and weight.
We used the algorithm library to sort of objects for us. The cmath library was used for atan2 and atan (arc tan) to find the angle in radians.
*/


#include<iostream>
#include<vector>
#include<string>
#include<algorithm> //this library was required for sorting the vectors in order
#include<cmath> //this library was required for the usage of atan and atan2

using namespace std;

class Point2D {
public:
	// Constructors
	Point2D();
	Point2D(double a, double b);

	// virtual Print functions for polymorphism
	virtual void print();
	virtual void print(int a);

	//member functions
	double length_squared() const;
	double angle() const;

private:
	double x;
	double y;
};

class ColorPoint2D : public Point2D {
public:
	// Constructors
	ColorPoint2D();
	ColorPoint2D(double a, double b, string col);

	// virtual print for polymorphism
	virtual void print();

	//get-r functions
	string getColor();

private:
	string color;
};

class WeightedPoint2D : public Point2D {
public:
	// constructors
	WeightedPoint2D();
	WeightedPoint2D(double a, double b, double wt);

	// print function
	virtual void print();

	//get-r functions
	double getWeight();

private:
	double weight;
};

//nonmember operator overloading functions
bool operator<(Point2D p1, Point2D p2);
bool operator<(ColorPoint2D p1, ColorPoint2D p2);
bool operator<(WeightedPoint2D p1, WeightedPoint2D p2);

int main() 
{
	//create 3 different vectors to store Point2D, ColorPoint2D, and WeightedPoint2D
	vector<Point2D> u;
	vector<ColorPoint2D> v;
	vector<WeightedPoint2D> w;

	//Displaying menu
	cout << "Welcome to Point Printer! You can create three different kinds of points: " << endl << endl;
	cout << "1. Point2D, e.g., (2,6.5)" << endl;
	cout << "2. ColorPoint2D, e.g., blue(-4.5,3.5)" << endl;
	cout << "3. WeightedPoint2D, e.g., .12(3.6,8.7)" << endl;
	cout << endl << "Enter 0 when you are finished." << endl << endl;
	//using a while loop to allow users to choose selection until they are finished
	while(1)
	{
		int selection;
		cout << "Selection: ";
		cin >> selection;
		if(selection != 0)
		{
			double x, y;
			cout << "x = ";
			cin >> x;
			cout << "y = ";
			cin >> y;

			if (selection == 1)
			{
				//stored in Point2D vector
				u.push_back(Point2D(x,y));
			}
			else if (selection == 2)
			{
				string color;
				//using cin.ignore() since used a cin before to get x and y, need to skip the return key
				cin.ignore();
				cout << "color: ";
				getline(cin, color);
				//stored in ColorPoint2D vector
				v.push_back(ColorPoint2D(x, y, color));
			}
			else if (selection == 3)
			{
				double weight;
				cout << "weight: ";
				cin >> weight;
				//stored in WeightedPoint2D vector
				w.push_back(WeightedPoint2D(x, y, weight));
			}
		}
		//when selection is 0, break out of while loop and goes to printing
		else
			break;
	}

	//this uses the algorithm library to sort each type of vector in order 
	//from least to greatest through operator overloading of <
	sort(u.begin(),u.end());
	sort(v.begin(),v.end());
	sort(w.begin(),w.end());

	cout << endl << "Your points are" << endl << endl;

	//prints out Point2D < ColorPoint2D < WeightedPoint2D
	for(int i = 0; i < u.size(); i++)
	{
		cout << i+1 << ": ";
		u[i].print();
		cout << endl;
	}
	for(int i = 0; i < v.size(); i++)
	{
		cout << i + 1 + u.size() << ": ";
		v[i].print();
		cout << endl;
	}
	for(int i = 0; i < w.size(); i++)
	{
		cout << i + 1 + u.size() + v.size() << ": ";
		w[i].print();
		cout << endl;
	}

	cout << endl;
	system("pause");
	return 0;
}

/** Default constructor for Point2D
*/
Point2D::Point2D() 
{ 
	x = 0; 
	y = 0; 
	return;
}

/** Point2D constructor

	@param a sets a to x
	@param b sets b to y
*/
Point2D::Point2D(double a, double b) 
{
	x = a; 
	y = b; 
	return;
}

/** Point2D print function prints out (x,y)
*/
void Point2D::print() 
{
	cout<<"("<<x<<","<<y<<")";
	return;
}

/** This print function adds a cout << endl; for convenience
	
	@param a is any int to automatically do cout << endl;
*/
void Point2D::print(int a) 
{
	print(); 
	cout<<endl;
	return;
}

/** This function returns the length squared of the Point2D object
	
	@return x*x+y*y is the length squared formula
*/
double Point2D::length_squared() const
{
	return x*x+y*y;
}

/** This function returns the angle of the Point2D object using atan2 through cmath library
	
	@return the angle in radians
*/
double Point2D::angle() const
{
	double a = atan2(y,x);

	if (a < 0) 
	{ 
		return a + atan(1.0)*8 ;
	} 
	return a;
}
/** Default constructor for ColorPoint2D
*/
ColorPoint2D::ColorPoint2D() 
{ 
	color = ""; 
	return ;
}

/** Constructor ColorPoint2D with parameters, invokes Point2D(a,b) constructor

@param a sets a to x
@param b sets b to y
@param col sets col to color
*/
ColorPoint2D::ColorPoint2D(double a, double b, string col)
	: Point2D(a,b)
{
	color = col;
	return;
}

/** ColorPoint2D print function that calls the Point2D print function
*/
void ColorPoint2D::print() {
	cout<<color;
	Point2D::print();
	return;
}

/** This function returns the color of the ColorPoint2D object
	
	@return color is the color of the object
*/
string ColorPoint2D::getColor()
{
	return color;
}
/** WeightedPoint2D constructor
	
	@param a sets a to x
	@param b sets b to y
	@param wt sets wt to weight
*/
WeightedPoint2D::WeightedPoint2D(double a, double b, double wt) 
	: Point2D(a,b) 
{
	weight = wt;
}

/** WeightedPoint2D print function that calls Point2D print function
*/
void WeightedPoint2D::print() 
{
	cout << weight;
	Point2D::print();
	return;
}

/** This function gets the weight of the WeightedPoint2D object
	
	@return weight is the weight of the object
*/
double WeightedPoint2D::getWeight()
{
	return weight;
}

/** This function overloads the < to compare two Point2D objects

	@param p1 is the first Point2D object being compared
	@param p2 is the second Point2D object being compared
	@return compares the length_squared/angle of both objects
*/
bool operator<(Point2D p1, Point2D p2)
{
	//if the length of both Point2D objects are not equal
	if (p1.length_squared() != p2.length_squared())
		return p1.length_squared() < p2.length_squared();
	//if the length are equal, but angles are not
	if (p1.angle() != p2.angle())
		return p1.angle() < p2.angle();
	//if both objects are equal to each other
	return true;
}

/** This function overloads the < to compare two ColorPoint2D objects
	
	@param p1 is the first ColorPoint2D object compared
	@param p2 is the second ColorPoint2D object compared
	@return compares the length_squared/angle/color of both objects
*/
bool operator<(ColorPoint2D p1, ColorPoint2D p2)
{
	//if the length of both ColorPoint2D objects are equal
	if (p1.length_squared() != p2.length_squared())
		return p1.length_squared() < p2.length_squared();
	//if the length are equal, but angles are not equal
	if (p1.angle() != p2.angle())
		return p1.angle() < p2.angle();
	//if both length and angle are equal, but color is not
	if (p1.getColor() != p2.getColor())
		return p1.getColor() < p2.getColor();
	//if both objects are equal 
	return true;
}

/** This function overloads the < to compare two WeightedPoint2D objects
	
	@param p1 is the first WeightedPoint2D object compared
	@param p2 is the second WeightedPoint2D object compared
	@return compares the length_squared/angle/weight of both objects
*/
bool operator<(WeightedPoint2D p1, WeightedPoint2D p2)
{
	//if the length of both WeightedPoint2D objects are not equal
	if (p1.length_squared() != p2.length_squared())
		return p1.length_squared() < p2.length_squared();
	//if the length are equal, but angles are not
	if (p1.angle() != p2.angle())
		return p1.angle() < p2.angle();
	//if both length and angle are equal, but weight is not
	if (p1.getWeight() != p2.getWeight())
		return p1.getWeight() < p2.getWeight();
	//if both objects are equal
	return true;
}