#include "Actor.h"
#include <string>
using namespace std;

/////////////Actor Implementation///////////////
void Actor::set_square_occ_status(int imageID, int x, int y)
{
	Square newSq;
	newSq.id = imageID;
	newSq.x = x;
	newSq.y = y;
	for (vector<Square>::iterator p = m_squares.begin(); p != m_squares.end(); p++)
	{
		Square temp = *p;
		if (newSq.id == temp.id && newSq.x == temp.x && newSq.y == temp.y)
			return;
	}
	m_squares.push_back(newSq);
}

///////////Player Implementation///////////////
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
Player::Player(int startX, int startY, StudentWorld* s)
: Actor(IID_PLAYER, startX, startY, *s)
{ 
	set_status(true);
	setVisible(true);
	available_bug_sprayers = 2;
	set_walk_thru_bricks(false);
	set_extra_bug_sprayers(false);
	set_square_occ_status(IID_PLAYER, startX, startY);
}

int Player::performAction()
{
	if (!get_status())
		return GWSTATUS_PLAYER_DIED;
///**************change this part///////////////////////////////////////////////////////////
	for (vector<Square>::const_iterator p = get_squares().begin(); p != get_squares().end(); p++)
	{
		Square temp = *p;
		if ((getX() == temp.x) && (getY() == temp.y))
		{
			if (temp.id == IID_SIMPLE_ZUMI || temp.id == IID_COMPLEX_ZUMI
				|| (!get_walk_thru_bricks_status() && (temp.id == IID_DESTROYABLE_BRICK))
				|| (temp.id == IID_BUGSPRAY) )
			{
				getWorld()->playSound(SOUND_PLAYER_DIE);
				set_status(false);
				//setVisible(false);
				return GWSTATUS_PLAYER_DIED;
			}
		}
	}
////////////////////////////////////////////////
	if (get_walk_thru_bricks_status())
	{
		//decrement walk-through-brick tick by one
	}
	if (get_extra_bug_sprayers())
	{
		//decrement simultaneous bug sprayers tick by one
	}

	int ch;
	//set_walk_thru_bricks(true);
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_DOWN:
			//cout << get_SWorld()->squareIsOccupied(getX(), getY() - 1);
			//cout << "walk_through_bricks: " << walk_through_bricks << " " << endl;
			if (!get_SWorld()->squareIsOccupied(getX(), getY() -1) )
				moveTo(getX(), getY() - 1);
			break;
		case KEY_PRESS_UP:
			if (!get_SWorld()->squareIsOccupied(getX(), getY() + 1) )
				moveTo(getX(), getY() + 1);
			break;
		case KEY_PRESS_LEFT:
			//cout << get_SWorld()->squareIsOccupied(getX() - 1, getY());
			//cout << "walk_through_bricks: " << walk_through_bricks << " " << endl;
			if (!get_SWorld()->squareIsOccupied(getX() - 1, getY()) )
				moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT:
			//cout << get_SWorld()->squareIsOccupied(getX() + 1, getY());
			//cout << "walk_through_bricks: " << walk_through_bricks << " " << endl;
			if (!get_SWorld()->squareIsOccupied(getX() + 1, getY()) )
				moveTo(getX() + 1, getY());
			break;
		case KEY_PRESS_SPACE:
			if (available_bug_sprayers > 0)
			{
				//create a BugSprayer object at getX, getY
				//available_bug_sprayers--;
			}
			break;
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

void Player::gained_Pickup()
{}

//////Brick Implementation/////////

//////DestroyableBrick Implementation///////////
DestroyableBrick::DestroyableBrick(int startX, int startY, StudentWorld* s)
:Actor(IID_DESTROYABLE_BRICK, startX, startY, *s)
{
	set_status(true);
	setVisible(true);
	set_square_occ_status(IID_DESTROYABLE_BRICK, startX, startY);
}

//////PermanentBrick Implementation////////////
PermanentBrick::PermanentBrick(int startX, int startY, StudentWorld* s)
: Actor(IID_PERMA_BRICK, startX, startY, *s)
{
	set_status(true);
	setVisible(true);
	set_square_occ_status(IID_PERMA_BRICK, startX, startY);
}
