//Base Class - Sport
class Sport
{
public:
	Sport(string name); //constructor that takes in a string 
	virtual ~Sport() {} //virtual destructor for Sport since it is base class
	string name() const;//prints out m_name

	//pure virtual functions
	//uses the derived functions instead since Sport is an abstract class
	virtual bool isOutdoor() const = 0;
	virtual string icon() const = 0;
private:
	string m_name;
};

//Derived Classes from Sport
class Snowboarding : public Sport
{
public:
	Snowboarding(string activity);//Constructor that takes in a string
	virtual ~Snowboarding();//Destructor for Snowboarding
	virtual bool isOutdoor() const;//determines if Snowboarding is outdoors
	virtual string icon() const;//displays the icon for Snowboarding
private:
	string m_activity;
};
//----------------------------------
class Biathlon : public Sport
{
public:
	Biathlon(string activity, double km);//Constructor that takes in a string and double
	virtual ~Biathlon();//Destructor for Biathlon
	virtual bool isOutdoor() const;//determines if Biathlon is outdoors
	virtual string icon() const;//displays the icon for Biathlon
private:
	string m_activity;
	double m_km;
};
//---------------------------------
class FigureSkating : public Sport
{
public:
	FigureSkating(string activity);//Constructor that takes in a string
	virtual ~FigureSkating();//Destructor for FigureSkating
	virtual bool isOutdoor() const;//determines if FigureSkating is outdoors
	virtual string icon() const;//displays the icon for FigureSkating
private:
	string m_activity;
};

//-----Sport Implementation-----

//Constructor for Sport
//Takes in a string and assigns m_name to it
Sport::Sport(string name)
: m_name(name)
{}

//function that returns a string
//returns m_name
string Sport::name() const
{
	return m_name;
}
//------------------------------

//---Snowboarding Implementation--

//Constructor for Snowboarding
//Takes in a string and passes that string to the Sport constructor
//the string 'activity' is then assigned to m_name for the Sport class
Snowboarding::Snowboarding(string activity)
:Sport(activity)
{}

//Sport destructor
//couts the statement to let user know that the destructor was called
Snowboarding::~Snowboarding()
{
	cout << "Destroying the Snowboarding object named " << name() << "." << endl;
}

//Function that returns a bool
//returns whether or not Snowboarding is T/F (true)
bool Snowboarding::isOutdoor() const
{
	return true;
}

//Function that returns a string
//returns a string describing the icon for Snowboarding
string Snowboarding::icon() const
{
	return "a descending snowboarder";
}
//------------------------------

//---Biathlon Implementation---

//Constructor for Biathlon, takes a string and double
//string is passed to the Sport constructor, assigning m_name to activity
//the member variable m_km is assigned to the double
Biathlon::Biathlon(string activity, double km)
:Sport(activity), m_km(km)
{}

//Destructor for Biathlon
//couts a statement to let user know that the destructor was called
Biathlon::~Biathlon()
{
	cout << "Destroying the Biathlon object named " << name() << ", distance " << m_km << " km." << endl;
}

//Function that returns a bool
//returns whether or not Biathlon is T/F (true)
bool Biathlon::isOutdoor() const
{
	return true;
}

//Function that returns a string
//returns a string describing the icon for Biathlon
string Biathlon::icon() const
{
	return "skier with a rifle";
}
//------------------------------

//---FigureSkating Implementation----

//Constructor for FigureSkating, takes a string 
//string is passed to the Sport constructor, assigning m_name to activity
FigureSkating::FigureSkating(string activity)
:Sport(activity)
{}

//Destructor for FigureSkating
//couts a statement to let user know that the destructor was called
FigureSkating::~FigureSkating()
{
	cout << "Destroying the FigureSkating object named " << name() << "." << endl;
}

//Function that returns a bool
//returns whether or not FigureSkating is T/F (false)
bool FigureSkating::isOutdoor() const
{
	return false;
}

//Function that returns a string
//returns a string describing the icon for FigureSkating
string FigureSkating::icon() const
{
	return "skater in the Biellmann position";
}
//-----------------------------------