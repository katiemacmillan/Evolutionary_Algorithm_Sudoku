 /************************************************************************
   File: globals.h
   Authors: Katie MacMillian, Jake Davidson
   Description: Stores global program information and variables. The ConstData 
   struct holds constant info about the puzzle size and genetic algorithm params.
   It also stores the initial puzzle state, the current and next populations in 
   the breed phase, and the current mutation rate.
 ************************************************************************/
#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <thread>

#include "square.h"
#include "puzzle.h"

/******************************************************************************
 * ConstData
 * This is a structure to hold "constant" data for the program. This is data
 * that is set at the begining of the program execution, possibly by the user,
 * but then remains constant throughout the remainder of the program execution.
 * The values contained here are the default values for the program.
 *****************************************************************************/
struct ConstData{
	const int DIM = 9; //9x9 puzzle
	const int SUBDIM = 3; //sugrids are 3x3
	const vector<char> VALUE_SET = {'1', '2', '3', '4', '5', '6', '7', '8', '9'}; //permute using these values

	int POP = 1000; //default pop size
	int GENERATIONS = 1000; //default number of generations

	int BREEDERS;
	int ELITE = 0; //how many individuals are selected for elitism
	int LUCKY = 0; //how many lucky (bottom fitness percentile) are chosen for reproduction
	bool ELITISM_FLAG = true; //carry over top percentage of previous population
	bool MUTATE_ONLY_FLAG = false; //if true, only mutate when making new generations
	int START_MUTATE = 5; //starting mutation rate (expressed as integer percentage)
};


extern Puzzle initial_puzzle; //the puzzle we are solving in it's init state
extern vector<Puzzle> population; //list of puzzles that make up our current population
extern vector<Puzzle> new_generation; //the next generation of puzzles created from population
extern vector<Puzzle> breeding_pool; //list to store current breeders
extern int mutation_rate; //rate at which mutations will occur

// program constants
extern ConstData const_data;

#endif