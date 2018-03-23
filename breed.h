 /************************************************************************
   File: breed.h
   Authors: Katie MacMillian, Jake Davidson
   Description: Contains function prototypes for the breeding functions in 
   breed.cpp.
 ************************************************************************/
#ifndef BREED_H_
#define BREED_H_

#include "globals.h"

void breed();
void mutateOnly();
void createNextGeneration(Puzzle&, Puzzle&);
void addElite(int);
void addLucky(int);
void mutate(Puzzle &);
int getGridPosition (int);

#endif