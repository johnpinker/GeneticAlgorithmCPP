/***********************************************************************
Author:		John Pinkerton
File:		GA.h
Purpose:	Header file for the implementation of the Genetic Algorithm.
***********************************************************************/
#pragma once

#include <fstream>
#include "Note.h"
#include "MidiFileWriter.h"
#include "InstrumentType.h"
#include "KitInstrument.h"
#include "Individual.h"
#include "MersenneTwister.h"

using namespace std;

class GA
{
private:
	// Popuation size
	int popSize;
	// crossover rate
	float crossoverRate;
	// mutation rate
	float mutationRate;
	// number of notes in a bar
	int resolution;
	// internal buffer for logging
	char logLine[255];
	// file to log individuals to
	ofstream indLog;
	// the instrument to use for evolution
	InstrumentType *instrument;
	// the main population
	Individual* population;
	// a temp population
	Individual* newPopulation;
	// the mating pool
	Individual* matingPool;
	// the random # generator
	MTRand rndGen;
	// current best individual
	Individual* bestInd;
	// current generation
	int generation;
	// average fitness for this generation
	float avgFitness;
	// variance for this generation
	float variance;

	// Calculate difference between two individuals
	float CalcDifference(Individual, Individual);

public:
	// constructor
	GA(void);
	// destructor
	~GA(void);
	// Initialize the ga system
	void InitGA();
	// get/set pop size
	int PopulationSize();
	int PopulationSize(int newSize);
	// get/set crossover rate
	float CrossoverRate();
	float CrossoverRate(float newRate);
	// get/set mutation rate
	float MutationRate();
	float MutationRate(float newRate);
	// start the GA
	void Start();
	// run the ga one time and return a string of log data
	char* RunOnce();
	// return a CString that contains a printout of the best individual
	CString PrintBest();
	// Prinnt the best individual to the provided output stream
	void PrintInd(ostream& os);
	// write the best individual to a new midi file named _fileName_
	void WriteMidiFile(CString fileName);
	// get/set the instrument to use
	InstrumentType* GetInstrument(void);
	void SetInstrument(InstrumentType* newInst);
	// tournament selection w/ a tournament size of 7
	void SelectTournament(void);
	// crossover the current population
	void Recombine(void);
	// mutate the current population
	void Mutate(void);
	// calculate the stats for the current generation
	void CalcStatsForGeneration(void);
	bool includeDifference;
};
