 /************************************************************************
   Program: AI PA#1 - Genetic Sudoku Solver
   Authors: Katie MacMillian, Jake Davidson
   Class: CSC 447/AI
   Instructor: Dr. Weiss
   Date: Feb 8, 2018
   Description: 

   This program will attempt to solve a given sudoku puzzle 
   using a genetic algorithm. 

   It starts by pre-processing the puzzle to take some of the load of the GA.
   It fills both naked and hidden singles until there is no change in the puzzle. 
   
   After this, it starts the solve routine. The outer loop here runs the inner 
   breed loop until either the puzzle is solved or we have reached the max generations. 
   The breed loop generates a new population by breeding and mutating the current
   population. It sorts this population by fitness and records the best and worst 
   of these values. It also keeps track of fitness stagnation, and if it stagnates for 
   too long it falls out and starts the outer loop again, which will start with a new 
   initial population filled with the best solutions we have guessed so far.

   After these loops fall out, we either have solved the puzzle or gotten to our max 
   generation limit. Either way, we output the results and exit the program.

   Input: the name of the file where the puzzle is stored and population, 
   		  generations, selection, and mutation values
   Output: Either the solution to the puzzle or the best guess we got after max
   		   generations have been reached
   Compilation instructions: make
   Usage: ./sudoku filename population generations selection mutation
 ************************************************************************/
#include "globals.h"
#include "preprocess.h"
#include "population.h"
#include "puzzle.h"
#include "breed.h"
#include <string>
using namespace std;

// "constant" data for the puzzle
ConstData const_data;

// program globals
int mutation_rate;
vector<Puzzle> population;
vector<Puzzle> new_generation;
vector<Puzzle> breeding_pool;
Puzzle initial_puzzle(9);

void solve();
void handleFlags(int, char*[]);
double tryParse(string, double);
bool readInPuzzle(char*);
void printUsage();
void printStartParams(char*, double);
void evaluatePresetCounts();

/******************************************************************************
 * main()
 *
 * This is the main entry point to the sudoku program. It begins by seeding
 * a random number generator, and then handles commandline arguments entered
 * by the user. When this is completed and the initial puzzle has been read
 * in, the function will enter a do-while loop which will look for naked and
 * hidden singles within the puzzle. 
 *
 * Both the function to find naked and hidden singles will return a boolean
 * value indicating whether or not a single was filled during the execution
 * of the function. The do-while loop will continue to look for singles until
 * both return without having filled any singles.
 *
 * When all singles have been filled in, the fitness of the puzzle is
 * evaluated. If the fitness of the puzzle is 0, it means that the puzzle is
 * completed and we are done. Otherwise the function moves goes into the solve
 * function.
 *
 * params:
 *			argc - 
 *			argv - 
 *
 * returns:
 *****************************************************************************/
int main(int argc, char* argv[])
{

	//init random number generator
	srand(static_cast <unsigned int> (time(0)));

	// take input flags and set program constant variable data
	handleFlags(argc, argv);

	// print original puzzle
	cout << "********Initial Configuration (9x9 grid)********" << endl;
	initial_puzzle.print(false);

	bool found1;
	bool found2;
	// fill singles of start puzzle until there is no change
	do
	{
		found1 = fillNakedSingles();
		found2 = fillHiddenSingles();

	} while (found1 || found2);

	cout << "********Filled In Predetermined Singles********" << endl;
	initial_puzzle.print(false);

	// evaluate puzzle to see if it is already solved
	initial_puzzle.evaluateFitness();
	

	// fill population and new_generation, this is to avoid pushbacks and rebuilding vectors
	for(int i = 0; i < const_data.POP; i++)
	{
		population.push_back(initial_puzzle);
		new_generation.push_back(initial_puzzle);

		if (i < const_data.BREEDERS){
			breeding_pool.push_back(initial_puzzle);
		}
	}

	evaluatePresetCounts();

	// check if puzzle is done
	if(initial_puzzle.fitness != 0)
	{
		solve();
	}
	else
	{
		cout << "**********Puzzle Complete After Filling Singles**********" << endl;
		initial_puzzle.print();
	}
	
	return 1;
}


