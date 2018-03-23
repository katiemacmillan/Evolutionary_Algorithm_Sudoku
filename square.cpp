 /************************************************************************
   File: square.cpp
   Authors: Katie MacMillian, Jake Davidson
   Description: Function definitions for the square class.
 ************************************************************************/
#include "square.h"

/***************
 * Constructors
 ***************/

/******************************************************************
 * square()
 * Default constructor for square, sets possible values to all true
 * since there is no value yet, and sets preset to false.
 *****************************************************************/
square::square(){
	_possible.resize(9, true);
	preset = false;
}

/******************************************************************
 * square(char val, int dim, bool pre)
 * Constructor that sets the square's value, the dimension of the
 * puzzle the square is in, and if the square was pre-set or not.
 *
 * params:
 *			val - the value of the square
 *			dim - dimension of the puzzle, used to tell how many 
 *				  possible values there are
 * 			pre - bool for if the value was pre-set, either from the
 *				  initial puzzle config or through hidden/naked singles
 *****************************************************************/
square::square(char val, int dim, bool pre) {
	_value = val;
	//fill possible values list with either true or false
	//if the square is blank, all values are possible
	_possible.resize(dim, (_value == '-'));
	preset = pre;
}


/***************
 * Getters
 ***************/

/******************************************************************
 * getPossibleCt
 * get the number of possibilities left for the square
 *
 * returns: the number of possible values
 *****************************************************************/
int square::getPossibleCt()
{
	int count = 0;
	// count the remaining true flags in the possible array
	for (int i = 0; i < 9; i++){
		if (_possible[i]){
			count++;
		}
	}
	return count;

}

/******************************************************************
 * getPossibleValues()
 * get the possible values for the square
 *
 * returns: a vector of the possible values
 *****************************************************************/
vector<char> square::getPossibleValues()
{
	vector<char> values;
	// add positions with a true flag to a value vector
	for (int i = 0; i < 9; i++){
		if (_possible[i]){
			values.push_back((char)(i+1));
		}
	}
	return values;
}

/******************************************************************
 * isPossibleValue(char val)
 * check if a value is a possibility for the square
 *
 * params:
 *			val - the value to check
 *
 * returns: true if possible, false if not
 *****************************************************************/
bool square::isPossibleValue(char val)
{
	//fancy trick to convert char to int index
	int i = val - '1';
	return _possible[i];
}

/******************************************************************
 * getValue()
 * return the value of the square
 *
 * returns: the char symbol of the square
 *****************************************************************/
char square::getValue()
{
	return _value;
}
/***************
 * Setters
 ***************/

/******************************************************************
 * setValue(char val)
 * set the value of the square
 *
 * params: 
 *			val - the value to set as a char
 *****************************************************************/
void square::setValue(char val)
{
	_value = val;
}

/******************************************************************
 * setValue(int num)
 * set the value of the square
 *
 * params: 
 *			val - the value to set as an int
 *****************************************************************/
void square::setValue(int num)
{
	// convert number to string
	string val = to_string(num);
	// set value to character
	_value = val[0];
}

/******************************************************************
 * setValue(char val)
 * set the square's possible flag for the given value to false
 *
 * params: 
 *			val - the value to set as a char
 *****************************************************************/
void square::togglePossible(char val)
{
	// convert value to int and decrement to find possition in array
	int num = atoi((const char*) &val)-1;
	_possible[num] = false;
}

/******************************************************************
 * setSingle()
 * set the first valid value from the possible array to _value
 *****************************************************************/
void square::setSingle()
{
	for (int i = 0; i < 9; i++){
		// find first index with the value 'true'
		if (_possible[i]){
			int num = i+1;
			// convert to string
			string val = to_string(num);
			// set as character
			_value = val[0];
		}
	}
	preset = true;
}

/******************************************************************
 * setSingle(char val)
 * set the passed in value as a hidden single, toggling the preset flag
 *
 * params: 
 *			val - the value to set as a char
 *****************************************************************/
void square::setSingle(char val)
{
	// set as character
	_value = val;
	preset = true;
}

/******************************************************************
 * setSingle()
 * print all possible symbols
 * used for debugging purposes
 *****************************************************************/
void square::printPossible(){
	for(int i = 0; i < 9; i++){
		if(_possible[i]){
			cout << i+1 << ":" << _possible[i] << " ";
		}
	}
	cout << endl;
}