#ifndef ARENA_INCLUDED
#define ARENA_INCLUDED

#include "globals.h"
#include "History.h"
#include <string>

using namespace std;

class Zombie;
class Player;

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     zombieCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfZombiesAt(int r, int c) const;
    void    display(string msg) const;
	//Added for History feature
	History& history();

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addZombie(int r, int c);
    bool addPlayer(int r, int c);
    void moveZombies();

  private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Zombie* m_zombies[MAXZOMBIES];
    int     m_nZombies;
    int     m_turns;
	//Added for History feature
	History m_hist;

      // Helper functions
    void checkPos(int r, int c) const;
    bool isPosInBounds(int r, int c) const;
};

bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
int computeDanger(const Arena& a, int r, int c);



#endif ARENA_INCLUDED