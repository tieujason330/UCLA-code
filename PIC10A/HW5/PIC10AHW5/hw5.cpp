/* Jason Tieu
	304047667
	This program requires me to create a cookie class called Cookie that 
	holds all the cookie fields listed. I have to create a full set of parameters
	and constructors. I will then display the cookie options.
*/


#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class Cookie //This begins my Cookie class.
{
public://In here, I will list the member functions I will be using.
	Cookie();// This is my default consturctor.

	Cookie(string cookieName, string cookieBase, int chipNum, string chipName , char cookieTopping, int cookieRating);//I am overloading the constructor here.

	string getName() const;
	void setName (string);

	string getBase() const;
	void setBase (string);
	
	string getChipName() const;
	int getChipQty() const;
	void setChipQty(string, int);
	
	char getTopping() const;
	void setTopping(char);
	
	int getratingLvl() const;
	void setratingLvl(int);
	
	void displayCookie();

	//void copy();

private://These are the data types I will be using.
	string name;
	string base;
	string chip;
	int chipQty;
	char topping;
	int ratingLvl;

};

int main() 
{
	cout << "Welcome to Blake's Cookie Mad House! Here are the wild options for you to choose from!" << endl << endl; 

	// create 4 cookies
	Cookie c1; // default constructor

	Cookie c2("Fudgy Pants", "Peanut Butter", 100, "Oreo bits", 'b', 6); // full constructor

	// change by hand
	Cookie c3;
	c3.getName();
	c3.setName("Dardevil");

	c3.getBase();
	c3.setBase("Pecan");

	c3.getChipName();
	c3.getChipQty();
	c3.setChipQty("Peppermints", 50);

	c3.getTopping();
	c3.setTopping('d');

	c3.getratingLvl();
	c3.setratingLvl(4);
	
	Cookie c4(c3); // copy an existing cookie

	// display the cookies
	c1.displayCookie();
	c2.displayCookie();
	c3.displayCookie();
	c4.displayCookie();

	return 0;
}

Cookie::Cookie()//this is my default constructor
{
	name = "White Fudge";
	base = "Cookie Dough";
	chipQty = 26;
	chip = "White chocolate";
	topping = 'a';
	ratingLvl = 6;
}

Cookie::Cookie(string cookieName, string cookieBase, int chipNum, string chipName, char cookieTopping, int cookieRating)
{
	name = cookieName;
	base = cookieBase;
	chipQty = chipNum;
	chip = chipName;
	topping = cookieTopping;
	ratingLvl = cookieRating;
}

//This function returns name of the cookie using a string type.
string Cookie::getName() const
{
	return name;
}
//This function assigns the cookieName to the name data that I created in the Cookie class.
void Cookie::setName (string cookieName)
{
	name = cookieName; //do NOT use string name = cookieName.
}

//This function returns the name of the base of my cookie using a string type.
string Cookie::getBase() const
{
	return base;
}
//This functions assigns the cookieBase to the base data that I created in the Cookie class.
void Cookie::setBase(string cookieBase)
{
	base = cookieBase; //do NOT use string base = cookieBase.
}

//This function returns the name of the chips I will be using.
string Cookie::getChipName() const
{
	return chip;
}

int Cookie::getChipQty() const
{
	return chipQty;
}
//This function assigns both a string and int, they are assigned from the chip and chipQty that I created.
void Cookie::setChipQty(string chipName, int chipNum)
{
	chip = chipName; //do NOT use string chip = chipName.
	chipQty = chipNum; //do NOT use int chipQty = chipNum.
}

//This function returns a topping using the char type.
char Cookie::getTopping() const
{
	return topping;
}
//This function assigns my topping to cookieTopping.
void Cookie::setTopping(char cookieTopping)
{
	topping = cookieTopping; //do NOT use char topping = cookieTopping.
}

//This function returns my rating using the int type.
int Cookie::getratingLvl() const
{
	return ratingLvl;
}
//This function assigns my rating to cookieRating for me to use in my Cookie function.
void Cookie::setratingLvl(int cookieRating)
{
	ratingLvl = cookieRating; //do NOT use int ratingLvl = cookieRating.
}


//In this function, I give the type of arguments as input. I then cout << them using the previous functions I have created.
void Cookie::displayCookie()
{
	cout <<  name << " - a " << base << " cookie loaded with " << chipQty << " "
		<< chip << endl << "\t" << " topped with yummy ";

	//This is used to determine my topping.
	topping;
	if (topping == 'a')
		cout << "apricots";
	else if (topping == 'b')
		cout << "bubblegum";
	else if (topping == 'c')
		cout << "cotton candy";
	else if (topping == 'd')
		cout << "dumplings";

	cout << ". " << "Rating level - " << ratingLvl << " out of 10 deliciousness." << endl << endl;	
}
