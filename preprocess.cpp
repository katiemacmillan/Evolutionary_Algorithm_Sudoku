 /************************************************************************
   File: preprocess.cpp
   Authors: Katie MacMillian, Jake Davidson
   Description: The file contains functions for pre-processing the puzzle. 
   We fill naked and hidden singles before sending the puzzle off to the 
   genetic algorithm.
 ************************************************************************/
#include "preprocess.h"

/******************************************************************************
 * fillNakeSingles()
 *
 * This function goes through each square of the initial puzzle and evaluates
 * the possible values. If only one value exists that could go in a given
 * square, that is the value set to the square.
 *
 * This process will happen repeatedly for all squares in the puzzle until
 * all squares have been evaluated without a single being filled in.
 *
 * returns: true if at least one naked single was filled in
 *****************************************************************************/
bool fillNakedSingles()
{
	// the value returned to the calling function to indicate whether a single was filled in this function execution
	bool found = false;
	bool filled;
	
	do
	{
		filled = false;
		initial_puzzle.evaluatePossibleValues();
		
		// for each square get the possible values it could be
		for(int i = 0; i < (const_data.DIM * const_data.DIM); i++)
		{
			// only check "blank" values
			if(initial_puzzle[i].getValue() == '-')
			{
				// fill in value if only one possible value exists
				if (initial_puzzle[i].getPossibleCt() == 1)
				{
					initial_puzzle[i].setSingle();
					filled = true;
					found = true;
				}
			}
		}
	// repeat naked single scan until a scan occurs where no changes are made
	} while (filled);

	return found;
}

/******************************************************************************
 * fillHiddenSingles()
 *
 * This function looks at each row of the initial puzzle, then each column,
 * then each subgrid. For each one it determines which values are left to be
 * filled in, and how many position options each value has. If a value only
 * has one position option, that value is set to the square in that position.
 *
 * This process of row, column and subgrid evaluation will happen repeatedly
 * until all have been examined at least once without any singles being filled
 * in.
 *
 * returns: true if at least one hidden single was filled in
 *****************************************************************************/
bool fillHiddenSingles()
{
	// the value returned to the calling function to indicate whether a single was filled in this function execution
	bool found = false;
	bool filled;

	do
	{
		filled = false;
		initial_puzzle.evaluatePossibleValues();

		// for each possible value (1-9)
		for (int v = 0; v < const_data.DIM; v++){
			char value = const_data.VALUE_SET[v];

			// look at each row
			for (int r = 0; r < const_data.DIM; r++)
			{
				// get the position the value can go in in the row
				int pos = findSinglePosition(initial_puzzle.getRow(r), value);

				if ((pos != -1) && (pos != -2))
				{
					// find position of square in linear puzzle vector
					pos += (r * const_data.DIM);
					initial_puzzle[pos].setSingle(value);
					filled = true;
					found = true;
				}
			}

			// look at each column
			for (int c = 0; c < const_data.DIM; c++)
			{
				// get the position the value can go in in the column
				int pos = findSinglePosition(initial_puzzle.getCol(c), value);

				if ((pos != -1) && (pos != -2))
				{
					// find position of square in linear puzzle vector
					pos = (pos * const_data.DIM) + c;
					initial_puzzle[pos].setSingle(value);
					filled = true;
					found = true;
				}
			}

			// look at each subgrid
			for (int r = 0; r < const_data.DIM; r += const_data.SUBDIM)
			{
				for (int c = 0; c < const_data.DIM; c += const_data.SUBDIM)
				{
					// get the position the value can go in in the subgrid
					int pos = findSinglePosition(initial_puzzle.getSub(r, c), value);

					if ((pos != -1) && (pos != -2))
					{
						// find position of square in linear puzzle vector
						pos = (((pos / const_data.SUBDIM) + r) *const_data. DIM) + (pos % const_data.SUBDIM) + c;
						initial_puzzle[pos].setSingle(value);
						filled = true;
						found = true;
					}
				}
			}
		}
	// repeat hidden single scan until a scan occurs where no changes are made
	} while (filled);

	return found;
}

/******************************************************************************
 * findSinglePosition(vector<square>, char)
 *
 * This function evaluates a vector of squares to determine how many positions
 * within the vector a given character can go. Each square within the vector
 * is checked to see if the character is a possible value of the square. If
 * so, the found flag is set. If a position is found where the value can go
 * when the found flag is already set, the position for the value is set to
 * -2, which breaks the check loop, and -2 is returned. This will indicate
 * that there are multiple positions the value can go in.
 *
 * params:
 *		squares - a vector of squares representing either a row, column, or
 *				  subgrid within a puzzle
 *		  value - a specific character value to count positions for
 *
 * returns: -2 if the character value has more than one possible position
 *			the only position a character value can go in
 *****************************************************************************/
int findSinglePosition(vector<square> squares, char value)
{
	// track the first square index it can be in
	int pos = -1;
	// track whether the value can go in a previous square
	bool found = false;
	// index into squares vector
	int s = 0;

	do{
	// if the square does not have a number
	if(squares[s].getValue() == '-')
	{
		// check if the value is a possible solution for the square
		if(squares[s].isPossibleValue(value))
		{
			// check if the value is already valid elsewhere in the squares
			if(found)
			{
				// if so, break
				pos = -2;
			}
			else
			{
				// else set pos to the square indedx and found flag to true
				found = true;
				pos = s;
			}
		}
		// if square holds value we're checking, cause loop to stop
		} else if (squares[s].getValue() == value){
			pos = -2;
		}

		s++;
	} while ((pos!= -2) && (s < const_data.DIM));

	return pos;
}