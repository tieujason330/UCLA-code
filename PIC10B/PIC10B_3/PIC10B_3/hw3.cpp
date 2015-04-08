/*
Jason Tieu
304047667
This project primarily focuses on inheritance and polymorphism.
We have a base class, Point2D, and two derived classes from it: ColorPoint2D and WeightedPoint2D. We utilize 
memory management via new keyword and polymorphism via virtual keyword.
*/
#include<iostream>
#include<vector>
#include<string>

using namespace std;

class Point2D {
public:
	// Constructors
	Point2D();
	Point2D(double a, double b);

	// virtual Print functions for polymorphism
	virtual void print();
	virtual void print(int a);

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

private:
	double weight;
};


int main() 
{
	//able to create a vector of Point2D pointer since Point2D is the base class
	vector<Point2D*> v;
	Point2D *p;

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
				//creates new Point2D object in heap, p points to this object
				p = new Point2D(x,y);
				//object is stored in the vector
				v.push_back(p);
			}
			else if (selection == 2)
			{
				string color;
				//using cin.ignore() since used a cin before to get x and y, need to skip the return key
				cin.ignore();
				cout << "color: ";
				getline(cin, color);
				//creates new ColorPoint2D object in heap
				p = new ColorPoint2D(x, y, color);
				//stored into vector
				v.push_back(p);
			}
			else if (selection == 3)
			{
				double weight;
				cout << "weight: ";
				cin >> weight;
				//creating new WeightedPoint2D object in heap
				p = new WeightedPoint2D(x, y, weight);
				//stored in vector
				v.push_back(p);
			}
		}
		//when selection is 0, break out of while loop and goes to printing
		else
			break;
	}
	cout << endl << "Your points are" << endl << endl;

	for(int i = 0; i < v.size(); i++)
	{
		cout << i+1 << ": ";
		//dereferncing the pointer using ->
		v[i]->print(1);
		//same as (*v[i]).print(1);
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
