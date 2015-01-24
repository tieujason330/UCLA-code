#include <queue>
#include <iostream>
#include <string>
using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols,
	int sr, int sc, int er, int ec);

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X..X...XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;

}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	/*
	push the starting coordinate(sr, sc) onto the coordinate stack and
	update maze[sr][sc] to indicate that the algorithm has encountered
	it(i.e., set maze[sr][sc] to have a value other than '.').
	While the stack is not empty,
	{ Pop the top coordinate off the stack.This gives you the current
	(r, c) location that your algorithm is exploring.
	If the current(r, c) coordinate is equal to the ending coordinate,
	then we've solved the maze so return true!
	Check each place you can move from the current cell as follows :
	If you can move NORTH and haven't encountered that cell yet,
	then push the coordinate(r - 1, c) onto the stack and update
	maze[r - 1][c] to indicate the algorithm has encountered it.
	If you can move EAST and haven't encountered that cell yet,
	then push the coordinate(r, c + 1) onto the stack and update
	maze[r][c + 1] to indicate the algorithm has encountered it.
	If you can move SOUTH and haven't encountered that cell yet,
	then push the coordinate(r + 1, c) onto the stack and update
	maze[r + 1][c] to indicate the algorithm has encountered it.
	If you can move WEST and haven't encountered that cell yet,
	then push the coordinate(r, c - 1) onto the stack and update
	maze[r][c - 1] to indicate the algorithm has encountered it.
	}
	There was no solution, so return false
	*/
	queue<Coord> coordStack;
	Coord a(sr, sc);
	coordStack.push(a);
	maze[sr][sc] = 'O';

	while (!coordStack.empty())
	{
		Coord b = coordStack.front();
		int nr = b.r();
		int nc = b.c();
		//cout << nr << "," << nc << endl;
		coordStack.pop();
		if (nr == er && nc == ec)
			return true;

		//if can move NORTH
		if ((nr - 1) > 0 && maze[nr - 1][nc] == '.')
		{
			Coord c(nr - 1, nc);
			coordStack.push(c);
			maze[nr - 1][nc] = 'O';
		}
		//if can move EAST
		if (((nc + 1) <= (nCols - 1)) && maze[nr][nc + 1] == '.')
		{
			Coord d(nr, nc + 1);
			coordStack.push(d);
			maze[nr][nc + 1] = 'O';
		}
		//if can move SOUTH
		if (((nr + 1) <= (nRows - 1)) && maze[nr + 1][nc] == '.')
		{
			Coord e(nr + 1, nc);
			coordStack.push(e);
			maze[nr + 1][nc] = 'O';
		}
		//if can move WEST
		if ((nc - 1) > 0 && maze[nr][nc - 1] == '.')
		{
			Coord f(nr, nc - 1);
			coordStack.push(f);
			maze[nr][nc - 1] = 'O';
		}
	}
	return false;
}
