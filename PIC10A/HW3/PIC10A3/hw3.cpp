/* Jason tieu
	304047667
	5/2/12
	This assignment is a tic-tac-toe game. It is played between two opponents. 
	The maximum amount of clicks would be 9. After one player gets three Xs or Os in a row,
	the game ends and a new one can start.
*/

#include "ccc_win.h"
//Initializing functions
void gameBoard();
Point getX();
Point getO();
void drawX(Point b);
void drawO(Point b);
void drawBox(int box);
int getBox(Point b);

//Creating the tic tac toe board using coordinate system and lines
void gameBoard() 
{
	cwin.coord (3,3,0,0);
	cwin << Line (Point (1,0), Point (1,3));
	cwin << Line (Point (2,0), Point (2,3)); 
	cwin << Line (Point (0,1), Point (3,1));
	cwin << Line (Point (0,2), Point (3,2));
}


//Prompting the user to click for X
Point getX()
{
	return cwin.get_mouse("X goes.");
}

//Prompting the user to  click for O
Point getO()
{
	return cwin.get_mouse("O goes."); 
}

//Drawing a X out of two lines
void drawX (Point b)
{
	const double Xlength = .4; 
	cwin << Line (Point (b.get_x() - Xlength, b.get_y() + Xlength), Point (b.get_x() + Xlength, b.get_y() - Xlength));
	cwin << Line (Point (b.get_x() - Xlength, b.get_y() - Xlength), Point (b.get_x() + Xlength, b.get_y() + Xlength));
}

//Drawing an O using the Circle (point, radius)
void drawO (Point b)
{
	const double RADIUS = .4;
	cwin << Circle (b , RADIUS);
}

int getBox(Point b)// this determines what the value of each box is
{
	if (b.get_x() <= 1 && b.get_x() >= 0 &&
		b.get_y() <= 1 && b.get_x() >= 0)
		return 11;
	else if (b.get_x() <= 1 && b.get_x() >= 0 &&
		b.get_y() <= 2 && b.get_x() >= 1)
		return 12;
	else if (b.get_x() <= 2 && b.get_x() >= 1 &&
		b.get_y() <= 3 && b.get_x() >= 2)
		return 13;
	else if (b.get_x() <= 2 && b.get_x() >= 1 &&
		b.get_y() <= 1 && b.get_x() >= 0)
		return 21;
	else if (b.get_x() <= 2 && b.get_x() >= 1 &&
		b.get_y() <= 2 && b.get_x() >= 1)
		return 22;
	else if (b.get_x() <= 2 && b.get_x() >= 1 &&
		b.get_y() <= 3 && b.get_x() >= 2)
		return 23;
	else if (b.get_x() <= 3 && b.get_x() >= 2 &&
		b.get_y() <= 1 && b.get_x() >= 0)
		return 31;
	else if (b.get_x() <= 3 && b.get_x() >= 2 &&
		b.get_y() <= 2 && b.get_x() >= 1)
		return 32;
	else if (b.get_x() <= 3 && b.get_x() >= 2 &&
		b.get_y() <= 3 && b.get_x() >= 2)
		return 33;
}

void drawBox(int box)//this draws an X in the boxes on the grid 
{
	if (box==11)
	{
	cwin << Line (Point (0,0), Point (1,1));
	cwin << Line (Point (1,0), Point (0,1));
	}
	else if (box==12)
	{
	cwin << Line (Point (0,1), Point (1,2));
	cwin << Line (Point (1,1), Point (0,2));
	}
	else if (box==13)
	{
	cwin << Line (Point (0,2), Point (1,3));
	cwin << Line (Point (1,2), Point (0,3));
	}
	else if (box==21)
	{
	cwin << Line (Point (1,0), Point (2,1));
	cwin << Line (Point (2,0), Point (1,1));
	}
	else if (box==22)
	{
	cwin << Line (Point (1,1), Point (2,2));
	cwin << Line (Point (2,1), Point (1,2));
	}
	else if (box==23)
	{
	cwin << Line (Point (1,2), Point (2,3));
	cwin << Line (Point (2,2), Point (1,3));
	}
	else if (box==31)
	{
	cwin << Line (Point (2,0), Point (3,1));
	cwin << Line (Point (3,0), Point (2,1));
	}
	else if (box==32)
	{
	cwin << Line (Point (2,1), Point (3,2));
	cwin << Line (Point (3,1), Point (2,2));
	}
	else if (box==33)
	{
	cwin << Line (Point (2,2), Point (3,3));
	cwin << Line (Point (3,2), Point (2,3));
	}

}

//main function begins here
int ccc_win_main()
{
	
	string answer = "y"; //starts off the game with answer as Y, so first tic tac toe match would begin
	
	do //using do while for the game
	{
		gameBoard(); //get gameboard function
		int turn = 1;//start turn at 1

		do
		{
			if (turn%2 == 1) { //if turn divided 2 remainder is 1, then draw for X
				//int box = getBox( getX() );
				//if(box==1)
					// box = 11;
				//drawBox(getBox(11));
				drawX ( getX () );
			}
			else{ //draw for O
				drawO ( getO () );
			}

			//game_over = check_winner(box_1, box_2, ...);
			turn ++;//turn increments by 1

		} while (turn <= 9);//do the do while function while turn is less than or equal to 9
		//while ( (turn <= 9 && !game_over);
		answer = cwin.get_string( "Do you wish to play again? Y/N?" );//afterwards, ask user if they want to play again
		cwin.clear(); 
		
	} while (answer == "Y" || answer == "y");//do this entire do while when user inputs "y" or "Y"

	return 0;
}