#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

StudentWorld::StudentWorld()
{
	levels.push_back("level00.dat");
	levels.push_back("level01.dat");
	levels.push_back("level02.dat");
	cur_lvl = 0;
}

StudentWorld::~StudentWorld()
{
	//used vectors to use clear function to delete actors and levels
	actors.clear();
	levels.clear();

	//sets all actors status to false
	vector<Actor*>::iterator p;
	for (p = actors.begin(); p != actors.end(); p++)
	{
		(*p)->set_status(false);
	}
	//sets player status to false
	get_Player()->set_status(false);

	//this function deletes objects that have status as false
	removeDeadGameObjects();
}
//deciding not to use iterators to due crashes
int StudentWorld::move()
{
	//update the score, level, lives, etc
	UpdateDisplayText();
	
	//made an exit pointer so i can specifically call it later on
	Exit* exit_ptr;

	/*
	//decided not to use iterator due to issues when loading next level
	//iterator incrementation sees to be invalid and causes game to crash
	//left code here for further reference in the future
	
	vector<Actor*>::iterator p;
	for (p = actors.begin(); p != actors.end(); p++)
	{
		if (ActorIsNotDead(*(*p)))
		{
			if ((*p)->get_status())
				(*p)->performAction();
			if ((*p)->get_ActorID() == IID_EXIT) 
				exit_ptr = dynamic_cast<Exit*>(*p); //keep track of exit to play sound when it appears

		}
		
		if (!PlayerIsNotDead())
			return GWSTATUS_PLAYER_DIED;

		if (isCurrentlLevelCompleted())
		{
			IncreaseScoreByBonus();
			//levels.pop_front();
			levels.pop();
			cur_lvl++;
			return GWSTATUS_FINISHED_LEVEL;
		}
	}
	*/

	//goes through each actor in the actors vector and gets their status
	//if status is true then call their performAction()
	for (int i = 0; i < actors.size(); i++)
	{
		if (ActorIsNotDead(*actors[i]))
		{
			if (actors[i]->get_status())
				actors[i]->performAction();
			if (actors[i]->get_ActorID() == IID_EXIT)
				exit_ptr = dynamic_cast<Exit*>(actors[i]); //keep track of exit to play sound when it appears

		}

		//if player is dead then returns player_died
		if (!PlayerIsNotDead())
			return GWSTATUS_PLAYER_DIED;

		//if level is completed
		//then increase score by however much bonus is left
		//plays sound finished_level
		//increments cur_levl to be used in LoadLevel() function to allow loading of next level
		//returns finished_level
		if (isCurrentLevelCompleted())
		{
			IncreaseScoreByBonus();
			playSound(SOUND_FINISHED_LEVEL);
			//levels.pop_front();
			//levels.pop();
			cur_lvl++;
			return GWSTATUS_FINISHED_LEVEL;
		}
	}

	//if player isn't dead, then call it's performAction()
	if (PlayerIsNotDead())
		player->performAction();

	//remove any dead objects
	removeDeadGameObjects();
	//decrements bonus each ticks
	reduceLevelBonusByOne();

	//cout << allZumiDead() << endl;

	//if all zumis are dead
	if (allZumiDead())
	{
		//sets exit visibility to true
		revealExitinMaze();
		//plays exit sound
		//had check if false and then set to true
		//due to continuous looping of the sound being played
		//this way the sound only plays once
		if (!exit_ptr->get_exit_sound())
		{
			playSound(SOUND_REVEAL_EXIT);
			exit_ptr->set_exit_sound(true);
		}
	}

	//if player is dead then returns player_died
	//this is checked again outside the Actor's performAction() for loop
	//previous one is checked inbetween each Actor performAction()
	if (!PlayerIsNotDead())
		return GWSTATUS_PLAYER_DIED;

	//check if level is completed
	//this is checked again outside the Actor's performAction() for loop
	//prevosu one is checked inbetween each Actor's performAction()
	if (isCurrentLevelCompleted())
	{
		IncreaseScoreByBonus();
		playSound(SOUND_FINISHED_LEVEL);
		//levels.pop_front();
		//levels.pop();
		cur_lvl++;
		return GWSTATUS_FINISHED_LEVEL;
	}	
	return GWSTATUS_CONTINUE_GAME;
}

//this cleanUp function sets all Actor status' to false
//then calls removeDeadGameObjects() to delete them
//also decrements life of player
//this function is called when player is killed
//resets the level
void StudentWorld::cleanUp()
{
	for (int i = 0; i < actors.size(); i++)
	{
		actors[i]->set_status(false);
	}
	get_Player()->set_status(false);
	removeDeadGameObjects();
	decLives();
}

