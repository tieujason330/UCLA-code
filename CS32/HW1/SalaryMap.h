#include "newMap.h"

class SalaryMap
{
public:
	SalaryMap();       // Create an empty salary map.

	bool add(std::string name, double salary);
	// If an employee with the given name is not currently in the map, 
	// there is room in the map, and the salary is not negative, add an
	// entry for that employee and salary and return true.  Otherwise
	// make no change to the map and return false.

	bool raise(std::string name, double pct);
	// If no employee with the given name is in the map or if pct is less
	// than -100, make no change to the map and return false.  Otherwise,
	// change the salary of the indicated employee by the given
	// percentage and return true.  For example, if pct is 10, the
	// employee gets a 10% raise; if it's -5, the salary is reduced by 5%.

	double salary(std::string name) const;
	// If an employee with the given name is in the map, return that
	// employee's salary; otherwise, return -1.

	int size() const;  // Return the number of employees in the SalaryMap.

	void print() const;
	// Write to cout one line for every employee in the map.  Each line
	// has the employee's name, followed by one space, followed by that
	// employee's salary.

private:
	// Some of your code goes here.
	Map m_map;
};