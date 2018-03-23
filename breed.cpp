 /************************************************************************
   File: breed.cpp
   Authors: Katie MacMillian, Jake Davidson
   Description: This file contains functions involved with breeding members
   of the population. This file assumes it has global access to the population 
   and breeding pool.
 ************************************************************************/
#include "breed.h"

/******************************************************************************
 * breed()
 *
 * This function selects the designated number of breeders from the parent
 * population and uses them to breed a child generation. This breeder pool is
 * then randomized.
 *
 * If the elitism flag is not set, or the breeder pool is larger than half the
 * population size, then the iteration limit for the breeder pool is set to
 * half the population size.
 *
 * The function then iterates through the breeder population, breeding pairs
 * until either all have been bred, or until the population in generation is
 * full.
 *
 * If the elitism flag is set to true, then the remaining population is
 * filled with members of the parent population.
 *****************************************************************************/
void breed()
{
	// select top portion of population for breeding
	for (int i = 0; i < const_data.BREEDERS; i++){
		breeding_pool[i] = population[i];
	}

	// shuffle breeders
	random_shuffle(breeding_pool.begin(), breeding_pool.end());

	/* max iterations should not exceed half the population size and should be
	 * half the population size if elitism flag is not set
	 */
	int i_max = const_data.BREEDERS;
	if ((i_max > const_data.POP / 2) || !const_data.ELITISM_FLAG)
	{
		i_max = const_data.POP / 2;
	}
	// index 0 is a special case, it gets bred with last breeder
	int pos = 0;
	createNextGeneration(breeding_pool[0], breeding_pool[i_max-1]);
	new_generation[pos++] = breeding_pool[0];
	new_generation[pos++] = breeding_pool[i_max-1];

	#pragma omp parallel for
	for (int i = 1; i < i_max; i++)
	{
		createNextGeneration(breeding_pool[i], breeding_pool[i-1]);
		new_generation[pos++] = breeding_pool[i];
		new_generation[pos++] = breeding_pool[i-1];
	}
	// handle elitism
	if (const_data.ELITISM_FLAG)
	{
		addElite(pos);
		pos += const_data.ELITE;
		addLucky(pos);
	}
	
	swap(population, new_generation);
}

/******************************************************************************
 * mutateOnly()
 *
 * This function selects the designated number of breeders from the parent
 * population and uses them to breed a child generation. This breeder pool is
 * then randomized.
 *
 * If the elitism flag is not set, or twice breeder pool is larger than the
 * population size, then the iteration limit for the breeder pool is set to
 * the population size.
 *
 * The function then iterates through the breeder population, mutating each
 * breeder until either each has been mutated twice, or until the population
 * in generation is full.
 *
 * If the elitism flag is set to true, then the remaining population is
 * filled with members of the parent population.
 *****************************************************************************/
void mutateOnly()
{
	// select top portion of population for breeding
	for (int i = 0; i < const_data.BREEDERS; i++){
		breeding_pool[i] = population[i];
	}

	// shuffle breeders
	random_shuffle(breeding_pool.begin(), breeding_pool.end());

	/* max iterations should not exceed population size and should be population
	 * size if elitism flag is not set
	 */
	int i_max = const_data.BREEDERS * 2;
	if ((i_max > const_data.POP) || !const_data.ELITISM_FLAG)
	{
		i_max = const_data.POP;
	}

	int pos = 0;
	int b = 0;
	for (int i = 0; i < i_max; i++)
	{
		// wrap iterator to beginning of breeder vector
		mutate(breeding_pool[b]);
		new_generation[pos] = breeding_pool[b];
		b = (b + 1) % const_data.BREEDERS;
		pos++;
	}

	// handle elitism
	if (const_data.ELITISM_FLAG)
	{
		addElite(pos);
		pos += const_data.ELITE;
		addLucky(pos);		
	}
	
	swap(population, new_generation);
}

/******************************************************************************
 * createNextGeneration(Puzzle, Puzzle, vector<Puzzle>&)
 *
 * This function uses two puzzles from the stock of breeders and breeds them
 * together.
 *
 * An array of booleans representing subgrid positions is randomly filled with
 * true or false values. These will be used to determine which subgrids will be
 * swapped between the curr and prev puzzles. Subgrids which have a value of
 * true will be swapped between the two puzzles.
 *
 * The function then iterates through both puzzles at the same time. For each
 * 1-D position, the corresponding subgrid position is calculated. If the
 * subgrid position value in the boolean array is true, then the corresponding
 * square is swapped in the two puzzles
 *
 * params:
 *			curr - the puzzle in the current iteration position in the
 *				   calling function
 *			prev - the puzzle in the position previor to the current iteration
 *				   position in the calling function
 ******************************************************************************/
