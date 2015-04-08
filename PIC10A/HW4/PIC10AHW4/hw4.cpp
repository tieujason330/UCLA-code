/* Jason Tieu
	304047667
	This program keeps track of the number of ants in two adjacent houses for a number of weeks. 
	Every second week of the month, I find out the total number of ants in each house,
	the user selects which house to call the exterminator to, who will not come until the fourth week, 
	or the user selects an option to give up. Every fourth week, one of the houses is visited by an exterminator, 
	resulting in a 90% reduction (rounded down) in the number of ants in that house. 
*/

#include <iostream>
#include <time.h>
#include <string>
using namespace std;

//declaring the prototypes I will be using in this function
int init_num_ants(int);
void showWeek (int, int, int);
void space();
int house_Number (int);
int extermination (int, int);


int main()
{
	srand (time(0));//using this for randomness
	int house1 = 0; //starting house1 at 0 so when I use random it will be within 10 - 100
	int house2 = 0; //starting house2 at 0 so when I use random it will be within 10 - 100
	string answer = "N";//answer is "N" so it can begin the do while loop
	int selection = 0;//selection is 0, waiting for user to replace it with 1 or 2
	int month = 0;//month begins at 0 so first set of weeks are 1 - 4
	int week = 1;
	do
	{	
			for(; week < month*4 + 5 ; week++)//using a for loop for incrementing the weeks
			{
				if (week == 1)//at week 1, the number of ants are created to be used until user quits.
				{
					house1 = init_num_ants(house1);
					house2 = init_num_ants(house2);
				}

				if (week == month*4 + 1)//this is the first week of the month
				{
					showWeek (week,house1,house2);//this calls the showWeek function
					house1 = house_Number(house1);//house1 is assigned to the house_Number function to get house1*1.3
					house2 = house_Number(house2);//house2 is assigned to the house_Number function to get house2*1.3
					space();//this calls the space function
				}

				if (week == month*4 + 2)//this is the second week of the month
				{
					showWeek (week,house1,house2);
					house1 = house_Number(house1);
					house2 = house_Number(house2);
					cout << "Select house for extermination: 1 or 2? ";//this asks which house to exterminate.
					cin >> selection;
					space();
				}

				if (week == month*4 + 3)//this is the third week of the month
				{
					//using if statements to decide which house has more ants.
					//the one with more ants would then be decreased by 30% of itself and transferred to house with less ants.
					if (house1 > house2)
					{
						house1 = house1 - (house1 * .3);
						house2 = house2 + (house1* .3);
						showWeek (week,house1,house2);
						house1 = house_Number(house1);
						house2 = house_Number(house2);
						space();
					}
					else if (house1 < house2)
					{
						house2 = house2 - (house2 * .3);
						house1 = house1 + (house2 * .3);
						showWeek (week,house1,house2);
						house1 = house_Number(house1);
						house2 = house_Number(house2);
						space();
					}
				}

				if (week == month*4 + 4)//this is the fourth week of the month
				{ 
					//using if statements to act upon what user selected from week 2
					//house that is selected would then be decreased by 90%
					if (selection == 1)
					{
						house1 = house1 * 0.1;
						showWeek (week, house1, house2);
						house1 = house_Number(house1);
						house2 = house_Number(house2);
						space();
					}
					else if (selection == 2)
					{
						house2 = house2 * 0.1;
						showWeek (week, house1, house2);
						house1 = house_Number(house1);
						house2 = house_Number(house2);
						space();
					}
					
				}
			}
			//after the 4th week, this asks user if they want to quit or not.
			cout << "Do you want to quit? Y or N: ";
			cin >> answer;
			space();
			month++;//this increments month in order to week the first week as 1,5,9,...

	}while((answer == "N" || answer == "n") && (house1 > 0 && house2 > 0));//the do while loop works as long as user picks N or n

	if ((answer == "Y") || (answer == "y"))//if user decides to quit, then the game ends
	{
		cout << "You have quit. LOSER! \n";
	}
		
	else if (house1 <= 0 && house2 <= 0)//if both houses have 0 or less ants, the user wins
	{
		cout << "You win the game!";
	}
	
	return 0;
}

//in this function, house1 and house2 are given random numbers to begin with
int init_num_ants(int house)
{
	house = rand()%91 + 10;
	return house;
	//house2 = house1 + rand()%91 + 9;
}

//this function displays what is the week and how many ants are in each house
void showWeek (int week, int house1, int house2)
{
	cout << "Week " << week << endl; 
	cout << "House 1 = " << house1 << " ants" << endl;
	cout << "House 2 = " << house2 << " ants" << endl;
}

//this function creates a blank line 
void space()
{
	cout << endl;
}

//this function increases house1 and house2 by 30%
int house_Number (int house)
{
	house = house * 1.3;
	return house;
}