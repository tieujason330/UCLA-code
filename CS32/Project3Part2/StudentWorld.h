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
class Exit;

class StudentWorld : public GameWorld
{
public:

	//Constructor
	StudentWorld();

	//Destructor
	~StudentWorld();

	virtual int init()
    {
		int x = LoadLevel();
		if (x == GWSTATUS_PLAYER_WON)//closes program when player finishes last level
			exit(EXIT_SUCCESS);
		else
			return GWSTATUS_CONTINUE_GAME;
    }

	virtual int move();

	virtual void cleanUp();
   
	//loads level
	int LoadLevel();

	//creates a StudentWorld
	GameWorld* createStudentWorld();

	//Update the Game Status Line
	void UpdateDisplayText();

	string ScoreBoard(int Score, int Lvl, int Lives, int Bonus);

	//Checks if player is dead
	bool PlayerIsNotDead() const;

	//checks if actor is dead
	bool ActorIsNotDead(Actor& a) const;

	//checks if player completed level
	bool isCurrentLevelCompleted();

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

	//make a function that returns ID of object
	//EXCLUDES PLAYER
	int getID_of_other_in_square(int x, int y);

	//checks to see if object calling this function is occupying with player
	bool occupy_with_player(int x, int y);

	//inserts an actor into the member data (vector), takes an Actor* as an argument
	//created to be used be accessed derived classes of Actor
	void insert_Actor(Actor* a);

	//retrieves player pointer for easier access by other Actors
	Player* get_Player() { return player; }

	//gets actors based on their id and coordinates
	//useful for outside the StudentWorld class
	Actor* get_Actor(int id, int x, int y);

	//retrieves extra lifetime value from level data
	int get_extra_lifetime() { return m_extra_lifetime; }

	//checks if how many Actors with the id exists currently
	int how_many_exist(int id);

	/////the functions below are get-functions for access by Actors

	//retrieves walk through lifetime value
	int get_walk_thru_lifetime() { return m_walk_thru_lifetime; }

	//retrieves walk through ticks value
	int get_walk_thru_ticks() { return m_walk_thru_ticks; }

	//retrieves simultaneous bugsprayers lifetime
	int get_inc_simul_lifetime() { return m_inc_simul_lifetime; }

	//retrieves simultaneous bugsprayers ticks
	int get_inc_simul_ticks() { return m_inc_simul_ticks; }

	//retrieves number of sprayers allowed
	int get_num_sprayers() { return m_num_sprayers; }

	//retrieves simple zumi movement ticks
	int get_simple_zumi_ticks() { return m_simple_zumi_ticks; }

	//retrieves complex zumi movement ticks
	int get_complex_zumi_ticks() { return m_complex_zumi_ticks; }

	//retrieves complex zumi search distance
	int get_complex_zumi_search_dist() { return m_complex_zumi_search_dist; }
	
	//retrieves probability for goodies overall
	int get_prob_goodie() { return m_prob_goodie_overall; }

	//retrieves prob for extra life goodie
	int get_prob_extra_life() { return m_prob_goodie_extra_life; }

	//retrieves prob for walk-through goodie
	int get_prob_walk_thru() { return m_prob_goodie_walk_thru; }

	//retrieves prob for more bugsprayers goodie
	int get_prob_simul_bugspray() { return m_prob_goodie_simul_bugspray; }

private:
	vector<Actor*> actors;//used vector to store Actor pointers
	Player* player;//had a separate pointer for Player
	vector<string> levels;//used list to store the levels
	Level lev;//made a level object to access level class
	int cur_lvl;
	unsigned int m_bonus;
	unsigned int m_extra_lifetime;
	unsigned int m_walk_thru_lifetime;
	unsigned int m_walk_thru_ticks;
	unsigned int m_inc_simul_lifetime;
	unsigned int m_inc_simul_ticks;
	unsigned int m_num_sprayers;
	unsigned int m_simple_zumi_ticks;
	unsigned int m_complex_zumi_ticks;
	unsigned int m_complex_zumi_search_dist;
	unsigned int m_prob_goodie_overall;
	unsigned int m_prob_goodie_extra_life;
	unsigned int m_prob_goodie_walk_thru;
	unsigned int m_prob_goodie_simul_bugspray;
};

#endif // STUDENTWORLD_H_
