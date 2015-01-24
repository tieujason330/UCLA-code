#ifndef HISTORY_INCLUDED
#define HISTORY_INCLUDED

#include "globals.h"

class Arena;

class History
{
	public:
		History(int nRows, int nCols);
		bool record(int r, int c);
		void display() const;
	private:
		int hist_rows;
		int hist_cols;
		int hist_arena[MAXROWS][MAXCOLS];
};

#endif