/******************************************************************************
 * solve()
 *
 * This function executes the nested do loops in an attempt to solve the 
 * initial puzzle. The best puzzle found during the course of running is saved
 * in a Puzzle variable called 'best_puzzle'. It initially begins as the
 * inital puzzle.
 *
 * The outter do-while loop begins by creating a new population, assessing the
 * fittness of each puzzle in the population, and then sorting the population
 * in ascending order based on fitness. When this is done a handful of
 * variables are reset to their starting positions to begin the breeding phase
 * for the newly generated population.
 *
 * During the inner do-while loop the breeding phase occurs. In this loop the
 * population is either breed, or mutated to create subsequent generations.
 * The 'population' vector is replaced with the new generation, and then each
 * puzzle in has its fitness assesed, and then the population is sorted based
 * on fitness.
 * 
 * After sorting the population the fitness of the first puzzle in the 
 * population is compared to the value stored in 'best_fitness'. If both
 * values are the same, the best_fitness_ct is incremented. This is to indicate
 * how many generations have passed without a changing top puzzle fitness.
 *
 * If the best_fitness_ct counter reaches 15, it indicates a significant level
 * of stagnation. In these cases the program begins to increase the mutation
 * rate by 5% for each subsequent generation with the same best fitness value.
 * The mutation rate does not, however, exceed 100%.
 *
 * When the first puzzle's fitness is not the same as the 'best_fitness', the
 * counter is reset to 0, and the mutation rate is set back to the original
 * starting mutation rate.
 * 
 * When this is done the fitness of the best puzzle in the new
 * generation, now stored in the population vector, is compared to the fitness
 * of the puzzle stored in the 'best_puzzle' variable. If it the puzzle in 
 * the population vector has a lower fitness value, it becomes the new best
 * puzzle, and the generation in which it was placed there is also stored in
 * 'best_generation'.
 *
 * The best 1% of puzzles from each generation is also stored in a best of
 * vector. When restarting the population, if the best of puzzles vector is
 * equal to the population size, then the best of vector is used as the new
 * population.
 *
 * This inner do-while loop continues until the best_fitness is 0, the best
 * fitness has been stagnant for a set amount of times, or until the maximum
 * number of allowable generations has been iterated through.
 *
 * If the best fitness counter reaches 15 and the best fitness is 2, we
 * increase maximum number of generations which can pass without change, since
 * we are only 2 values off, we want to give a little extra time before
 * restarting.
 *
 * When the inner do-while loop concludes the outter loop will check to see if
 * a solution has been found, or if the maximum number of generations has been
 * reached. If not, a new populatin is created and the inner loop will begin
 * again. If so, the best puzzle from all of the generations is printed out
 * for the user.
 *****************************************************************************/
void solve()
{
	Puzzle best_puzzle = initial_puzzle;
	vector<Puzzle> best_of_puzzles;
	int best_generation = -1;

	int restart_ct = 0;
	int generation = 0;
	int best_fitness;
	int best_fitness_ct;
	int max_best_fitness_ct;
	auto fitness_time = chrono::high_resolution_clock::now();

	//timing iterations per second
	auto start_time = chrono::high_resolution_clock::now();
	chrono::duration<float> duration = start_time - start_time;
  	
	

	do{
		// get, evaluate and sort a new population
		if(best_of_puzzles.size() == const_data.POP)
		{
			cout << "**********Sarting With Best Of Population - Restart Number: " << restart_ct;
			cout << "**********" << endl;
			swap(population, best_of_puzzles);
			best_of_puzzles.clear();
		}
		else
		{
			cout << "**********Sarting With New Population - Restart Number: " << restart_ct;
			cout << "**********" << endl;
			getInitialPop();
		}
		fitness();

		// reset solve variables for the new population
		best_fitness = population[0].fitness;
		best_fitness_ct = 0;
		mutation_rate = const_data.START_MUTATE;
		max_best_fitness_ct = 20;
		do{
			start_time = chrono::high_resolution_clock::now();

			// create next generation via mutation or breeding
			if(const_data.MUTATE_ONLY_FLAG)
			{
				// generate new population using only mutation
				mutateOnly();
			}
			else
			{
				// generate new population using breeding
				breed();
			}
			duration += (chrono::high_resolution_clock::now() - start_time);

			// rank and sort population of new generation
			fitness();

			// keep track of failure to make fitness progress
			if (best_fitness == population[0].fitness)
			{
				best_fitness_ct++;
				
				// increase mutation rate after 15 stagnant generations, cap mutation rate at 100%
				if ((best_fitness_ct > 15) && (mutation_rate < 100))
				{
					mutation_rate += 5;
					// if fitness is 2, we're close, so don't restart after 30 give time for more mutations and breeding
					if (best_fitness == 2)
					{
						max_best_fitness_ct = 50;
					}
					else
					{
						// reset to 20 for cases with pure mutation or without elitism
						max_best_fitness_ct = 20;
					}
				}
			} 
			else
			{
				// reset the stagnation counter
				best_fitness_ct = 0;
				mutation_rate = const_data.START_MUTATE;
			}

			// store best puzzle out of all generations
			if(population[0].fitness <= best_puzzle.fitness)
			{
				best_puzzle = population[0];
				best_generation = generation;
			}

			// track the fitness of the most fit member of the population
			best_fitness = population[0].fitness;

			// store the best 1% of each generation and reset fitness
			for( int i = 0; i < (const_data.POP * 0.01); i++)
			{
				if(best_of_puzzles.size() < const_data.POP)
				{
					best_of_puzzles.push_back(population[i]);
				}
			}


			// display each generation details to the user
			 cout << "Generation"<< right << setw(5)<< generation;
			 cout << ": best score =" << right << setw(3)<< best_fitness;
			 cout << ", worst score =" << right << setw(3)<< population.back().fitness << endl;

			// move on to next generation
			generation++;

		// stop when a puzzle is solved, when the best fitness is stagnant or when reached max generations
		} while ((best_fitness != 0) && (best_fitness_ct < max_best_fitness_ct) && (generation < const_data.GENERATIONS));
		
		
		// if a puzzle hasn't been solved restart the population
		if(best_fitness != 0){
			restart_ct++;
		}


	// stop when a puzzle has been solved or when we have reached the maximum number of generations
	} while ((best_fitness != 0) && (generation < const_data.GENERATIONS));
		// get duration in millisecond format

		auto millisec = chrono::duration_cast<chrono::milliseconds>(duration);
		double iter_per_sec = (generation/(double)millisec.count()) * 1000;

		cout << "************Best Solution************" << endl;
		best_puzzle.print();
		cout << "Sudoku results: fitness " << best_puzzle.fitness << ", generation "<< best_generation;
		cout << ", " << millisec.count() << " msec, "<< iter_per_sec << " iter/sec" << endl;

}

