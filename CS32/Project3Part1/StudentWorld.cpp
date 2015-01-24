#include "StudentWorld.h"
#include "Actor.h"
#include <list>
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
}

StudentWorld::~StudentWorld()
{
	actors.clear();
	levels.clear();

	delete player; //freed memory
	player = NULL;//pointed dangling ptr to NULL
}

int StudentWorld::move()
{
	UpdateDisplayText();
	
	vector<Actor*>::iterator p;
	for (p = actors.begin(); p != actors.end(); p++)
	{
		if ( !ActorIsDead(*(*p)) )
			(*p)->performAction();
		player->performAction();
		//if (PlayerIsDead())
			//return GWSTATUS_PLAYER_DIED;
		//if (isCurrentlLevelCompleted())
		{
		//	IncreaseScoreByBonus();
		//	levels.pop_front();
		//	return GWSTATUS_FINISHED_LEVEL;
		}
	}

	//removeDeadGameObjects();
	reduceLevelBonusByOne();

	//if (allZumiDead())
		//revealExitinMaze();

	//if (PlayerIsDead())
	//	return GWSTATUS_PLAYER_DIED;

	//if (isCurrentlLevelCompleted())
	{
	//	IncreaseScoreByBonus();
		//levels.pop_front();
	//	return GWSTATUS_FINISHED_LEVEL;
	}	

	return GWSTATUS_CONTINUE_GAME;
	
}

int StudentWorld::LoadLevel()
{
	string current_level = levels.front();
	Level::LoadResult result = lev.loadLevel(current_level);

	if (current_level == "level00.dat" && result == Level::load_fail_file_not_found)
		return GWSTATUS_NO_FIRST_LEVEL; //first level not found
	else if (current_level != "level00.dat" && result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
	else if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;

	Level::MazeEntry item;

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
		}
	}

	m_bonus = lev.getOptionValue(optionLevelBonus);
	//add rest later****************************


	return GWSTATUS_CONTINUE_GAME;
}

//Update the Game Status Line
void StudentWorld::UpdateDisplayText()
{
	int score = getScore();
	int level = getLevel();
	int livesLeft = getLives();

	string s = ScoreBoard(score, level, livesLeft, m_bonus);

	setGameStatText(s);
}

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
bool StudentWorld::PlayerIsDead() const
{
	if (player->get_status())
		return true;
	else
		return false;
}

//checks if actor is dead
bool StudentWorld::ActorIsDead(Actor& a) const
{
	if (a.get_status())
		return true;
	else
		return false;
}

//checks if player completed level
bool StudentWorld::isCurrentlLevelCompleted() const
{
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
	for (p = actors.begin(); p != actors.end(); p++)
		if (!(*p)->get_status())
			p = actors.erase(p);
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
	vector<Actor*>::iterator p;
	for (p = actors.begin(); p != actors.end(); p++)
	{
		if ((*p)->get_status())
			return false;
	}
	return true;
}

//reveals the exit for player to go to
void revealExitinMaze()
{
	//sets exit isVisible to true
}

bool StudentWorld::squareIsOccupied(int x, int y)
{
	Level::MazeEntry item;

	item = lev.getContentsOf(x, y);
	if (item == Level::perma_brick)
		return true;
	if (item == Level::destroyable_brick && player->get_walk_thru_bricks_status() == false)
		return true;
	else if (item == Level::destroyable_brick && player->get_walk_thru_bricks_status() == true)
		return false;
	else
		return false;
}

//make a function that returns the ID of object
int StudentWorld::getID_of_square(int x, int y)
{
	Level::MazeEntry item;
	item = lev.getContentsOf(x, y);
	if (item == Level::perma_brick)
		return IID_PERMA_BRICK;
	if (item == Level::destroyable_brick)
		return IID_DESTROYABLE_BRICK;
	else
		return -123; //if item is not one of the featured objects
}