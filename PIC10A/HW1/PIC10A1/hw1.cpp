/* Jason Tieu
ID 304047667 
Date - 4/12/12
My goal is to make a program that breaks a given number of cents down into units of currency
	- dollars, quarters, dimes, nickels, and pennies. 
*/

#include <iostream>
using namespace std;

int main () { 

	int total_cents, dollars, difference, quarters, dimes, nickels, pennies;//Initialize variables
	cout<< "Enter total cents: "; //Prompts user to input cents
	cin>> total_cents; //User inputs cents

	const int CENTS_PER_DOLLARS = 100; //Constant for how many cents in a dollar
	const int CENTS_PER_QUARTERS = 25; //Constant for how many cents in a quarter
	const int CENTS_PER_DIMES = 10; //Constant for how many cents in a dime
	const int CENTS_PER_NICKELS = 5; //Constant for how many cents in a nickel
	const int CENTS_PER_PENNIES = 1; //Constant for how many cents in a penny

	dollars = total_cents / CENTS_PER_DOLLARS; //Take total and divide it by 100 (cents per dollar)

	difference = total_cents - (dollars * CENTS_PER_DOLLARS); //Take difference of total cents and dollars
	quarters = difference / CENTS_PER_QUARTERS; // Assign quarters as difference divided by 25 (cents per quarter)

	difference = difference - (quarters * CENTS_PER_QUARTERS); //Take initial difference and subtract quarters.
	dimes = difference / CENTS_PER_DIMES; //Assign dimes as new difference divided by 10 (cents per dime)

	difference = difference - (dimes * CENTS_PER_DIMES); //Take difference = difference - (quarters * CENTS_PER_QUARTERS) and subtract dimes.
	nickels = difference / CENTS_PER_NICKELS; //Assign nickels as new difference divided by 5 (cents per nickel)

	difference = difference - (nickels * CENTS_PER_NICKELS); //Take difference = difference - (dimes * CENTS_PER_DIMES) and subtract nickels.
	pennies = difference / CENTS_PER_PENNIES; //Assign pennies as new difference divided by 1 (cents per penny)


	cout<< "This corresponds to " << dollars << " dollars, " << quarters << " quarters, " << dimes << " dimes, " 
		<< nickels << " nickels, and " << pennies << " pennies.\n"; //This outputs cents in dollars, quarters, dimes, nickels, and pennies

	return 0;
}