/******************************************************************************
 * handleFlags(int, char*)
 *
 * This function uses the commandline arguments to retrieve user specified run
 * parameters. A switch statement is used to determine what needs to be parsed
 * based on the number of command line arguments.
 *
 * The switch statement starts with the highest number of commandline inputs
 * and does not contain breaks, rather it allows the program to fall through
 * all case executions under the initial triggering case. This is because
 * with each additional command line argument, all previous must also be dealt
 * with and parsed. Allowing the cases to fall through reduces repeated code
 * for things that must be parsed in multiple cases.
 *
 * params:
 *			argc - the number of command line arguments
 *			argv - a pointer of character arrays containing the command line
 *				   argument strings
 *****************************************************************************/
void handleFlags(int argc, char* argv[])
{
	double selection = 0.4;
	// check for valid number of arguments
	if (argc < 2 || argc > 7)
	{
		printUsage();
		exit(-1);
	}
	//check for correct file extension
	string filename(argv[1]);
	string extension = filename.substr(filename.length() - 4, filename.length());
	if (extension.compare(".txt") != 0)
	{
		cout << "Please provide a .txt file" << endl;
		exit(-1);
	}
	//read in puzzle from supplied file
	if(!readInPuzzle(argv[1]))
	{
		exit(-1);
	}
	
	//number of cmd line args not constant
	switch(argc)
	{
		// let cases fall through, higher counts will also parse elements of lower counts
		case 7: // all options entered
			const_data.ELITISM_FLAG = tryParse(argv[5], 1);
		case 6: // mutation rate, population, generations, and selection
			const_data.START_MUTATE = (tryParse(argv[5], 0.05)) * 100;
		case 5: // population, generations, and selection
			selection = tryParse(argv[4], 0.4);
		case 4: // generations, and selection
			const_data.GENERATIONS = tryParse(argv[3], 1000);
		case 3: // specified population
			const_data.POP = tryParse(argv[2], 100);
		// case 2 is all defaults
		// case 1 is invalid
	}

	// if negative mutation rate, solve using mutation only with population selection
	if (const_data.START_MUTATE < 0)
	{
		const_data.MUTATE_ONLY_FLAG = true;
	}

	// don't select more than 100% of the population for breeding or less than 0
	if (selection > 1.0)
	{
		selection = 1.0;
	}
	else if (selection < 0)
	{
		selection = 0;
	}

	// number of breeders to produce next generation
	const_data.BREEDERS = const_data.POP * selection;
	if (const_data.BREEDERS < const_data.POP / 2)
	{
		// number of breeders to keep in next generation
		const_data.ELITE = (const_data.POP - (const_data.BREEDERS * 2)) * 0.9;
		// number of lucky, non-fit population to keep in next generation
		const_data.LUCKY = const_data.POP - (const_data.BREEDERS* 2) - const_data.ELITE;
	}

	printStartParams(argv[1], selection);
}

/******************************************************************************
 * tryParse(string, double)
 *
 * This function will attempt to parse a string from command line input into
 * a double data type. If this is unsuccessful, the default value passed in
 * as 'd' will be returned to the user.
 *
 * params:
 *			s - a string to be parsed into a double
 *			d - a default double to be used if parsing fails
 *
 * returns: a double value, either parsed from the string or the given default
 *****************************************************************************/
double tryParse(string s, double d)
{
	double val;
	try
	{
		val = stod(s);
	} catch(...)
	{
		//default value if error on parse
		return d;
	}
	return val;
}

