GA - SUDOKU
Authors: Jake Davidson & Katherine MacMillan
Due Date: Feb 8, 2018
Class: CSC 447 - Artificial Intelligence
Professor: Dr. John Weiss

Usage:

This program has multiple input options to customize the way the program runs

sudoku <puzzle.txt filename> <population size> <maximum generation> <selection rate> <mutation rate> <elitism flag>
	<puzzle.txt filename> 	
			DESCRIPTION		- The txt file containing the puzzle to be solved
			DATATYPE		- STRING
			DEFAULT			- None, MANDATORY PARAMETER
	<population size> 		
			DESCRIPTION		- The size of the population pool to generate for breeding
			DATATYPE		- INTEGER
			DEFAULT			- 1000
	<maximum generation>	
			DESCRIPTION		- The maximum number of generations to breed before aborting
			DATATYPE		- INTEGER
			DEFAULT			- 1000
	<selection rate>		
			DESCRIPTION		- The percentage of the population to select for breeding
			DATATYPE		- DOUBLE
			DEFAULT			- 0.4
	<mutation rate>			
			DESCRIPTION		- The percent chance that any given puzzle will mutate
			DATATYPE		- DOUBLE
			DEFAULT			- 0.05
	<elitism flag>			
			DESCRIPTION		- Either a 1 or a 0 to indicate if elitism should be turned on
			DATATYPE		- BOOLEAN INTEGER
			DEFAULT			- 1 (true)

Sample Usage:
	sudoku med1.txt 2000 5000 0.5 0.1 0

-------------------------------
--------Program Summary--------
-------------------------------

------------------
****Parameters****
------------------
This program contains several optional parameters and one mandatory one. The first parameter, the puzzle text file path MUST be entered as a parameter or the program will not continue. 

All the remaining parameters have default values, however in order to specify a later parameter, for instance, selection rate, the preceeding parameters must also be specified.

In the event of a selection rate greater than 100% being specified, the selection rate will be clipped to 100%.
In the event of a negative seletion rate, the selection rate will default to 0, and there will be no breeding pool from which to create the next generation. This will result in pure elitism involving the original population. If the elitism flag is also set to false, no progress will be made, and nothing will be done. The population will cease to exist after the initial generation.

In the event of a negative generation or population size, a negative population will result in a population size of 0, and a negative generation count will result in zero generations of breeding to occur. 

In the event of a negative mutation rate the program will attempt to solve the puzzle using pure mutation rather than breeding. In this case the breeding pool will be selected using the selection rate, but rather than breeding pairs from the breeding pool, each puzzle in the breeding pool will be mutated and put into the next generation. This mode may be used with or without elitism. When elitism is enabled, the "elites" and "luckies" will not be mutated.

The final parameter is a boolean integer to indicate the status of elitism. If a 0 is passed in elitism will be turned off. All other integer values will result in elitism remaining on. Elitism is on by default.

-----------------------
****Data Structures****
-----------------------
Two specialized classes were created for this program. 

The Square class is a class to represent an individual square within a puzzle. The square class contains information about the square, such as whether or not it is considered a preset value, and what possible values, if any, can go in the square. It also contains methods to set a square's value as a single, indicating that it is a preset value, and to modify which values are possible in the square.

The Puzzle class is a class to represent a single puzzle. It contains a 1-D vector of Square objects, as well as the dimention of the puzzle, the dimention of a subgrid within the puzzle, and the fitness of the puzzle. This class also contains methods for retrieving or setting a single row, column or subgrid from within the puzzle, as well as methods to evaluate possible hidden single values in the puzzle, and the fitness of the puzzle. The Puzzle class also contains a method to print out the puzzle, which will put asterisks beside each row containing duplicates, and below each column containing duplicates.

---------------
****Singles****
---------------
Singles are handled in two separte functions. First naked singles are filled in by finding the possible values which could go in each empty square. If only one value can go into a given square, that value is set as a single in that square. Next the hidden singles are examined for each row, column and subgrid. All the possible positions for the possible values for a given row, column, or subgrid are examined, and if a value exists with only one possible position, the value is places as a single within that position.
The naked and hidden singles will continually be checked until no singls have been found during the execution of either the fillNakeSingles() function or the fillHiddenSingles() function. This is done because at time filling in a hidden single will reveal a naked single, so when hidden singles have completed their passes, the naked singles must be re-checked, and visa versa.

------------------
****Population****
------------------
The initial population is generate in subgrid major fashion by creating subgrids which consist of 1-9 permutations. Each subgrid is examined for preset squares, and those values are removed from a set of possible 1 - 9 values which could go into the subgrid. Once the set of possible values contains only the remaining values of the permutation for the subgrid, each square is filled by randomly selecting a value from the set and placing the value into the square. Once a value is selected it is removed from the set of possible values to choose from.
This ensures that there are no duplicates in the subgrids when the initial population is done being generated.

