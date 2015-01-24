#include "Arena.h"
#include "Player.h"
#include "Zombie.h"
#include "History.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

Arena::Arena(int nRows, int nCols)
	: m_rows(nRows), m_cols(nCols), m_player(nullptr), m_nZombies(0), m_turns(0), m_hist(nRows, nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }

    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    for (int k = 0; k < m_nZombies; k++)
        delete m_zombies[k];
    delete m_player;
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::zombieCount() const
{
    return m_nZombies;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c);
    return m_grid[r-1][c-1];
}

int Arena::numberOfZombiesAt(int r, int c) const
{
    int count = 0;
    for (int k = 0; k < m_nZombies; k++)
    {
        Zombie* zp = m_zombies[k];
        if (zp->row() == r  &&  zp->col() == c)
            count++;
    }
    return count;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (brains)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');

        // Indicate each zombie's position
    for (int k = 0; k < m_nZombies; k++)
    {
        const Zombie* zp = m_zombies[k];
        char& gridChar = displayGrid[zp->row()-1][zp->col()-1];
        switch (gridChar)
        {
          case '.':  gridChar = 'Z'; break;
          case 'Z':  gridChar = '2'; break;
          case '9':  break;
          default:   gridChar++; break;  // '2' through '8'
        }
    }

      // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

      // Write message, zombie, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << zombieCount() << " zombies remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c);
    m_grid[r-1][c-1] = status;
}

bool Arena::addZombie(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    if (m_nZombies == MAXZOMBIES)
        return false;
    m_zombies[m_nZombies] = new Zombie(this, r, c);
    m_nZombies++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (m_player != nullptr  ||  ! isPosInBounds(r, c))
        return false;
    if (numberOfZombiesAt(r, c) > 0)
        return false;
    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveZombies()
{
      // Move all zombies
    for (int k = m_nZombies-1; k >= 0; k--)
    {
        Zombie* zp = m_zombies[k];
        zp->move();

        if (m_player != nullptr  &&
                zp->row() == m_player->row()  &&  zp->col() == m_player->col())
            m_player->setDead();

        if (zp->isDead())
        {
			//*******************************************
			//record zombie death BEFORE deleting zombie
			history().record(zp->row(), zp->col());
			//*******************************************

            delete zp;
              // The order of Zombie pointers in the m_zombies array is
              // irrelevant, so it's easiest to move the last pointer to
              // replace the one pointing to the now-deleted zombie.  Since
              // we are traversing the array from last to first, we know this
              // last pointer does not point to a dead zombie.

            m_zombies[k] = m_zombies[m_nZombies-1];
            m_nZombies--;
        }
    }

      // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c) const
{
    if (!isPosInBounds(r, c))
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ")" << endl;
        exit(1);
    }
}
 // Return false without changing anything if moving one step from (r,c)
  // in the indicated direction would hit a run off the edge of the arena.
  // Otherwise, update r and c to the position resulting from the move and
  // return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    int rnew = r;
    int cnew = c;
    switch (dir)
    {
      case NORTH:  if (r <= 1)        return false; else rnew--; break;
      case EAST:   if (c >= a.cols()) return false; else cnew++; break;
      case SOUTH:  if (r >= a.rows()) return false; else rnew++; break;
      case WEST:   if (c <= 1)        return false; else cnew--; break;
    }
    r = rnew;
    c = cnew;
    return true;
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should drop a brain and not move;
  // otherwise, this function sets bestDir to the recommended direction
  // to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
      // How dangerous is it to stand?
    int standDanger = computeDanger(a, r, c);

      // if it's not safe, see if moving is safer
    if (standDanger > 0)
    {
        int bestMoveDanger = standDanger;
        int bestMoveDir = NORTH;  // arbitrary initialization

          // check the four directions to see if any move is
          // better than standing, and if so, record the best
        for (int dir = 0; dir < NUMDIRS; dir++)
        {
            int rnew = r;
            int cnew = c;
            if (attemptMove(a, dir, rnew, cnew))
            {
                int danger = computeDanger(a, rnew, cnew);
                if (danger < bestMoveDanger)
                {
                    bestMoveDanger = danger;
                    bestMoveDir = dir;
                }
            }
        }

          // if moving is better than standing, recommend move
        if (bestMoveDanger < standDanger)
        {
            bestDir = bestMoveDir;
            return true;
        }
    }
    return false;  // recommend standing
}

int computeDanger(const Arena& a, int r, int c)
{
      // Our measure of danger will be the number of zombies that might move
      // to position r,c.  If a zombie is at that position, it is fatal,
      // so a large value is returned.

    if (a.numberOfZombiesAt(r,c) > 0)
        return MAXZOMBIES+1;

    int danger = 0;
    if (r > 1)
        danger += a.numberOfZombiesAt(r-1,c);
    if (r < a.rows())
        danger += a.numberOfZombiesAt(r+1,c);
    if (c > 1)
        danger += a.numberOfZombiesAt(r,c-1);
    if (c < a.cols())
        danger += a.numberOfZombiesAt(r,c+1);

    return danger;
}

//added for History feature
History& Arena::history()
{
	return m_hist;
}