void createNextGeneration(Puzzle &curr, Puzzle &prev)
{
	// randomly set the which subgrid positions will be swapped
	bool cross_over[9];
	for (int i = 0; i < 9; i++)
	{
		if(rand()%2 == 0)
		{
			cross_over[i] = true;
		}
		else
		{
			cross_over[i] = false;
		}
	}
	// breed the puzzles
	for( int pos = 0; pos < const_data.DIM * const_data.DIM; pos++)
	{
		// retrieve the subgrid position based on 1-D puzzle position
		int grid = getGridPosition(pos);

		// check if the subgrid is to be swapped with the previous puzzle or not
		if (cross_over[grid])
		{
			square temp = prev[pos]; 
			prev[pos] = curr[pos];
			curr[pos] = temp;
		}
	}
	// roll the dice to see if either of the puzzles mutate
	if(rand()%100 < mutation_rate)
	{
		mutate(prev);
	}
	if(rand()%100 < mutation_rate)
	{
		mutate(curr);
	}
}

/******************************************************************************
 * addElite(vector<Puzzle>&)
 *
 * This function retrieves a pre-determined number of the most fit puzzles from
 * the parent population and carries them over into the new generation.
 *
 * params:
 *		pos - the current position in the new generation vector
*****************************************************************************/
void addElite(int pos)
{
	// add best of the population to the new generation
	for (int e = 0; e < const_data.ELITE; e++)
	{
		new_generation[pos] = population[e];
		pos++;
	}
}

/******************************************************************************
 * addLucky(vector<Puzzle>&)
 *
 * This function selects a number of random puzzles from the parent population
 * which are not in the top elite population group. These "lucky" individuals
 * are carried over into the new generation.
 *
 * params:
 *		pos - the current position in the new generation vector
 *****************************************************************************/
void addLucky(int pos)
{
	// add a few lucky souls to the new generation
	for(int i = 0; i < const_data.LUCKY; i++)
	{
		// pick a random puzzle
		int l = rand()%population.size();

		// ensure the chosen are not part of the elite group
		if (l < const_data.ELITE)
		{
			l += const_data.ELITE;
		}
		new_generation[pos] = population[l];
		pos++;
	}
}

/******************************************************************************
 * mutate(Puzzle&)
 *
 * This function selects a random subgrid within the passed in puzzle, and then
 * selects two random, non-preset positions within the subgrid. The values in
 * these two positions are then swapped.
 *
 * params:
 * 		puz - a reference to a puzzle object to be mutated
 *****************************************************************************/
void mutate( Puzzle &puz)
{
	//limit the number of times we try and find a mutate point (in case we hit a statistical home run)
	int search_limit = 100;

	// grab a random row and column and associated subgrid position
	int r = rand() % const_data.DIM;
	int c = rand() % const_data.DIM;
	int g = getGridPosition((r * const_data.DIM) + c);

	vector<square> grid = puz.getSub(r, c);

	//check that there are at least two non-preset values in the grid
	if (puz.subgrid_presets[g] > 7)
		return;

	int i1;
	int i2;
	int iter = 0;

	// look for different squares where neither contain preset values
	do
	{
		// grab two random positions within the subgrid
		i1 = rand() % const_data.DIM;
		i2 = rand() % const_data.DIM;
		iter++;
	} while (((i1 == i2) || grid[i1].preset || grid[i2].preset) && iter < search_limit);

	if(iter < search_limit)
	{
		// swap the two values
		char temp = grid[i1].getValue();
		grid[i1].setValue(grid[i2].getValue());
		grid[i2].setValue(temp);
		puz.setSub(r,c, grid);		
	}
}

/******************************************************************************
 * getGridPosition(int)
 *
 * This function uses a very awful looking function to compute the subgrid 
 * position of any given square based on its 1-D position within the puzzle.
 *
 * In an attempt to make the process more clear, what could be done in a 
 * single, horrifying, statement has been broken down into several steps
 * with clear variable names.
 *
 * The row and column are calculated based on the 1-D position.
 *
 * Next the starting row and column for the subgrid where the row and column
 * intersect are computed. 
 *
 * The first subgrid the farthest to the left in each row of subgrids will
 * have a position corresponding to the first row of itself.
 *
 * In order to compute the subgrid position for any subgrid, the quotient of
 * the subgrid start column divided by the subgrid dimension is added to the
 * subgrid start row.
 *
 * params:
 *			pos - a 1-D position within a puzzle
 *
 * returns: the integer subgrid position corresponding to the 1-D puzzle
 *			position
 *****************************************************************************/
int getGridPosition (int pos)
{
	// row and column containing square in position pos
	int row = pos / const_data.DIM;
	int col = pos % const_data.DIM;

	// starting row and column for subgrid containing square in row and col
	int subgrid_row_start = (row-(row % const_data.SUBDIM));
	int subgrid_col_start = (col - (col % const_data.SUBDIM));

	// return subgrid position
	return subgrid_row_start + (subgrid_col_start / const_data.SUBDIM);
}