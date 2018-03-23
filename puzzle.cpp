 /************************************************************************
   File: puzzle.cpp
   Authors: Katie MacMillian, Jake Davidson
   Description: Function definitions for the puzzle class
 ************************************************************************/
#include "puzzle.h"

/***************
 * Constructors
 ***************/

/******************************************************************
 * Puzzle(int dim)
 * Constructor for new puzzle object. Takes dimension of puzzle.
 *
 * params:
 *			dim - dimension of puzzle (ex, 9x9 is dim = 9)
 *****************************************************************/
Puzzle::Puzzle(int dim)
{
	_dim = dim;
	_sub_dim = sqrt(dim); //subgrid dimensions
	fitness = 0; //start with 0 fitness
}

/******************************************************************
 * Puzzle(const Puzzle& p)
 * Copy constructor for making copy of other puzzle object
 *
 * params:
 *			p - puzzle to create copy of
 *****************************************************************/
Puzzle::Puzzle(const Puzzle& p){
	_dim = p._dim;
	_squares = p._squares;
	_sub_dim = p._sub_dim;
	fitness = p.fitness;
	subgrid_presets = p.subgrid_presets;
}

/***************
 * Operators
 ***************/

/******************************************************************
 * operator [] (int i) const
 * overloaded [] access oparator for ease of access to puzzle squares
 *
 * params:
 *			i - index of square to access
 * returns: square at index i in puzzle
 *****************************************************************/
square Puzzle::operator [] (int i) const
{
	return _squares.at(i);
}

/******************************************************************
 * operator [] (int i)
 * overloaded [] operator for assignment. This allows you to assign
 * a square position in the puzzle class as you would a normal vector.
 *
 * params:
 *			i - index of square to access
 * returns: reference to location in squares vector for assignment
 *****************************************************************/
square & Puzzle::operator [] (int i)
{
	return _squares.at(i);
}

/******************************************************************
 * operator < (const Puzzle& puz) const
 * overloaded < operator for use in sorting puzzles by fitness. The 
 * STL needs this operator to use its sort routine on custom classes.
 *
 * params:
 *			puz - puzzle to compare against
 * returns: true/false for less than/greater than
 *****************************************************************/
bool Puzzle::operator < (const Puzzle& puz) const
{
	return (this->fitness < puz.fitness);
}

/***************
 * Functions
 ***************/

/******************************************************************
 * addSquare(square s)
 * Add a new square to the end of the squares vector.
 *
 * params:
 *			s - square to add
 *****************************************************************/
void Puzzle::addSquare(square s)
{
	_squares.push_back(s);
}

/******************************************************************
 * size()
 * Returns the number of squares in the puzzle
 *
 * returns: the size of the square as int
 *****************************************************************/
int Puzzle::size()
{
	return _squares.size();
}

/******************************************************************
 * setRow(int r, vector<square> s)
 * overwrites a row in the puzzle with a new one
 *
 * params:
 *			r - which row to set
 *			s - vector of squares to replace current row with
 *****************************************************************/
void Puzzle::setRow(int r, vector<square> s){
	//ensure the puzzle is large enough
	if(_squares.size() != (_dim*_dim)){
		_squares.resize(_dim*_dim);
	}
	//get index into row (stored in row major 1D form)
	int pos = r*_dim;
	//copy values into row
	for( int i = 0; i < s.size(); i++){
		_squares[pos+i] = s[i];
	}
}

/******************************************************************
 * setCol(int c, vector<square> s)
 * overwrites a column in the puzzle with a new one
 *
 * params:
 *			c - which column to set
 *			s - vector of squares to replace current column with
 *****************************************************************/
void Puzzle::setCol(int c, vector<square> s){
	//ensure puzzle is large enough
	if(_squares.size() != (_dim*_dim)){
		_squares.resize(_dim*_dim);
	}
	//write into column
	for( int i = 0; i < _dim; i++){
		int pos = (i*_dim) + c; //this gets row index
		_squares[pos] = s[i];
	}
}
/******************************************************************
 * setSub(int r, int c, vector<square> s)
 * overwrites a subgrid in the puzzle with new squares
 *
 * params:
 *			r - row of subgrid
 *			c - col of subgrid
 *			s - vector of squares to replace current subgrid with
 *****************************************************************/
void Puzzle::setSub(int r, int c, vector<square> s)
{
	//ensure puzzle is large enough
	if(_squares.size() != (_dim*_dim)){
		_squares.resize(_dim*_dim);
	}
	// find top left square of grid containing square in row, col
	int row = r - r%3;
	int col = c - c%3;
	int indx = 0;
	for (int i = 0; i < 3; i++)
	{
		//convert to 1D coords
		int pos = ((row + i) * _dim) + col;
		//overwrite values in subgrid with new ones
		for (int p = pos; p < pos + 3; p++)
		{
			_squares[p] = s[indx];
			indx++;
		}
	}
}

/******************************************************************
 * getRow(int r)
 * returns a row of the puzzle as a vector
 *
 * params:
 *			r - row to return
 * returns: a vector of squares from row r
 *****************************************************************/
vector<square> Puzzle::getRow(int r){
	std::vector<square> row;
	int pos = r*_dim; //get 1D start pos of row
	for( int i = 0; i < _dim; i++){
		row.push_back(_squares[pos+i]);
	}
	return row;
}

