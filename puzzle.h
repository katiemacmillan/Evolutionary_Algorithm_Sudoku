 /************************************************************************
   File: puzzle.h
   Authors: Katie MacMillian, Jake Davidson
   Description: Header file for the puzzle class. The puzzle class contains 
   a list of square objects that make up the puzzle (1D row major) as well 
   as other data about the puzzle, such as it's fitness and dimension.

   Usage: You can create a puzzle with just a dimension, which will make all 
   squares have a blank value. You can also use the copy constructor to copy 
   an already existing puzzle. Once created, there are multiple ways to fill 
   it with values, using the set functions. You can set rows, columns, and 
   subgrids, or set one square at a time with addSquare(). Once filled, 
   evaluateFitness will assign a rank to the puzzle based on how correct 
   the puzzle is (defined as the number of duplicates in rows and columns).
 ************************************************************************/
#ifndef __PUZZLE_H_
#define __PUZZLE_H_

#include <vector>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include "square.h"

class Puzzle
{
public:
	private:
		std::vector<square> _squares; //the squares of the puzzle, stored in row major format
		int _dim; //dimension of one side of the puzzle (square puzzles assumed/supported)
		int _sub_dim; //dimension of subgrid
		void eliminateValues(square&, int); //eliminate a square's values that it cannot be
		int countDuplicates(vector<square>); //count the number of duplicates squares in list
	public:
		int fitness; //fitness level of the puzzle
		vector<int> subgrid_presets; // number of preset values in each subgrid
		//constructors
		Puzzle(int); //constructor that takes the size of the puzzle
		Puzzle(const Puzzle&); //copy constructor
		//functions
		void addSquare(square); //add a square to the back of the puzzle
		int size(); //get the size of the puzzle
		void print(bool duplicates = true); //print the puzzle in a nice format
		vector<square> getRow(int); //get a specific row of the puzzle
		vector<square> getCol(int); //get a specific col of the puzzle
		vector<square> getSub(int, int); //get a specific subgrid of the puzzle
		vector<square> getSquares(); //get all the squares in the puzzle
		void setRow(int, vector<square>); //set a row of the puzzle
		void setCol(int, vector<square>); //set the col of the puzzle
		void setSub(int, int, vector<square>); //set the subgrid of the puzzle
		void evaluatePossibleValues(); //eliminate possible values for all squares in puzzle
		void evaluateFitness(); //set the puzzle's fitness level

		//overloaded operators
		square operator [] (int) const; //access a square with []
		square & operator [] (int); //assign to a square with []
		bool operator < (const Puzzle& puz) const; //compare two puzzle's using fitness
};
#endif