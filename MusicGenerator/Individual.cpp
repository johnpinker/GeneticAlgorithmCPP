#include "StdAfx.h"
#include ".\individual.h"

// Constructor
Individual::Individual(void)
{
}

// Destructor
Individual::~Individual(void)
{
}

// fetch the individuals note list
list<Note>& Individual::GetNotes(void)
{
	return this->noteList;
}

// Get/Set the fitness value
float Individual::GetFitness(void)
{
	return this->fitness;
}
float Individual::SetFitness(float newFitness)
{
	return this->fitness = newFitness;
}