---------------
****Fitness****
---------------
Fitness is computed by evaluating the number of duplicates within a puzzle. The total number of duplicates within each row and each column of a puzzle are summed together to become the fitness of the puzzle being evaluated. Since the puzzle is handled in subgrid major fashion, no subgrids will have duplicates, and do not need to be checked, as they will always be 0.

---------------
****Sorting****
---------------

A '<' operator has been added to the Puzzle class which allows the use of the STL sort function. This is used to sort the puzzles in ascending order based on fitness within a population vector. This allows the puzzles with the best fitness to be placed at the front of the population vector for easy access.

----------------
****Breeding****
----------------

Breeding is done by using pairs from the breeding pool. The breeding pool is extracted from the population based on the selection rate. The top percentage of the population specified by the selection rate is copied over into the breeding pool, which is then shuffled.

If the breeding pool size is greater than half the population, only the first breeders up to the same index as half the population size will be taken out for breeding.

If the breeding pool size is less than half the population and elitism is not turned on, random members of the breeding pool will be selected and bred until the new generation has a complete population. If elitism is turned on then each member of the breeding population will breed twice, once with the one before it in the vector, and once with the puzzle following it in the vector, and the remaining population size will be filled with direct members of the breeding generation through elitism.

When two puzzles are bred together they are tracked by current puzzle and previous puzzle. The current puzzle is the puzzle at the current index of the breeder pool, and previous is, naturally, the previous puzzle. An array of booleans is created to represent the 9 different subgrid positions within a puzzle. Each position in the array of crossover points is randomly filled with a true or a false. 

Both the current and previous puzzles are walked iteratively in 1-D. The subgrid position of each square is calculated based on its 1-D position within the puzzle, which is stored row major for ease. If the flag in the crossover array for a square's subgrid position is marked as true, then the puzzle destination of the squares is swapped, meaning the subgrid from puzzle 'current' is moved to the primary child of 'previous' and the subgrid from 'previous' is moved into the primary child of 'current'. This results in multiple random crossover points, and a good mixing of genetic material between parents to produce new varieties of children. The idea is that if the best generations are very similiar, many of these subgrids are the same, so we are ensuring the ones that are different are randomized.

When two puzzle's children are completed each one is given a chance to mutate.

There is a mode of solving involving mutation only. In this mode the population is not bred together, but instead the breeding population is selected and then each one is mutated. This is done until the new generation has a full population if elitism is turned off. If elitism is turned on, then the remaing population is filled with unmutated members of the previous generation

----------------
****Mutation****
----------------

Mutation is implemented by selecting a random subgrid position within the puzzle being mutated. Then two positions within the subgrid are randomly chosen. These will be continually picked until two positions are found that are different positions, and where neither position is a preset square. When these two positions are found within the subgrid, the values of the two squares are swapped.

----------------
****Timing****
----------------

In order to best capture the time taken to breed each generation, timing only encapsulates the breed or mutate only step of the program execution.

----------------------------------
--------Program Evaluation--------
----------------------------------

------------------
****Efficiency****
------------------

Several decisions wer made to increase the efficiency of this program. One of the primary methods to increase efficiency was to cut down on the number of pushbacks that were made. To this end, the global vectors holding the population, new generation and breeding pool are temporarily padded at the beginning of the program execution with the initial puzzle. This was needed because there is not a no-arg constructor for the puzzle class. This is due to needing the dimention to be able to set the size of the squares vector within the puzzle class, and the desire to leave the program capabale of easily being modified to handle puzzles of different dimensions.

By having the vectors padded, new children are directly placed into the new generation vector and when breeding is completed, the population and new generation vectors are swapped. 

The subgrid preset count is also evaluated at the begining of the program execution after the initial puzzle has had all singles filled. This is because the subgrid positions do not move, and these preset counts will remain constant. By evaluating this at the begining, and having the value copied as the population is generated and children are bred, this value is passed along. If this were not done, then the subgrid preset count would have to be re-assesed at every execution of the mutate function.

Finally, breeding generations is done in parallel utilizing OpenMP, and the unroll loops flag was added to the make file.

----------------
****Accuracy****
----------------

A series of trials was done using varying population sizes and the following settings:
Generations: 1000
Selection Rate: 40%
Mutation Rate: 5%
Elitism: On

Each puzzle was run consecutively 50 times each using populations of 100, 500, 1000, and 2000. The following depicts the rate at which the program was able to solve the puzzle within 1000 generations:

Puzzle 		100 	500 	1000 	2000
-----------------------------------------
Medium1 	30% 	88% 	96% 	96%
Medium2 	14% 	52% 	74% 	72%
Medium3 	20% 	66% 	90% 	98%
Hard1 		0% 		26% 	58% 	40%
Hard2 		0% 		24% 	38% 	18%
Hard3 		14% 	24% 	76% 	82%

The easy puzzles were able to solve through nake and hidden singles, and were not subject to the breeding process.
