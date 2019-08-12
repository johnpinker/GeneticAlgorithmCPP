/***********************************************************************
Author:		John Pinkerton
File:		Individual.h
Purpose:	Definition of an individual member of the population.
***********************************************************************/
#pragma once

#include <list>
#include "Note.h"

using namespace std;

class Individual
{
private:
	// genome structure
	list<Note> noteList;
	// fitness of the genome
	float fitness;
public:
	// constructor/destructor
	Individual(void);
	~Individual(void);
	// return the notes wrapped by this object
	list<Note>& GetNotes(void);
	// get/set the fitness
	float GetFitness(void);
	float SetFitness(float newFitness);
};
