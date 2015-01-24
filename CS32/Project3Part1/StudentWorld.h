#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"
//#include "Actor.h"
#include <vector>
#include <string>
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Player;

class StudentWorld : public GameWorld
{
public:

	StudentWorld();

	~StudentWorld();

	virtual int init()
    {
		LoadLevel();
		return GWSTATUS_CONTINUE_GAME;
    }

	virtual int move();

	virtual void cleanUp()
    {
		actors.clear();
		delete player; //freed memory
		player = NULL; //pointed dangling ptr to NULL
    }

	int LoadLevel();

	//creates a StudentWorld
	GameWorld* createStudentWorld();

	//Update the Game Status Line
	void UpdateDisplayText();

	string ScoreBoard(int Score, int Lvl, int Lives, int Bonus);

	//Checks if player is dead
	bool PlayerIsDead() const;

	//checks if actor is dead
	bool ActorIsDead(Actor& a) const;

	//checks if player completed level
	bool isCurrentlLevelCompleted() const;

	//increases score by bonus
	void IncreaseScoreByBonus();

	//gets rid of dead actors
	void removeDeadGameObjects();

	//decrements levelbonus
	void reduceLevelBonusByOne();

	//checks if all Zumis are gone
	bool allZumiDead();

	//reveals the exit for player to go to
	void revealExitinMaze();

	//sees if square of level is occupied by an object
	//to be used primarily by player
	bool squareIsOccupied(int x, int y);

	//make a function that returns ID of object
	int getID_of_square(int x, int y);

private:
	vector<Actor*> actors;
	Player* player;
	list<string> levels;
	Level lev;
	unsigned int m_bonus;
};

#endif // STUDENTWORLD_H_
