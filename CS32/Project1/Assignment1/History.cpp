#include"History.h"
#include"globals.h"
#include"Arena.h"
#include<iostream>
#include<cstdlib>

using namespace std;

History::History(int nRows, int nCols)
	:hist_rows(nRows), hist_cols(nCols)
{
	for(int r = 0; r < MAXROWS; r++)
	{
		for(int c = 0; c < MAXCOLS; c++)
		{
			hist_arena[r][c] = 0;
		}
	}
}

bool History::record(int r, int c)
{
	if(r > hist_rows || c > hist_cols)
		return false;
	hist_arena[r-1][c-1]++; //increments history by one at that spot
	return true;
}

void History::display() const
{
	clearScreen();

	for(int r = 0; r < hist_rows; r++)
	{
		for(int c = 0; c < hist_cols; c++)
		{
			int x = hist_arena[r][c];
			char letter = 'A';
			if(x == 0)
			{
				cout << '.';
			}
			//displays A..Z depending on how many dead Zombies in that spot
			else if (x == 1)
			{
				cout << static_cast<char>(letter);
			}
			else if(x > 1 && x < 26)
			{
				cout << static_cast<char>(letter+(x-1));
			}
			else if (x >=26)
				cout << 'Z';
			}

			cout << endl;
	}
	cout << endl;
}
	