/******************************************************************************
 * readInPuzzle(char*)
 *
 * This function takes in a file path to a .txt file and opens it. Should the
 * file fail to open, the function returns false. 
 *
 * If the file opens successfully, each character is then read in, ignoring
 * white space. Each character is used as a value in a new square, which is
 * added to a global Puzzle variable called 'puzzle'. If the character read
 * in is not a '-' character, the square added is also flaged as a preset
 * value.
 *
 * params:
 *		file_name - the path to the text file containing the puzzle to be
 *					read in
 *			 
 * returns: true if the file was read in successfully, false if the file failed
 * 			to open
 *****************************************************************************/
bool readInPuzzle(char* file_name)
{
	ifstream file(file_name);

	//check if file opened
	if (!file.is_open())
	{
		cout << "Failed to open object file.  Exiting program..." << endl;
		return false;
	}

	char value;
	int i = 0;

	// read in characters ignoring white space
	while (file >> value)
	{
		// insert into puzzle
		if(value != '-')
		{
			initial_puzzle.addSquare(square(value, const_data.DIM, true));
		}
		else
		{
			initial_puzzle.addSquare(square(value, const_data.DIM));
		}
		// increment position, not going beyond bounds of puzzle
		if (i < (const_data.DIM * const_data.DIM)){
			i++;
		}
	}

	file.close();
	return true;
}

/******************************************************************************
 * printUsage()
 *
 * This function is just used to print out program usage for the user. It is
 * here to cut remove bulk cout statements from the handle flags function.
 *****************************************************************************/
void printUsage()
{
	cout << "\nInvalid amount of arguments.\nUsage:" << endl;
	cout << ".\\sudoku filename population generations selection mutation elitism" << endl;
	cout << "Mandatory Parameter: filename" << endl;
	cout << "Additional Options - Default:\npopulation  - 1000\ngeneration - 1000" << endl;
	cout << "selection - 0.4\nmutation - 0.05\nelitism - 1" << endl;
	cout << "Sample Usage:\n.\\sudoku puzzle.txt 2000 5000 0.6 0.1 0" << endl;
	cout << "Exiting program..." << endl;

}

/******************************************************************************
 * printStartParams(char*, double)
 *
 * This function is just used to print out the starting parameters for the
 * program.
 *
 * params:
 *		file_name - the name of the text file with the initial  puzzle
 *		selection - the user chosen selection rate
 *****************************************************************************/
void printStartParams(char* file_name, double selection)
{
	cout << left << setw(25)<< "Sudoku: " << file_name << endl;
	cout << left << setw(25)<< "population size: " << const_data.POP << endl;
	cout << left << setw(25)<< "number of generations: " << const_data.GENERATIONS << endl;
	cout << left << setw(25)<< "selection rate: " << selection << endl;
	cout << left << setw(25)<< "mutation rate: " << const_data.START_MUTATE / 100.0 << endl;
	cout << left << setw(25)<< "elitism: ";
	if (const_data.ELITISM_FLAG){
		cout << "ON" << endl;
		cout << left << setw(25)<< "breeders: " << const_data.BREEDERS << endl;
		cout << left << setw(25)<< "elite progessers: " << const_data.ELITE << endl;
		cout << left << setw(25)<< "lucky progressers: " << const_data.LUCKY << endl;
	}
	else
	{
		cout << "OFF" << endl;
	}

	cout << left << setw(25)<< "mutation only: ";
	if (const_data.MUTATE_ONLY_FLAG){
		cout << "ON" << endl;
	}
	else
	{
		cout << "OFF" << endl;
	}

	cout << endl;
}

/******************************************************************************
 * evaluatePresetCounts()
 *
 * This function counts the number of preset values in each subgrid of the
 * initial puzzle after singles have been filled. This values is then saved
 * into a vector in the puzzle class. This is to avoid having to repeat this
 * preset count for every subgrid we want to mutate later.
 *****************************************************************************/
void evaluatePresetCounts()
{
	vector<square> grid;
	vector<int> subgrid_presets;
	int count;
	int r;
	int c;
	
	// go through each subgrid in the puzzle
	for (int i = 0; i < const_data.DIM; i++)
	{
		count = 0;
		// get row and column for subgrid position
		r = (i / const_data.SUBDIM) * const_data.SUBDIM;
		c = (i % const_data.SUBDIM) * const_data.SUBDIM;

		grid = initial_puzzle.getSub(r, c);

		// count up the number of preset values in the subgrid
		for (int j = 0; j < grid.size(); j++)
		{
			if (grid[i].preset)
				count++;
		}
		subgrid_presets.push_back(count);
	}
	// store preset vector in the initial puzzle
	initial_puzzle.subgrid_presets = subgrid_presets;
}