#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <vector>
using namespace std;

struct Square
{
	int id;
	int x;
	int y;
	Square()
	{
		id = -1;
		x = 0;
		y = 0;
	}
};

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, StudentWorld& s) : GraphObject(imageID, startX, startY) 
	{ 
		m_gw = &s;
		m_sw = &s;
	}
	void set_status(bool s) { m_status = s; }
	bool get_status() { return m_status; }
	virtual int performAction() = 0;
	void set_square_occ_status(int imageID ,int x, int y);
	const vector<Square>& get_squares() { return m_squares; }
	GameWorld* getWorld() { return m_gw; }
	StudentWorld* get_SWorld() { return m_sw;  }
private:
	GameWorld* m_gw;
	StudentWorld* m_sw;
	bool m_status;
	vector<Square> m_squares;
};

class Player : public Actor
{
public:
	Player(int startX, int startY, StudentWorld* s);
	virtual int performAction();
	void gained_Pickup();
	void set_walk_thru_bricks(bool check) { walk_through_bricks = check; }
	bool get_walk_thru_bricks_status() const { return walk_through_bricks; }
	void set_extra_bug_sprayers(bool check) { extra_bug_sprayers = check; }
	bool get_extra_bug_sprayers() const { return extra_bug_sprayers; }
private:
	int available_bug_sprayers;
	bool walk_through_bricks;
	bool extra_bug_sprayers;
};

class DestroyableBrick : public Actor
{
public:
	DestroyableBrick(int startX, int startY, StudentWorld* s);
	virtual int performAction() { return GWSTATUS_CONTINUE_GAME; }
};

class PermanentBrick : public Actor
{
public:
	PermanentBrick(int startX, int startY, StudentWorld* s);
	virtual int performAction() { return GWSTATUS_CONTINUE_GAME; }
};

class Zumi : public Actor
{};

class SimpleZumi : public Zumi
{};

class ComplexZumi : public Zumi
{};




#endif // ACTOR_H_
