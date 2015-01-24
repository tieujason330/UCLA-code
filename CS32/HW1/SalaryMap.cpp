#include"SalaryMap.h"
#include<iostream>
using namespace std;

SalaryMap::SalaryMap()
// Create an empty salary map
{}

bool SalaryMap::add(std::string name, double salary)
// If an employee with the given name is not currently in the map, 
// there is room in the map, and the salary is not negative, add an
// entry for that employee and salary and return true.  Otherwise
// make no change to the map and return false.
{
	if (salary >= 0)
	{
		m_map.insert(name, salary);
		return true;
	}
	else
		return false;
}

bool SalaryMap::raise(std::string name, double pct)
// If no employee with the given name is in the map or if pct is less
// than -100, make no change to the map and return false.  Otherwise,
// change the salary of the indicated employee by the given
// percentage and return true.  For example, if pct is 10, the
// employee gets a 10% raise; if it's -5, the salary is reduced by 5%.
{
	if (pct < -100 || !m_map.contains(name))
	{
		return false;
	}
	else
	{
		double newSalary = (salary(name)*(pct / 100)) + salary(name);
		m_map.update(name, newSalary);
		return true;
	}
}

double SalaryMap::salary(std::string name) const
// If an employee with the given name is in the map, return that
// employee's salary; otherwise, return -1.
{
	if (m_map.contains(name))
	{
		double sal;
		m_map.get(name, sal);
		return sal;
	}
	else
		return -1;
}

int SalaryMap::size() const
// Return the number of employees in the SalaryMap.
{
	return m_map.size();
}

void SalaryMap::print() const
// Write to cout one line for every employee in the map.  Each line
// has the employee's name, followed by one space, followed by that
// employee's salary.
{
	string name;
	double salary;

	for (int i = 0; i < size(); i++)
	{
		m_map.get(i, name, salary);
		cout << name << " " << salary << endl;
	}
}