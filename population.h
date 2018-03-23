 /************************************************************************
   File: population.h
   Authors: Katie MacMillian, Jake Davidson
   Description: Function prototypes for population.cpp. These functions 
   deal with generating the initial poulation and fitness evaluation.
 ************************************************************************/
#ifndef POPULATION_H_
#define POPULATION_H_

#include "globals.h"


void fitness();
void getInitialPop();
vector<char> eraseSetValue (vector<char>, char);

#endif