 /************************************************************************
   File: square.h
   Authors: Katie MacMillian, Jake Davidson
   Description: Header file for the square class. A square object represents 
   one square in the sudoku puzzle. It contains the char symbol in that square, 
   as well as additional info like if it was a preset value and what possible 
   values the square could be (used for pre-processing hidden/naked singles)

   Usage: The default constructor will set all possible values to true and 
   set the preset flag to false. You can also create a square with a vale 
   already filled (use '-' for blank), as well as the dimension of the puzzle 
   and whether or not it is a preset square. Once created, you can use the 
   set and toggle functions to modify the possible values if you are solving 
   for hidden and naked singles.
 ************************************************************************/
#ifndef __SQUARE_H_
#define __SQUARE_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

class square
{
	public:
		bool preset; //flag for if this square is 'set in stone' from the init puzzle config or through hidden/naked singles
		// Constructors
		square(); //default
		square(char, int, bool = false); //set square value, defeult to preset flag = false

		// Getters
		int getPossibleCt(); //get the number of possible squares
		char getValue(); //get the value of the square
		vector<char> getPossibleValues(); //get the possible values

		// Setters
		//set the value
		void setValue(char);
		void setValue(int);
		void togglePossible(char); //set possible value to false
		void setSingle(); //set _value to first valid value
		void setSingle(char); //set the value and toggle preset

		// Others
		void printPossible(); //print the possible values
		bool isPossibleValue(char); //check if value is possible

	private:
		char _value;				// value at possition
		vector<bool> _possible;			// which values are possible for this square
};

#endif