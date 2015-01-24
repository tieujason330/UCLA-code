#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <vector>
#include <queue>
#include <iostream>
using namespace std;

struct Coord
{
	int x;
	int y;
	bool disc = bool();
};

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	//Constructor takes in ID, x and y values, and pointer to a StudentWorld object
	Actor(int imageID, int startX, int startY, StudentWorld* s);

	//sets status for actor, takes in a bool
	//status determines whether actor is alive or not
	void set_status(bool s) { m_status = s; }

	//retrieves status 
	bool get_status() const { return m_status; }

	//pure abstract function for all derived classes to implement
	virtual int performAction() = 0;

	//gets a pointer to StudentWorld
	StudentWorld* get_SWorld() const { return m_sw; }

	//gets ID of an Actor
	int get_ActorID() const { return m_ID; }
	
	//sets Actor's visibility
	void set_Actor_vis(bool t) { setVisible(t);  m_vis = t; }
	
	//gets Actor's visibility
	bool get_Actor_vis() const{ return m_vis; }
private:
	int m_ID;
	StudentWorld* m_sw;
	bool m_status;
	bool m_vis;
};

class Player : public Actor
{
public:
	//Player constructor, takes in x and y coordinates and pointer to StudentWorld object
	Player(int startX, int startY, StudentWorld* s);
	//virtual performAction() function determins what the player does
	virtual int performAction();
	
	//sets max_bug_sprayers, takes in an int
	void set_max_bug_sprayers(int t) { max_bug_sprayers = t; }
	//getes max bug sprayers
	int get_max_bug_sprayers() { return max_bug_sprayers; }
	//sets available bug sprayers
	void set_avail_bug_sprayers(int t) { available_bug_sprayers = t; }
	//gets available bug sprayers
	int get_avail_bug_sprayers() { return available_bug_sprayers; }
	//increases available bug sprayers by one
	void inc_avail_bug_sprayers_by_one() { available_bug_sprayers++; }
	//gets available bug sprayers
	int get_avail_bug_sprayers() const { return available_bug_sprayers; }
	//sets T/F for walk through bricks, takes in a bool
	void set_walk_thru_bricks(bool check) { walk_through_bricks = check; }
	//gets status for walk through bricks
	bool get_walk_thru_bricks_status() const { return walk_through_bricks; }
	//sets extra bug sprayers T/F, takes in a bool
	void set_extra_bug_sprayers(bool check) { extra_bug_sprayers = check; }
	//gets status for extra bug sprayers
	bool get_extra_bug_sprayers() const { return extra_bug_sprayers; }
	//sets walk through ticks, takes in an int
	void set_walk_thru_ticks(int check) { walk_thru_ticks = check; }
	//setes extra sprayers ticks, takes in an int
	void set_extra_bug_sprayers_ticks(int check) { extra_bug_sprayer_ticks = check; }

	int get_deployed_bombs() { return deployed_bombs; }
	void dec_deployed_bombs_by_one() { deployed_bombs--; }
	void set_deployed_bombs(int t) { deployed_bombs = t; }
private:
	int max_bug_sprayers;
	int available_bug_sprayers;
	int deployed_bombs;
	bool walk_through_bricks;
	int walk_thru_ticks;
	bool extra_bug_sprayers;
	int extra_bug_sprayer_ticks;
};

class DestroyableBrick : public Actor
{
public:
	//Constructor, takes in an x and y coordinates, and pointer to StudentWorld object
	DestroyableBrick(int startX, int startY, StudentWorld* s);
	//performAction determines what a brick does
	virtual int performAction(); //{ return GWSTATUS_CONTINUE_GAME; }
};

class PermanentBrick : public Actor
{
public:
	//Constructor, takes in an x and y coordinates, and pointer to StudentWorld object
	PermanentBrick(int startX, int startY, StudentWorld* s);
	//performAction determines what a brick does
	virtual int performAction() { return GWSTATUS_CONTINUE_GAME; }
};

