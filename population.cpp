 /************************************************************************
   File: population.cpp
   Authors: Katie MacMillian, Jake Davidson
   Description: This file contains function definitions for generating the 
   initial population and evaluating the fitness for each member of a population.
 ************************************************************************/
#include "population.h"

/******************************************************************************
 * getInitialPop()
 *
 * This function generates puzzles to fill the population vector. To create a
 * new puzzle, a temporary puzzle is created and set to the value of the
 * initial puzzle. Then for each subgrid in the new puzzle, a copy of the
 * possible values (1-9) is made, and all preset values in the subgrid are
 * removed from the temporary set. 
 *
 * The remaining values in the temporary set are then shuffled and each empty
 * square in the subgrid is filled in with one of the remaining values in the
 * temporary set. This ensures that each subgrid in each puzzle is a
 * permutation of 1 though 9.
 *****************************************************************************/
void getInitialPop()
{
	for (int pop = 0; pop < const_data.POP; pop++) 
	{
		vector<char> temp_set = const_data.VALUE_SET;
		Puzzle temp_puzzle = initial_puzzle;
		vector<char>::iterator it;
		vector<square> subgrid;

		for(int r = 0; r < const_data.DIM; r+=3){
			for(int c = 0; c < const_data.DIM; c+=3){
				subgrid = temp_puzzle.getSub(r,c);
				temp_set = const_data.VALUE_SET;

				// remove all in place elements from the value set
				for(int i = 0; i < subgrid.size(); i++){
					temp_set = eraseSetValue(temp_set, subgrid[i].getValue());
				}
				// randomize remaining value set
				random_shuffle(temp_set.begin(), temp_set.end());

				// fill empty squares with remaining set values
				for(int i = 0; i < subgrid.size(); i++){
					if(subgrid[i].getValue() == '-'){
						// set square value to first value in set
						subgrid[i].setValue(temp_set.back());
						// remove used value from set
						temp_set.pop_back();
					}
				}
				temp_puzzle.setSub(r, c, subgrid);

			}
		}
		population[pop] = temp_puzzle;
	}
}

/******************************************************************************
 * fitness()
 *
 * This function calls the puzzle class' evaluate fitness function on each
 * puzzle in the population vector. When this is completed it then sorts
 * the vector in ascending order based on fitness.
 *****************************************************************************/
void fitness()
{
	//map for counting symbols in each row, col, subgrid
	for (int i = 0; i < population.size(); i++)
	{
		// only check rows and columns since we know subgrids have no duplicates
		population[i].evaluateFitness();
	}

	// sort the population based on fitness
	sort(population.begin(), population.end());
}

/******************************************************************************
 * eraseSetValue()
 *
 * This function goes through a vector of characters and when it finds the
 * target character it erases it from the vector. The resulting vector is then
 * returned.
 *
 * params:
 *		   set - a vector of possible characters
 *		target - the target character to erase
 *
 * returns: a vector of characters with the target character removed
 *****************************************************************************/
vector<char> eraseSetValue (vector<char> set, char target)
{
	for (int i = 0; i < set.size(); i++){
		if (set[i] == target){
			set.erase(set.begin()+i);
			return set;
		}
	}
	return set;
}