/******************************************************************
 * getCol(int c)
 * returns a col of the puzzle as a vector
 *
 * params:
 *			c - col to return
 * returns: a vector of squares from col c
 *****************************************************************/
vector<square> Puzzle::getCol(int c){
	std::vector<square> col;
	for( int i = c; i < (_dim*_dim) + c; i += _dim){
		col.push_back(_squares[i]);
	}
	return col;
}

/******************************************************************
 * getSub(int r, int c)
 * returns a subgrid from the puzzle
 *
 * params:
 *			r - row of subgrid
 *			c - col of subgrid
 * returns: a vector of squares from the specified subgrid
 *****************************************************************/
vector<square> Puzzle::getSub(int r, int c)
{
	std::vector<square> sub;
	//get subgrid from r and c
	int row = r - r%3;
	int col = c - c%3;
	//walk subgrid filling vector with squares
	for (int i = 0; i < 3; i++)
	{
		int pos = ((row + i) * _dim) + col;
		for (int p = pos; p < pos + 3; p++)
		{
			sub.push_back(_squares[p]);
		}
	}
	return sub;
}

/******************************************************************
 * getSquares()
 * returns all squares from the puzzle
 *
 * returns: a vector of all squares
 *****************************************************************/
vector<square> Puzzle::getSquares()
{
	return _squares;
}

/******************************************************************
 * print()
 * prints the puzzle in a nice format, with *s indicating duplicates
 *
 * params:
 *			duplicates - print * for dupilcate symbols (default true)
 *****************************************************************/
void Puzzle::print(bool duplicates) 
{
	//start at beginning of puzzle
	int r = 0;
	int c = 0;
	int row_dupe = 0;
	std::vector<int> col_dupes;
	//walk puzzle
	for(int i = 0; i < (_dim*_dim); i++)
	{
		cout << _squares[i].getValue();
		c++;
		//if at end of subgrid, print space before the next one
		if (c % (int)(sqrt(_dim)) == 0)
		{
			cout << "  ";
		}
		//if we are at the end of a row, newline
		if (c % _dim == 0)
		{
			if(duplicates)
			{
				//print * if duplicates in row
				row_dupe = countDuplicates(getRow(r));
				for (int x = 0; x < row_dupe; x++)
					cout << "*";
			}
			
			cout << endl;
			//increment row counter
			r++;
			//if we are at the end of the subgrid row print a newline for separation
			if (r%(int)(sqrt(_dim)) == 0)
			{
				cout << endl;
			}
			//reset col counters
			c = 0;
		}
	}
	if (duplicates)
	{
		//print col duplicates
		c = 0;
		while (c < _dim)
		{
			if (countDuplicates(getCol(c)))
				cout << "*"; //duplicate
			else
				cout << " "; //np dupes, pad with space
			c++;
			//if we are at a new subgrid, space again
			if (c % (int)(sqrt(_dim)) == 0)
			{
				cout << "  ";
			}
		}
	}
	cout << endl;
}


/******************************************************************
 * evaluatePossibleValues()
 * eliminate all possible values from each square
 *****************************************************************/
void Puzzle::evaluatePossibleValues()
{
	for(int pos = 0; pos < (_dim * _dim); pos++)
	{
		eliminateValues(_squares[pos], pos);
	}
}

/******************************************************************
 * eliminateValues(square &s, int pos)
 * Eliminate possible square values for a particular square.
 * We can eliminate possible square values if that square's symbol
 * exists elsewhere in the row, col, and subgrid
 *
 * params:
 *			s - the square to eliminate values from
 *			pos - the 1D position of the square in the puzzle
 *****************************************************************/
void Puzzle::eliminateValues(square &s, int pos)
{
	int c = pos % _dim;
	int r = pos / _dim;
	// eliminate values that are in square's row
	vector<square> row = getRow(r);
	for(int i = 0; i < row.size(); i++){
		if(row[i].getValue() != '-')
		{
			s.togglePossible(row[i].getValue());
		}
	}

	// eliminate values that are in square's column
	vector<square> col = getCol(c);
	for(int i = 0; i < col.size(); i++){
		if(col[i].getValue() != '-')
		{
			s.togglePossible(col[i].getValue());
		}		
	}
	
	// eliminate values that are in square's grid
	vector<square> sub = getSub(r,c);
	for(int i = 0; i < sub.size(); i++){
		if(sub[i].getValue() != '-')
		{
			s.togglePossible(sub[i].getValue());
		}
	}
}

/******************************************************************
 * evaluateFitness()
 * We define a solution's fitness by the number of duplicate symbols
 * throughout all rows and columns.  This function assigns a fitness
 * to the puzzle.
 *****************************************************************/
void Puzzle::evaluateFitness()
{
	fitness = 0;

	for (int i = 0; i < _dim; i++)
	{
		fitness += countDuplicates(getRow(i));
		fitness += countDuplicates(getCol(i));
	}
}

/******************************************************************
 * countDuplicates(vector<square> list)
 * Return the number of duplicates in a list of squares
 *
 * params:
 *			list - vector of squares to count duplicates from
 *****************************************************************/
int Puzzle::countDuplicates(vector<square> list){
	bool seen[9] = {0};
	int duplicates = 0;

	for(int i = 0; i < list.size(); i++){
		int value = (list[i].getValue() - '0') - 1;
		if (!seen[value])
			seen[value] = true;
		//if we have seen it, we have a duplicate, increment duplicates
		else
		{
			duplicates++;
		}
	}
	return duplicates;
}