class Exit : public Actor
{
public:
	//Constructor, takes in an x and y coordinates, and pointer to StudentWorld object
	Exit(int startX, int startY, StudentWorld *s);
	//performAction determines what an exit does
	virtual int performAction();
	//gets status of sound for exit
	bool get_exit_sound() { return m_exit_sound; }
	//sets exit sound
	void set_exit_sound(bool t) { m_exit_sound = t; }
private:
	bool m_exit_sound;
	bool m_sound_finished_lvl;
};

class BugSprayer : public Actor
{
public:
	//Constructor, takes in an x and y coordinates, and pointer to StudentWorld object
	BugSprayer(int startX, int startY, StudentWorld *s);
	//performAction determines what it'll do during each tick
	virtual int performAction();
	void set_sprayer_lifetime(int t) { m_sprayer_lifetime = t; }
	int get_sprayer_lifetime() { return m_sprayer_lifetime; }
private:
	int m_sprayer_lifetime;
};

class BugSpray : public Actor
{
public:
	//Constructor, takes in an x and y coordinates, and pointer to StudentWorld object
	BugSpray(int startX, int startY, StudentWorld *s);
	//performAction determines what it'll do during each tick
	virtual int performAction();
	//helps to determine which goodie type will spawn when a Zumi is killed
	void determine_Goodie_type();
	//spawns a goodie depending on int x (compares to values of probability for each goodie)
	void spawn_goodie(int x);//spawns goodie depending on what x is 
private:
	int m_spray_lifetime;
};

class ExtraLifeGoodie : public Actor
{
public:
	//Constructor, takes in an x and y coordinates, and pointer to StudentWorld object
	ExtraLifeGoodie(int startX, int startY, StudentWorld* s);
	//performAction determines what it'll do during each tick
	virtual int performAction();
private:
	int m_timer;
};

class WalkThruWallsGoodie : public Actor
{
public:
	//Constructor, takes in an x and y coordinates, and pointer to StudentWorld object
	WalkThruWallsGoodie(int startX, int startY, StudentWorld* s);
	//performAction determines what it'll do during each tick
	virtual int performAction();
private:
	int m_timer;
};

class IncSimulSprayerGoodie : public Actor
{
public:
	//Constructor, takes in an x and y coordinates, and pointer to StudentWorld object
	IncSimulSprayerGoodie(int startX, int startY, StudentWorld* s);
	//performAction determines what it'll do during each tick
	virtual int performAction();
private:
	int m_timer;
};

class SimpleZumi : public Actor
{
public:
	//Constructor, takes in an x and y coordinates, and pointer to StudentWorld object
	SimpleZumi(int startX, int startY, StudentWorld* s);
	//performAction determines what it'll do during each tick
	virtual int performAction();
private:
	int m_currentDirection; // 1 (up), 2 (down), 3 (left), 4 (right)
	int m_move_ticks;
};

class ComplexZumi : public Actor
{
public:
	//Constructor, takes in an x and y coordinates, and pointer to StudentWorld object
	ComplexZumi(int startX, int startY, StudentWorld* s);
	//performAction determines what it'll do during each tick
	virtual int performAction();
	//sets if ComplexZumi can smell player, takes in a bool
	void set_smell(bool t) { m_smell_Player = t; }
	//gets value of m_smell_Player
	bool get_smell() { return m_smell_Player; }

	//unused functions

	//breadth-first search
	//takes in starting x and y and ending x and y
	bool path_found(int sX, int sY, int eX, int eY);

	//gets a specific cCord from a queue
	//takes in a queue and x and y to match for the Coord
	Coord& get_Coord(queue<Coord>& g ,int x, int y);

	//sets the bool disc of a coord to argument z
	//takes in a queue, Coord, and bool
	void set_Coord_disc(queue<Coord>& g, Coord t, bool z);

	//gets the bool disc of a coord 
	//takes in a queue and a Coord
	bool get_disc(queue<Coord> g, Coord t);

private:
	int m_currentDirection; // 1 (up), 2 (down), 3 (left), 4 (right)
	int m_move_ticks;
	int m_search_dist;
	bool m_smell_Player;
	bool path_exists;
	queue<Coord> m_p;
	queue<Coord> level;
	bool search;
};

#endif // ACTOR_H_