//this function loads the next level and creates the Actors required for this level
int StudentWorld::LoadLevel()
{
	string current_level = "";
	Level::LoadResult result = lev.loadLevel("");

	//makes sure that it stays within the vector of levels
	if (cur_lvl < levels.size())
	{
		current_level = levels[cur_lvl];
		result = lev.loadLevel(current_level);
	}


	if (cur_lvl == 0 && current_level == "level00.dat" && result == Level::load_fail_file_not_found)
		return GWSTATUS_NO_FIRST_LEVEL; //first level not found
	else if (cur_lvl != 0 && current_level != "level00.dat" && result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON; //last level is completed
	else if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR; //level can't be loaded due to issues

	//if level is successfully loaded
	Level::MazeEntry item;
	if (result == Level::load_success)
	{
		
		//goes through each x,y coordinate and checks the content of the Level
		//spawns a certain Actor depending on what is required for the level
		for (int i = 0; i < VIEW_WIDTH; i++)
		{
			for (int j = 0; j < VIEW_HEIGHT; j++)
			{
				int x = i;
				int y = j;
				item = lev.getContentsOf(x, y);
				if (item == Level::player)
					player = new Player(x, y, this);
				if (item == Level::perma_brick)
					actors.push_back(new PermanentBrick(x, y, this));
				if (item == Level::destroyable_brick)
					actors.push_back(new DestroyableBrick(x, y, this));
				if (item == Level::exit)
					actors.push_back(new Exit(x, y, this));
					if (item == Level::simple_zumi)
						actors.push_back(new SimpleZumi(x, y, this));
					if (item == Level::complex_zumi)
						actors.push_back(new ComplexZumi(x, y, this));
			}
		}

		//retrieve the numerical options that the level offers
		//this is used for the Actors to determine many values
		m_bonus = lev.getOptionValue(optionLevelBonus);
		m_extra_lifetime = lev.getOptionValue(optionGoodieLifetimeInTicks);
		m_walk_thru_lifetime = lev.getOptionValue(optionGoodieLifetimeInTicks);
		m_walk_thru_ticks = lev.getOptionValue(optionWalkThruLifetimeTicks);
		m_inc_simul_lifetime = lev.getOptionValue(optionGoodieLifetimeInTicks);
		m_inc_simul_ticks = lev.getOptionValue(optionBoostedSprayerLifetimeTicks);
		m_num_sprayers = lev.getOptionValue(optionMaxBoostedSprayers);
		m_simple_zumi_ticks = lev.getOptionValue(optionTicksPerSimpleZumiMove);
		m_complex_zumi_ticks = lev.getOptionValue(optionTicksPerComplexZumiMove);
		m_complex_zumi_search_dist = lev.getOptionValue(optionComplexZumiSearchDistance);
		m_prob_goodie_overall = lev.getOptionValue(optionProbOfGoodieOverall);
		m_prob_goodie_extra_life = lev.getOptionValue(optionProbOfExtraLifeGoodie);
		m_prob_goodie_walk_thru = lev.getOptionValue(optionProbOfWalkThruGoodie);
		m_prob_goodie_simul_bugspray = lev.getOptionValue(optionProbOfMoreSprayersGoodie);

		return GWSTATUS_CONTINUE_GAME;
	}
}

//Update the Game Status Line
void StudentWorld::UpdateDisplayText()
{
	int score = getScore();
	int level = getLevel();
	int livesLeft = getLives();

	string s = ScoreBoard(score, level, livesLeft, m_bonus);

	//GameWorld function
	setGameStatText(s);
}

//create the ScoreBoard
//required #include <sstream> and #include <iomanip> to format the score in the required manner
string StudentWorld::ScoreBoard(int Score, int Lvl, int Lives, int Bonus)
{
	ostringstream oss;
	oss.fill('0');
	oss << "Score: " << setw(8) << Score;
	oss << "  ";
	oss << "Level: " << setw(2) << Lvl;
	oss << "  ";
	oss << "Lives: " << setw(3) << Lives;
	oss << "  ";

	oss.fill(' ');
	oss << "Bonus: " << setw(6) << Bonus;
	
	string s = oss.str();
	return s;
}

//Checks if player is dead
bool StudentWorld::PlayerIsNotDead() const
{
	return player->get_status();
}

//checks if actor is dead
bool StudentWorld::ActorIsNotDead(Actor& a) const
{
	return a.get_status();
}

//checks if player completed level
bool StudentWorld::isCurrentLevelCompleted()
{
	int temp_id;
	
	//goes through each actor
	for (int i = 0; i < actors.size(); i++)
	{
		temp_id = actors[i]->get_ActorID();
		//if the actor is Exit and it's visibility is true and player is occupying same square
		//then return true
		if (temp_id == IID_EXIT && actors[i]->get_Actor_vis() == true
			&& occupy_with_player(actors[i]->getX(), actors[i]->getY()) == true)
			return true;
	}
	return false;
}

//increases score by bonus
void StudentWorld::IncreaseScoreByBonus()
{
	increaseScore(m_bonus);
}

//gets rid of dead actors
void StudentWorld::removeDeadGameObjects()
{
	vector<Actor*>::iterator p;
	bool temp_stat;
	for (p = actors.begin(); p != actors.end(); )
	{
		temp_stat = (*p)->get_status();
		if (!temp_stat)
		{
			delete *p; //must call to delete the object p is referring to - prevents mem leak
			p = actors.erase(p); //p becomes invalid, reassign p
		}
		else
			++p; //skips to next one before continuing on
	}
	
	//if player is dead
	//delete what player pointer is pointing to
	//and set pointer to null
	if (!PlayerIsNotDead())
	{
		delete player;
		player = NULL;
	}
	
}

//decrements levelbonus
void StudentWorld::reduceLevelBonusByOne()
{
	if (m_bonus > 0)
		m_bonus--;
	else
		m_bonus = 0;
}

//checks if all Zumis are gone
bool StudentWorld::allZumiDead()
{
	int temp_id;

	//goes through all actors and if the actor is a Zumi (simple or complex)
	//checks its status
	for (int i = 0; i < actors.size(); i++)
	{
		temp_id = actors[i]->get_ActorID();
		if (temp_id == IID_SIMPLE_ZUMI || temp_id == IID_COMPLEX_ZUMI)
		{
			if (actors[i]->get_status() == true)
				return false;
		}
	}
	return true;
}

//reveals the exit for player to go to
void StudentWorld::revealExitinMaze()
{
	int temp_id;
	//checks if all zumis are dead, if so then reveals exit
	if (allZumiDead())
	{
		for (int i = 0; i < actors.size(); i++)
		{
			temp_id = actors[i]->get_ActorID();
			if (temp_id == IID_EXIT)
				actors[i]->set_Actor_vis(true);
		}
	}
}

//takes in two ints and returns the ID of the Actor in that x and y coordinate
int StudentWorld::getID_of_other_in_square(int x, int y)
{
	int c, r;

	for (int i = 0; i < actors.size(); i++)
	{
		c = actors[i]->getX();
		r = actors[i]->getY();

		if (x == c && y == r)
			return actors[i]->get_ActorID();
	}
	return -123; //temp to show no other game object is in that square
}

//takes in two ints and sees if player's getX() and getY() are the same as those two
bool StudentWorld::occupy_with_player(int x, int y)
{
	//cout << "exit x = " << x << " exit y = " << y << endl;
	//cout << "player x = " << player->getX() << " player y = " << player->getY() << endl;

	return (x == player->getX() && y == player->getY());
}

//inserts an actor, takes in an Actor pointer
//created to allow specific actors to use in their respective classes
//example: zumi dies and spawns a goodie (able to do this in zumi's performAction() )
void StudentWorld::insert_Actor(Actor* a)
{
	actors.push_back(a);
}

//takes in three ints, and returns an Actor pointer
//used in derived classes of Actor so I can specify a particular Actor I want to look at
Actor* StudentWorld::get_Actor(int id, int x, int y)
{
	int temp_id, temp_x, temp_y;

	for (int i = 0; i < actors.size(); i++)
	{
		temp_id = actors[i]->get_ActorID();
		temp_x = actors[i]->getX();
		temp_y = actors[i]->getY();
		if (temp_id == id && temp_x == x && temp_y == y)
			return actors[i];
	}
	return NULL;
}

//takes in an int
//uses argument to return how many instances of a particular actor with that ID are currently alive
//used primarily to count bugsprayer amount
int StudentWorld::how_many_exist(int id)
{
	int count = 0;
	for (int i = 0; i < actors.size(); i++)
		if (actors[i]->get_ActorID() == id)
			count++;
	return count;
}