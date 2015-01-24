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

//If the start location is equal to the ending location, then we've
//	solved the maze, so return true.
//Mark the start location as visted.
//For each of the four directions,
//	If the location one step in that direction(from the start
//	location) is unvisited,
//		then call pathExists starting from that location(and
//		ending at the same ending location as in the
//		current call).
//			If that returned true,
//				then return true.
//Return false.
bool pathExists(string maze[], int nRows, int nCols,
	int sr, int sc, int er, int ec);

//the function takes in an array of strings, and ints representing the number of rows,
//number of columns, starting row, starting column, ending row, and ending column
//Using recursion to see if the path exists or not by going through each element
//around the starting element and seeing if there is a path that connects to the ending goal
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	//If the start location is equal to the ending location, then we've
		//solved the maze, so return true.
	if (sr == er && sc == ec)
		return true;
	//Mark the start location as visted.
	maze[sr][sc] = 'O';
	int nr = sr;
	int nc = sc;

	//For each of the four directions,
		//If the location one step in that direction(from the start
			//location) is unvisited,
				//then call pathExists starting from that location(and
						//ending at the same ending location as in the
						//current call).
					//If that returned true,
						//then return true.
	
	//If you can move North and haven't encountered that cell yet,
	if ((nr - 1) > 0 && maze[nr - 1][nc] == '.')
	{
		if (pathExists(maze, nRows, nCols, nr-1, nc, er, ec))
			return true;
	}
	//If you can move EAST and haven't encountered that cell yet,
	if (((nc + 1) <= (nCols - 1)) && maze[nr][nc + 1] == '.')
	{
		if (pathExists(maze, nRows, nCols, nr, nc+1, er, ec))
			return true;
	}
	//If you can move SOUTH and haven't encountered that cell yet,
	if (((nr + 1) <= (nRows - 1)) && maze[nr + 1][nc] == '.')
	{
		if (pathExists(maze, nRows, nCols, nr+1, nc, er, ec))
			return true;
	}
	//If you can move WEST and haven't encountered that cell yet,
	if ((nc - 1) > 0 && maze[nr][nc - 1] == '.')
	{
		if (pathExists(maze, nRows, nCols, nr, nc-1, er, ec))
			return true;
	}
	
	return false;
}
