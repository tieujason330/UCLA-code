#include "Actor.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <queue>
#include <cassert>
using namespace std;

/////////////Actor Implementation///////////////
Actor::Actor(int imageID, int startX, int startY, StudentWorld* s) 
: GraphObject(imageID, startX, startY)
{
	m_ID = imageID;
	m_sw = s;
}

///////////Player Implementation///////////////
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
Player::Player(int startX, int startY, StudentWorld* s)
: Actor(IID_PLAYER, startX, startY, s)
{ 
	set_status(true);
	set_Actor_vis(true);
	set_max_bug_sprayers(2);
	set_avail_bug_sprayers(max_bug_sprayers);
	set_walk_thru_bricks(false);
	set_extra_bug_sprayers(false);
	walk_thru_ticks = 0;
	extra_bug_sprayer_ticks = 0;
	deployed_bombs = 0;
}

int Player::performAction()
{
	//if player status is false return player died
	if (!get_status())
		return GWSTATUS_PLAYER_DIED;

	//cout << get_extra_bug_sprayers() << endl;

	//set temp_id to other possible id of player's x and y 
	int temp_id = get_SWorld()->getID_of_other_in_square(getX(), getY());
	
	//if player has the WalkThroughBricks goodie
	if (get_walk_thru_bricks_status())
	{
		//decrement walk-through-brick tick by one
		walk_thru_ticks--;

		//once ticks = 0, player no longer has walkthroughgoodie power
		if (walk_thru_ticks <= 0)
			set_walk_thru_bricks(false);
	}

	//if player has the extra bug_sprayers goodie
	if (get_extra_bug_sprayers())
	{
		//cout << "t max " << max_bug_sprayers << endl;
		//cout << "t avail " << available_bug_sprayers << endl;
		//cout << "t deployed " << deployed_bombs << endl;
		//if (get_avail_bug_sprayers() >= get_max_bug_sprayers())
		//set_avail_bug_sprayers(max_bug_sprayers);

		//if number of deployed bombs =< 0, set it to zero
		//set max bug sprayers to bug sprayer amount from goodie
		//example: 2 bombs currently deployed, pick up goodie
		//max set to max-deployed
		//once those 2 bombs explode then the max = original max
		if (deployed_bombs <= 0)
		{
			set_max_bug_sprayers(get_SWorld()->get_num_sprayers());
			deployed_bombs = 0;
		}

		//decrement simultaneous bug sprayers tick by one
		extra_bug_sprayer_ticks--;

		//once timer is 0, get rid of goodie power
		if (extra_bug_sprayer_ticks <= 0)
		{
			set_max_bug_sprayers(2);
			set_avail_bug_sprayers(max_bug_sprayers);
			set_extra_bug_sprayers(false);
		}
	}

	int ch;
	int dir_id;
	//cout << walk_through_bricks << endl;
	//set_walk_thru_bricks(true);
	
	//gets the user input for movement (WASD)
	//checks to see if square that is being moved to is occupied by either
	//a brick (without goodie)
	//if not then allows the movement
	if (get_SWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_DOWN:
			dir_id = get_SWorld()->getID_of_other_in_square(getX(), getY() - 1);
			if (dir_id != IID_DESTROYABLE_BRICK && dir_id != IID_PERMA_BRICK
				|| (dir_id == IID_DESTROYABLE_BRICK && get_walk_thru_bricks_status()))
				moveTo(getX(), getY() - 1);
			break;
		case KEY_PRESS_UP:
			dir_id = get_SWorld()->getID_of_other_in_square(getX(), getY() + 1);
			if (dir_id != IID_DESTROYABLE_BRICK && dir_id != IID_PERMA_BRICK
				|| (dir_id == IID_DESTROYABLE_BRICK && get_walk_thru_bricks_status()))
				moveTo(getX(), getY() + 1);
			break;
		case KEY_PRESS_LEFT:
			dir_id = get_SWorld()->getID_of_other_in_square(getX() - 1, getY());
			if (dir_id != IID_DESTROYABLE_BRICK && dir_id != IID_PERMA_BRICK
				|| (dir_id == IID_DESTROYABLE_BRICK && get_walk_thru_bricks_status()))
				moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT:
			dir_id = get_SWorld()->getID_of_other_in_square(getX() + 1, getY());
			if (dir_id != IID_DESTROYABLE_BRICK && dir_id != IID_PERMA_BRICK
				|| (dir_id == IID_DESTROYABLE_BRICK && get_walk_thru_bricks_status()))
				moveTo(getX() + 1, getY());
			break;
		//SPACE BAR drops a bug sprayer if the player has any to drop
		case KEY_PRESS_SPACE:
			if (available_bug_sprayers > 0)
			{
				get_SWorld()->insert_Actor(new BugSprayer(getX(), getY(), get_SWorld()));
				available_bug_sprayers--;
				deployed_bombs++;
				//if (deployed_bombs >= available_bug_sprayers)
					//deployed_bombs = max_bug_sprayers - available_bug_sprayers;
			}
			break;
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

//////Brick Implementation/////////

//////DestroyableBrick Implementation///////////
DestroyableBrick::DestroyableBrick(int startX, int startY, StudentWorld* s)
:Actor(IID_DESTROYABLE_BRICK, startX, startY, s)
{
	set_status(true);
	setVisible(true);
}

int DestroyableBrick::performAction()
{
	//if destroyable brick is still alive
	if (get_status())
	{
		//checks if player is occupying same square as destroyable brick
		//AND if player does not have the walk through bricks goodie activated
		//if so then sets the player status to false
		//made the brick check due to issues when the player checked
		if (get_SWorld()->occupy_with_player(getX(), getY()) && get_SWorld()->get_Player()->get_walk_thru_bricks_status() == false)
		{
			get_SWorld()->get_Player()->set_status(false);
			get_SWorld()->playSound(SOUND_PLAYER_DIE);
			return GWSTATUS_PLAYER_DIED;
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

//////PermanentBrick Implementation////////////
PermanentBrick::PermanentBrick(int startX, int startY, StudentWorld* s)
: Actor(IID_PERMA_BRICK, startX, startY, s)
{
	set_status(true);
	set_Actor_vis(true);
}

///////////Exit Implementation//////////////////
Exit::Exit(int startX, int startY, StudentWorld* s)
: Actor(IID_EXIT, startX, startY, s)
{
	set_status(true);
	//set_Actor_vis(false);
	m_sound_finished_lvl = false;
	m_exit_sound = false;
}

int Exit::performAction()
{
	//cout << "in exit performaction()" << endl;
	//cout << "GETWORLD " << get_SWorld()->occupy_with_player(getX(), getY()) << endl;
	//cout << "GET EXIT VIS " << get_Actor_vis() << endl;

	//checks if the exit is visible and if the player is occupying same square as the exit
	//if so then the exit plays the finished-level sound and the next level is loaded by StudentWorld
	if (get_SWorld()->occupy_with_player(getX(), getY()) == true
		&& get_Actor_vis() == true)
	{
		//cout << "in exit statement() *********" << endl;
		if (m_sound_finished_lvl == false)
		{
			get_SWorld()->playSound(SOUND_FINISHED_LEVEL);
			m_sound_finished_lvl = true;
		}
		return GWSTATUS_FINISHED_LEVEL;
	}
}

///////////BugSprayer Implementation/////////////////
BugSprayer::BugSprayer(int startX, int startY, StudentWorld* s)
: Actor(IID_BUGSPRAYER, startX, startY, s)
{
	set_status(true);
	set_Actor_vis(true);
	set_sprayer_lifetime(40);
}

int BugSprayer::performAction()
{
	//cout << m_sprayer_lifetime << endl;

	//if bug sprayer is still alive
	if (get_status() )
	{
		//decrement bug sprayer lifetime
		if (get_sprayer_lifetime() > 0)
			m_sprayer_lifetime--;
		else
		{
			set_sprayer_lifetime(0);

			//once bug sprayer lifetime is depleted
			//generates bugspray in two squares in each direction
			if (get_sprayer_lifetime() == 0)
			{
				get_SWorld()->playSound(SOUND_SPRAY);
				//cout << "works" << endl;
				//make bugspray at location of bugsprayer
				get_SWorld()->insert_Actor(new BugSpray(getX(), getY(), get_SWorld()));
				//cout << "pass " << endl;
				///////test///////
				//get_SWorld()->insert_Actor(new IncSimulSprayerGoodie(getX(), getY(), get_SWorld()));
				///////////////////

				//make bugspray for two squares to the right
				int temp_id;
				int temp_id_prev; //checks the previous sq so if a bugspray doesn't spawn b/c of perma brick
								  //it won't spawn beyond that one
				for (int i = 1; i < 3; i++)
				{
					temp_id_prev = get_SWorld()->getID_of_other_in_square(getX() + (i - 1), getY());
					temp_id = get_SWorld()->getID_of_other_in_square(getX() + i, getY());
					if (temp_id != IID_PERMA_BRICK && temp_id_prev != IID_PERMA_BRICK && getX() + i < VIEW_WIDTH)//checks upperbound of x axis
						get_SWorld()->insert_Actor(new BugSpray(getX() + i, getY(), get_SWorld()));
					if (temp_id == IID_DESTROYABLE_BRICK)
						break;
				}
				//make bugspray for two squares to left
				for (int i = 1; i < 3; i++)
				{
					temp_id_prev = get_SWorld()->getID_of_other_in_square(getX() - (i - 1), getY());
					temp_id = get_SWorld()->getID_of_other_in_square(getX() - i, getY());
					if (temp_id != IID_PERMA_BRICK && temp_id_prev != IID_PERMA_BRICK && getX() - i > 0)//checks lowerbound of x axis
						get_SWorld()->insert_Actor(new BugSpray(getX() - i, getY(), get_SWorld()));
					if (temp_id == IID_DESTROYABLE_BRICK)
						break;
				}
				//make bugspray for two squares up
				for (int i = 1; i < 3; i++)
				{
					temp_id_prev = get_SWorld()->getID_of_other_in_square(getX(), getY() + (i - 1));
					temp_id = get_SWorld()->getID_of_other_in_square(getX(), getY() + i);
					if (temp_id != IID_PERMA_BRICK && temp_id_prev != IID_PERMA_BRICK && getY() + i < VIEW_HEIGHT)//checks upperbound of y axis
						get_SWorld()->insert_Actor(new BugSpray(getX(), getY() + i, get_SWorld()));
					if (temp_id == IID_DESTROYABLE_BRICK)
						break;
				}
				//make bugspray for two squares down
				for (int i = 1; i < 3; i++)
				{
					temp_id_prev = get_SWorld()->getID_of_other_in_square(getX(), getY() - (i - 1));
					temp_id = get_SWorld()->getID_of_other_in_square(getX(), getY() - i);
					if ( temp_id != IID_PERMA_BRICK && temp_id_prev != IID_PERMA_BRICK && getY() - i > 0)//checks lowerbound of y axis
						get_SWorld()->insert_Actor(new BugSpray(getX(), getY() - i, get_SWorld()));
					if (temp_id == IID_DESTROYABLE_BRICK || temp_id == IID_DESTROYABLE_BRICK)
						break;
				}
				//sets the bugsprayer to false so it can be deleted in cleanUp() in StudentWorld
				set_status(false);


				int max = get_SWorld()->get_Player()->get_max_bug_sprayers();
				int avail = get_SWorld()->get_Player()->get_avail_bug_sprayers();

				//once bugsprayer is destroyed, increases available bug sprayers by one
				get_SWorld()->get_Player()->inc_avail_bug_sprayers_by_one();
				
				//makes sure available bug sprayers isn't greater than max amount
				if (get_SWorld()->get_Player()->get_avail_bug_sprayers() >= get_SWorld()->get_Player()->get_max_bug_sprayers())
					get_SWorld()->get_Player()->set_avail_bug_sprayers(max);

				//decrements how many bug sprayers are currently deployed
				get_SWorld()->get_Player()->dec_deployed_bombs_by_one();

				//makes sure deployed amount isn't less than zero
				if (get_SWorld()->get_Player()->get_deployed_bombs() <= 0)
					get_SWorld()->get_Player()->set_deployed_bombs(0);
			}
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

///////////BugSpray Implementation////////////////////
BugSpray::BugSpray(int startX, int startY, StudentWorld* s)
: Actor(IID_BUGSPRAY, startX, startY, s)
{
	set_status(true);
	set_Actor_vis(true);
	m_spray_lifetime = 3;
	//cout << "bugspray" << endl;
}

int BugSpray::performAction()
{
	int temp_id, temp_x, temp_y;
	//cout << m_spray_lifetime << endl;


	if (get_status() == true)
	{
		//decrement lifetime of bugspray
		if (m_spray_lifetime > 0)
			m_spray_lifetime--;
		else
		{
			m_spray_lifetime = 0;
			temp_id = get_SWorld()->getID_of_other_in_square(getX(), getY());
			temp_x = getX();
			temp_y = getY();
			
			//checks to see if bugspray is occupying same square as certain Actors
			//if so then it will set the actor's status to false (killing them)
			//and doing certain functions such as increasing player's score, etc

			//if destroyable brick then the brick gets erased
			if (temp_id == IID_DESTROYABLE_BRICK)
				get_SWorld()->get_Actor(IID_DESTROYABLE_BRICK, temp_x, temp_y)->set_status(false);

			//if bug sprayer than causes a chain reaction
			//explodes that bugsprayer
			if (temp_id == IID_BUGSPRAYER)
			{
				BugSprayer* bs = dynamic_cast<BugSprayer*>(get_SWorld()->get_Actor(IID_BUGSPRAYER, temp_x, temp_y));
				bs->set_sprayer_lifetime(0);
			}

			//if Player, then kills the player
			if (get_SWorld()->occupy_with_player(temp_x, temp_y))
				get_SWorld()->get_Player()->set_status(false);

			//if simple zumi, then kills the zumi and plays enemy_die sound
			//also calls function that helps determine if a goodie will be dropped
			if (temp_id == IID_SIMPLE_ZUMI)
			{
				SimpleZumi* sz = dynamic_cast<SimpleZumi*>(get_SWorld()->get_Actor(IID_SIMPLE_ZUMI, temp_x, temp_y));
				sz->set_status(false);
				get_SWorld()->playSound(SOUND_ENEMY_DIE);
				get_SWorld()->increaseScore(100);
				determine_Goodie_type();
			}

			//if complex zumi, then kills zumi and plays enemy_die sound
			//also calls a function that helps determine if a goodie will be dropped
			if (temp_id == IID_COMPLEX_ZUMI)
			{
				ComplexZumi* cz = dynamic_cast<ComplexZumi*>(get_SWorld()->get_Actor(IID_COMPLEX_ZUMI, temp_x, temp_y));
				cz->set_status(false);
				get_SWorld()->playSound(SOUND_ENEMY_DIE);
				get_SWorld()->increaseScore(500);
				determine_Goodie_type();
			}

			//sets bugspray to false so it can be deleted
			set_status(false);
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

//created this to help determine which goodie will be dropped
void BugSpray::determine_Goodie_type()
{
	//got the probabilities for each goodie from StudentWorld
	int overall = get_SWorld()->get_prob_goodie();
	int extra = get_SWorld()->get_prob_extra_life();
	int walk = get_SWorld()->get_prob_walk_thru();
	int multi = get_SWorld()->get_prob_simul_bugspray();

	//made a vector for the values so they could be sorted in order
	vector<int> goodies = { extra, walk, multi };

	//sorting the chances for each goodie least to greatest
	sort(goodies.begin(), goodies.end());


	bool check_for_zero = false;
	//checks if any probabilities are zero due to special cases of spawning goodies
	for (vector<int>::iterator p = goodies.begin(); p != goodies.end(); p++)
	{
		if (*p == 0)
		{
			check_for_zero = true;
			break;
		}
	}

	//randomizes chance variable
	srand(time(NULL));
	int chance = 1 + (rand() % 99);

	//if chance is less than the overall probability
	//then a goodie will spawn
	if (chance < overall)
	{
		//set goodies in ascending order to specific variables
		vector<int>::iterator p = goodies.begin();
		int least = *p;
		int mid = *(p + 1);
		int big = *(p + 2);
		//cout << least << endl << mid << endl << big << endl;

		//randomizes a second value to see which goodie will be dropped
		int chance2 = (rand() % 99);
		//if there is a zero
		//then only checks the middle and largest variables
		if (check_for_zero)
		{
			if (chance2 < mid)
				spawn_goodie(mid);
			if (chance2 >= mid)
				spawn_goodie(big);
		}
		//else no zero, then checks between all three variables
		else
		{
			if (chance2 < least)
				spawn_goodie(least);
			if (chance2 >= least && chance2 <= big)
				spawn_goodie(mid);
			if (chance2 > big)
				spawn_goodie(big);
		}

	}
}

//this function is called by determine_Goodie_type()
//it takes in an int to help determine which goodie will be spawned
void BugSpray::spawn_goodie(int x)
{
	//cout << "in spwan goodie" << endl;

	//gets the probability of each goodie
	int extra = get_SWorld()->get_prob_extra_life();
	int walk = get_SWorld()->get_prob_walk_thru();
	int multi = get_SWorld()->get_prob_simul_bugspray();
	
	//determins which goodie will be dropped
	if (x == extra)
		get_SWorld()->insert_Actor(new ExtraLifeGoodie(getX(), getY(), get_SWorld()));
	if (x == walk)
		get_SWorld()->insert_Actor(new WalkThruWallsGoodie(getX(), getY(), get_SWorld()));
	if (x == multi)
		get_SWorld()->insert_Actor(new IncSimulSprayerGoodie(getX(), getY(), get_SWorld()));


}
/////////////////ExtraLifeGoodie Implementation////////////////////////////
ExtraLifeGoodie::ExtraLifeGoodie(int startX, int startY, StudentWorld* s)
:Actor(IID_EXTRA_LIFE_GOODIE, startX, startY, s)
{
	set_status(true);
	set_Actor_vis(true);
	m_timer = get_SWorld()->get_extra_lifetime();
}

int ExtraLifeGoodie::performAction()
{
	//if ExtraLifeGoodie is alive
	if (get_status())
	{
		//reduce its timer
		if (m_timer > 0)
			m_timer--;
		//if timer is zero then set its status to zero so StudentWorld can delete it
		else
		{
			m_timer = 0;
			set_status(false);
		}
		//if player is occupying same square as this goodie
		//increases player life
		//increases score by 1000
		//sets goodie status to false
		//calls StudentWorld to play got_goodie sound
		if (get_SWorld()->occupy_with_player(getX(), getY()))
		{
			get_SWorld()->incLives(); //increases player life by 1
			get_SWorld()->increaseScore(1000); //increase score by 1000
			set_status(false);
			get_SWorld()->playSound(SOUND_GOT_GOODIE);
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

//////////////WalkThruWallsGoodie Implementation//////////////////////////
WalkThruWallsGoodie::WalkThruWallsGoodie(int startX, int startY, StudentWorld* s)
: Actor(IID_WALK_THRU_GOODIE, startX, startY, s)
{
	set_status(true);
	set_Actor_vis(true);
	m_timer = get_SWorld()->get_walk_thru_lifetime();
}

int WalkThruWallsGoodie::performAction()
{
	//cout << m_timer << endl;

	//if WalkThru goodie is aliev
	if (get_status())
	{
		//reduce it's timer 
		if (m_timer > 0)
			m_timer--;
		//if timer is 0 then set it's status to 0 so StudentWorld can delete it next tick
		else
		{
			m_timer = 0;
			set_status(false);
		}
		//if player is occupying same square as this goodie
		//sets player's timer for using this goodie power
		//increases player life
		//increases score by 1000
		//sets goodie status to false
		//calls StudentWorld to play got_goodie sound
		if (get_SWorld()->occupy_with_player(getX(), getY()))
		{
			int ticks = get_SWorld()->get_walk_thru_ticks();
			get_SWorld()->get_Player()->set_walk_thru_ticks(ticks);
			get_SWorld()->get_Player()->set_walk_thru_bricks(true);
			get_SWorld()->increaseScore(1000); //increase score by 1000
			set_status(false);
			get_SWorld()->playSound(SOUND_GOT_GOODIE);
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

///////////////IncSimulSprayerGoodie Implementation/////////////////////
IncSimulSprayerGoodie::IncSimulSprayerGoodie(int startX, int startY, StudentWorld* s)
: Actor(IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, startX, startY, s)
{
	set_status(true);
	set_Actor_vis(true);
	m_timer = get_SWorld()->get_inc_simul_lifetime();
}

int IncSimulSprayerGoodie::performAction()
{
	//if multi-sprayers goodie is alive
	if (get_status())
	{
		//reduce its timer
		if (m_timer > 0)
			m_timer--;
		//if timer is 0 then sets status to 0 so StudentWorld can delete it next tick
		else
		{
			m_timer = 0;
			set_status(false);
		}
		//if player is occupying same square as this goodie
		//set timer for player's usage of this goodie power
		//increases player life
		//increases score by 1000
		//sets goodie status to false
		//calls StudentWorld to play got_goodie sound
		if (get_SWorld()->occupy_with_player(getX(), getY()))
		{
			int ticks = get_SWorld()->get_inc_simul_ticks();

			//gets sprayer_num for that particular level
			int sprayer_num = get_SWorld()->get_num_sprayers();
			
			get_SWorld()->get_Player()->set_extra_bug_sprayers_ticks(ticks);

			//get deployed amount of bugsprayers from Player
			int deployed = get_SWorld()->get_Player()->get_deployed_bombs();

			//if player has not had this goodie power
			//then increases the max number of bugsprayers to sprayer_num - deployed 
			//then sets available amount of bug sprayers to the max amount
			//some complications here during testing
			//at some times the max,available, and deployed amount change
			//can't exactly pinpoint the issue
			if (get_SWorld()->get_Player()->get_extra_bug_sprayers() == false)
			{
				get_SWorld()->get_Player()->set_max_bug_sprayers(sprayer_num - deployed);
				get_SWorld()->get_Player()->set_avail_bug_sprayers(get_SWorld()->get_Player()->get_max_bug_sprayers());
			}
			get_SWorld()->increaseScore(1000); //increase score by 1000
			get_SWorld()->get_Player()->set_extra_bug_sprayers(true);
			set_status(false);
			get_SWorld()->playSound(SOUND_GOT_GOODIE);
		}

	}
	return GWSTATUS_CONTINUE_GAME;
}

///////////////SimpleZumi////////////////////////////////////
SimpleZumi::SimpleZumi(int startX, int startY, StudentWorld* s)
: Actor(IID_SIMPLE_ZUMI, startX, startY, s)
{
	set_status(true);
	set_Actor_vis(true);
	srand(time(NULL));
	m_currentDirection = 1 + (rand() % 4); //generates random between 1 and 4
	m_move_ticks = get_SWorld()->get_simple_zumi_ticks();
}

int SimpleZumi::performAction()
{
	int temp_id, temp_x, temp_y;

	//checks to see if SimpleZumi is alive
	//if so then allowed to do actiosn
	if (get_status())
	{
		//sets if occupying with player
		//if so then sets player status to false
		//play's player died sound
		//returns that player has died
		if (get_SWorld()->occupy_with_player(getX(), getY()))
		{
			get_SWorld()->get_Player()->set_status(false);
			get_SWorld()->playSound(SOUND_PLAYER_DIE);
			return GWSTATUS_PLAYER_DIED;
		}

		//if move ticks is greater than zero, decrements (cannot move yet)
		if (m_move_ticks > 0)
			m_move_ticks--;
		//if move ticks is zero, allowed to perform an action
		if (m_move_ticks <= 0)
		{
			temp_x = getX();
			temp_y = getY();
			//cout << m_currentDirection << endl;
			//srand(time(NULL));
			//switch statement for currentDirection of SimpleZumi (randomized when SimpleZumi is created)
			//each case is the same for different directions
			//checks to see if the square being moved to is occupied by a certain object
			//if not, then moves
			//then randomly picks a new direction
			switch (m_currentDirection)
			{
			case 1: //moves up
				temp_id = get_SWorld()->getID_of_other_in_square(temp_x, temp_y + 1);
				if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
					&& temp_id != IID_BUGSPRAYER)
					moveTo(temp_x, temp_y + 1);
				m_currentDirection = 1 + (rand() % 4);
				break;

			case 2://moves down
				temp_id = get_SWorld()->getID_of_other_in_square(temp_x, temp_y - 1);
				if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
					&& temp_id != IID_BUGSPRAYER)
					moveTo(temp_x, temp_y - 1);
				m_currentDirection = 1 + (rand() % 4);
				break;
				
			case 3://moves left
				temp_id = get_SWorld()->getID_of_other_in_square(temp_x - 1, temp_y);
				if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
					&& temp_id != IID_BUGSPRAYER)
					moveTo(temp_x - 1, temp_y);
				m_currentDirection = 1 + (rand() % 4);
				break;
				
			case 4://moves right
				temp_id = get_SWorld()->getID_of_other_in_square(temp_x + 1, temp_y);
				if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
					&& temp_id != IID_BUGSPRAYER)
					moveTo(temp_x + 1, temp_y);
				m_currentDirection = 1 + (rand() % 4);
				break;
					
			default:
				m_currentDirection = 1 + (rand() % 4);
			}
			//since move ticks is zero, allowing SimpleZumi to move
			//resets move_ticks and goes on
			m_move_ticks = get_SWorld()->get_simple_zumi_ticks();

		}
	}
	
	return GWSTATUS_CONTINUE_GAME;

}

////////////////ComplexZumi Implementation////////////////////////////////////
ComplexZumi::ComplexZumi(int startX, int startY, StudentWorld* s)
: Actor(IID_COMPLEX_ZUMI, startX, startY, s)
{
	set_status(true);
	set_Actor_vis(true);
	srand(time(NULL));
	m_currentDirection = 1 + (rand() % 4); //generates random between 1 and 4
	m_move_ticks = get_SWorld()->get_complex_zumi_ticks();
	m_search_dist = get_SWorld()->get_complex_zumi_search_dist();
	set_smell(false);
	path_exists = false;
	search = false;

	//stores a queue of coords for the level
	for (int i = 0; i < VIEW_WIDTH; i++)
	{
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			Coord w;
			w.x = i;
			w.y = j;
			w.disc = false;
			level.push(w);
		}
	}
}


int ComplexZumi::performAction()
{
	int temp_id;
	int temp_x = getX();
	int temp_y = getY();
	//queue<Coord> points;
	//bool search = false;
	//bool search2 = false;

	//gets search distance for ComplexZumi from StudentWorld
	m_search_dist = get_SWorld()->get_complex_zumi_search_dist();

	int horzDist, vertDist;

	//if ComplexZumi is alive
	if (get_status())
	{
		//checks to see if ComplexZumi is on same square as Player
		//if so then set player status to false
		//play sound for player death
		//return player_died constant
		if (get_SWorld()->occupy_with_player(getX(), getY()))
		{
			get_SWorld()->get_Player()->set_status(false);
			get_SWorld()->playSound(SOUND_PLAYER_DIE);
			return GWSTATUS_PLAYER_DIED;
		}

		//if ticks is not zero, decrement (can't move yet)
		if (m_move_ticks > 0)
		{
			m_move_ticks--;
		}
		//if ticks is zero, allowed to do a movement
		if (m_move_ticks <= 0)
		{
			//checks horizontal and vertical distance from player location
			horzDist = abs(get_SWorld()->get_Player()->getX() - getX());
			vertDist = abs(get_SWorld()->get_Player()->getY() - getY());

			//if horizontal and vertical distance are both less than search distance
			//sets smell to true
			if (horzDist <= m_search_dist && vertDist <= m_search_dist)
				set_smell(true);
			else
				set_smell(false);

			/*
			//if smell is true, then does a breadth-first search to determine the
			//shortest route to the player
			if (get_smell() == true)
			{
				search = path_found(getX(), getY(), get_SWorld()->get_Player()->getX(), get_SWorld()->get_Player()->getY());
				
				if (search == true)
				{
					temp_x = m_p.front().x;
					temp_y = m_p.front().y;
					temp_id = get_SWorld()->getID_of_other_in_square(temp_x, temp_y);
					if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
						&& temp_id != IID_BUGSPRAYER)
						moveTo(temp_x, temp_y);
					m_p.pop();
				}
				//breadth-first search
				//if new queue is smaller than old queue
				//new queue overwrites old queue
				//else old queue stays
			}
			*/
			//if smell is false, the ComplexZumi acts as a SimpleZumi
			//if (get_smell() == false || search == false)
			{
				temp_x = getX();
				temp_y = getY();
				switch (m_currentDirection)
				{
				case 1: //moves up
					temp_id = get_SWorld()->getID_of_other_in_square(temp_x, temp_y + 1);
					if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
						&& temp_id != IID_BUGSPRAYER)
						moveTo(temp_x, temp_y + 1);
					m_currentDirection = 1 + (rand() % 4);
					break;

				case 2://moves down
					temp_id = get_SWorld()->getID_of_other_in_square(temp_x, temp_y - 1);
					if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
						&& temp_id != IID_BUGSPRAYER)
						moveTo(temp_x, temp_y - 1);
					m_currentDirection = 1 + (rand() % 4);
					break;

				case 3://moves left
					temp_id = get_SWorld()->getID_of_other_in_square(temp_x - 1, temp_y);
					if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
						&& temp_id != IID_BUGSPRAYER)
						moveTo(temp_x - 1, temp_y);
					m_currentDirection = 1 + (rand() % 4);
					break;

				case 4://moves right
					temp_id = get_SWorld()->getID_of_other_in_square(temp_x + 1, temp_y);
					if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
						&& temp_id != IID_BUGSPRAYER)
						moveTo(temp_x + 1, temp_y);
					m_currentDirection = 1 + (rand() % 4);
					break;

				default:
					m_currentDirection = 1 + (rand() % 4);
				}
				//since move ticks is zero, it gets reset after the zumi moves
				m_move_ticks = get_SWorld()->get_complex_zumi_ticks();
			}
		}
	}
	else
		return GWSTATUS_CONTINUE_GAME;
}

bool ComplexZumi::path_found(int sX, int sY, int eX, int eY)
{
	queue<Coord> coords;
	Coord a;
	a.x = sX;
	a.y = sY;
	coords.push(a);
	//this should mark the Coord in map as true 
	set_Coord_disc(level, a, true);


	while (!coords.empty())
	{
		Coord b = coords.front();
		int nX = b.x;
		int nY = b.y;
		coords.pop();
		
		//check if x and y are same as player x and y
		if (nX == eX && nY == eY)
		{
			m_p = coords;
			return true;
		}

		int temp_id;
		//if can move UP
		temp_id = get_SWorld()->getID_of_other_in_square(nX, nY + 1);
		if (nY + 1 < VIEW_HEIGHT)
		{
			Coord u;
			u.x = nX;
			u.y = nY + 1;
			if (!get_disc(level, u))
			{
				coords.push(u);
				set_Coord_disc(level, u, true);
			}
		}
		//if can move DOWN
		temp_id = get_SWorld()->getID_of_other_in_square(nX, nY - 1);
		if (nY - 1 > 0)
		{
			Coord u;
			u.x = nX;
			u.y = nY - 1;
			if (!get_disc(level, u))
			{
				coords.push(u);
				set_Coord_disc(level, u, true);
			}
		}
		//if can move RIGHT
		temp_id = get_SWorld()->getID_of_other_in_square(nX + 1, nY);
		if ( nX + 1 < VIEW_WIDTH)
		{
			Coord u;
			u.x = nX + 1;
			u.y = nY;
			if (!get_disc(level, u))
			{
				coords.push(u);
				set_Coord_disc(level, u, true);
			}
		}
		//if can move LEFT
		temp_id = get_SWorld()->getID_of_other_in_square(nX - 1, nY);
		if (nX - 1 > 0)
		{
			Coord u;
			u.x = nX - 1;
			u.y = nY;
			if (!get_disc(level, u))
			{
				coords.push(u);
				set_Coord_disc(level, u, true);
			}
		}
	}
	return false;
}

//sets the bool disc of a coord to argument z
//takes in a queue, Coord, and bool
void ComplexZumi::set_Coord_disc(queue<Coord>& g, Coord t, bool z)
{
	int tx = t.x;
	int ty = t.y;

	//makes a temporary queue
	queue<Coord> temp = g;
	//searches g to find the correct and return it
	while (!g.empty())
	{
		if (g.front().x == tx && g.front().y == ty)
		{
			//set bool to true to show it has been discovered
			g.front().disc = z;
			break;
		}
		g.pop();
	}
	//if g is empty, sets g to temp(the original content) so g does not change
	g = temp;
}

//gets the bool disc of a coord 
//takes in a queue and a Coord
bool ComplexZumi::get_disc(queue<Coord> g, Coord t)
{
	int tx = t.x;
	int ty = t.y;

	//makes a temporary queue
	queue<Coord> temp = g;
	//searches g to find the correct and return it
	while (!temp.empty())
	{
		if (temp.front().x == tx && temp.front().y == ty)
			//set bool to true to show it has been discovered
			return temp.front().disc;
		temp.pop();
	}
	return false;
	//if g is empty, sets g to temp(the original content) so g does not change
}

/*
bool ComplexZumi::path_found()
{
	//mape a queue that tracks all the x,y coordinates of a map
	//to be used to check if the square has already been discovered or not
	
	queue<Coord> map;
	for (int i = 0; i < VIEW_WIDTH; i++)
	{
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			Coord w;
			w.x = i;
			w.y = j;
			w.disc = true;
			map.push(w);
		}
	}
	

	queue<Coord> points;
	int temp_id;
	int temp_x;
	int temp_y;

	//determine the first step
	Coord start;
	cout << m_currentDirection << endl;
	temp_x = getX();
	temp_y = getY();
	switch (m_currentDirection)
	{
	case 1://up
		temp_id = get_SWorld()->getID_of_other_in_square(temp_x, temp_y + 1);
		if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
			&& temp_id != IID_BUGSPRAYER)
		{
			start.x = getX();
			start.y = getY() + 1;
		}
		break;
	case 2://down
		temp_id = get_SWorld()->getID_of_other_in_square(temp_x, temp_y - 1);
		if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
			&& temp_id != IID_BUGSPRAYER)
		{
			start.x = getX();
			start.y = getY() - 1;
		}
		break;
	case 3://left
		temp_id = get_SWorld()->getID_of_other_in_square(temp_x - 1, temp_y);
		if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
			&& temp_id != IID_BUGSPRAYER)
		{
			start.x = getX() - 1;
			start.y = getY();
		}
		break;
	case 4://right
		temp_id = get_SWorld()->getID_of_other_in_square(temp_x + 1, temp_y);
		if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
			&& temp_id != IID_BUGSPRAYER)
		{
			start.x = getX() + 1;
			start.y = getY();
		}
		break;
	}

	points.push(start);

	while (!points.empty())
	{
		Coord possible_end;
		possible_end.x = points.front().x;
		possible_end.y = points.front().y;

		cout << possible_end.x << "," << possible_end.y << endl;
		//cout << "* " << get_SWorld()->get_Player()->getX() << "," << get_SWorld()->get_Player()->getY() << endl;
		points.pop();

		//if the end x and y equal to Player's x and y
		//we have found a solution so return true
		if (possible_end.x == get_SWorld()->get_Player()->getX() && 
			possible_end.y == get_SWorld()->get_Player()->getY())
		{
			if (!points.empty())
				m_p = points;
			return true;
		}
		
		temp_x = possible_end.x;
		temp_y = possible_end.y;

		//must make sure the tile in each direction is valid
		//cannot be a brick or bugsprayer

		//checks up
		temp_id = get_SWorld()->getID_of_other_in_square(temp_x, temp_y + 1);
		if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
			&& temp_id != IID_BUGSPRAYER && temp_y + 1 < VIEW_HEIGHT)
		{
			Coord u;
			u.x = temp_x;
			u.y = temp_y + 1;
			u.disc = true;
			if (!queue_contains(points, u) && !get_disc(map, u))
			{
				points.push(u);
				set_Coord_disc(map, u);
			}
		}
		//checks down
		temp_id = get_SWorld()->getID_of_other_in_square(temp_x, temp_y - 1);
		if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
			&& temp_id != IID_BUGSPRAYER && temp_y - 1 > 0)
		{
			Coord d;
			d.x = temp_x;
			d.y = temp_y - 1;
			d.disc = true;
			if (!queue_contains(points, d) && !get_disc(map, d))
			{
				points.push(d);
				set_Coord_disc(map, d);
			}
		}
		//checks left
		temp_id = get_SWorld()->getID_of_other_in_square(temp_x - 1, temp_y);
		if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
			&& temp_id != IID_BUGSPRAYER && temp_x - 1 > 0)
		{
			Coord l;
			l.x = temp_x - 1;
			l.y = temp_y;
			l.disc = true;
			if (!queue_contains(points, l) && !get_disc(map, l))
			{
				points.push(l);
				set_Coord_disc(map, l);
			}
		}
		//checks right
		temp_id = get_SWorld()->getID_of_other_in_square(temp_x + 1, temp_y);
		if (temp_id != IID_DESTROYABLE_BRICK && temp_id != IID_PERMA_BRICK
			&& temp_id != IID_BUGSPRAYER && temp_x + 1 < VIEW_WIDTH)
		{
			Coord r;
			r.x = temp_x + 1;
			r.y = temp_y;
			r.disc = true;
			if (!queue_contains(points, r) && !get_disc(map, r))
			{
				points.push(r);
				set_Coord_disc(map, r);
			}
		}
	}
	//queue is empty and there is no solution
	return false;
}
*/

