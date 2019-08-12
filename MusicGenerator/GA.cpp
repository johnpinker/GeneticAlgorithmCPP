#include "StdAfx.h"
#include "ga.h"
#include "KitInstrument.h"


/*
Default Constructor
*/
GA::GA(void)
: includeDifference(false)
{
	this->generation = 0;
	this->indLog.open("indLog.txt");
}

/*
Destructor
*/
GA::~GA(void)
{
	this->indLog.close();
	delete [] this->population;
	delete this->instrument;
}

/*
Initialize the GA with the correct operators to use
*/
void GA::InitGA() {
	this->population = new Individual[this->popSize];
	for (int i=0; i<this->popSize;i++) {
		this->instrument->Initialize(this->population[i].GetNotes());
	}
}


// Population Size Setters/Getters
int GA::PopulationSize() {
	return this->popSize;
}
int GA::PopulationSize(int newSize) {
	return (this->popSize = newSize);
}

// Crossover Rate Setters/Getters
float GA::CrossoverRate() {
	return this->crossoverRate;
}
float GA::CrossoverRate(float newRate) {
	return (this->crossoverRate = newRate);
}

// Mutation Rate Setters/Getters
float GA::MutationRate() {
	return this->mutationRate;
}
float GA::MutationRate(float newRate) {
	return (this->mutationRate = newRate);
}


/*
Start the GA with whatever values are currently defined for
pop size, crossover rate and mutation rate
*/
void GA::Start() {

}


/*
Run the GA one time and return a char pointer containing stats from this run
*/
char* GA::RunOnce() {

	this->SelectTournament();
	this->Recombine();
	this->Mutate();
	this->generation++;
	this->CalcStatsForGeneration();
	this->indLog << "Best individual for generation " << this->generation << endl;
	this->PrintInd(this->indLog);
	sprintf(this->logLine, "%d\t%f\t%f\t%f\r\n", this->generation, 
		this->bestInd->GetFitness(), this->avgFitness, this->variance
		);
	return this->logLine;
}

/*
Print the currently best individual to a CString object and return it
*/
CString GA::PrintBest() {
	CString foo;
	list<Note>::iterator it = this->bestInd->GetNotes().begin();
	for (int i=0; i < (int)this->bestInd->GetNotes().size(); i++,it++) {
		sprintf(logLine, "Note %d: Value %d, Velocity %d, Location %d, Length %d\r\n", i, 
			it->Value(), it->Velocity(), it->Location(), it->Length()
			);
		foo += CString(logLine);
	}
	return foo;
}

/*
Print the currently best individual to the specified output stream
*/
void GA::PrintInd(ostream& os) {
	list<Note>::iterator it = this->bestInd->GetNotes().begin();
	for (int i=0; i < (int)this->bestInd->GetNotes().size(); i++,it++) {
		this->indLog << "Note " << i << ": Value " << it->Value() << ", Velocity ";
		this->indLog << it->Velocity() << ", Location " << it->Location() ;
		this->indLog << ", Length " << it->Length() << endl;
	}
	this->indLog << endl;
}

/*
Write out the current best individual to a Type 0 MIDI file with name _fileName_
*/
void GA::WriteMidiFile(CString fileName)
{
	char file[255];
	strcpy(file, fileName);
	MidiFileWriter fileWriter(file);
	fileWriter.resolution = this->instrument->GetResolution();
	if (this->instrument->GetType() == InstrumentType::InstTypes::KitInstrument)
		fileWriter.channel = 0x09;
	else
		fileWriter.channel = 0x00;
	for (int i=0; i < this->instrument->GetResolution(); i++)
	{
		list<Note>::iterator it = this->bestInd->GetNotes().begin();
		for (int j=0; j< (int)this->bestInd->GetNotes().size(); j++, it++)
		{
			if (it->Location() == i)
			{
				fileWriter.AddNote(&(*it));
			}
		}
	}
	fileWriter.WriteHeader();
	fileWriter.WriteTrack();
}

// Get/Set instrument
InstrumentType* GA::GetInstrument(void)
{
	return this->instrument;
}
void GA::SetInstrument(InstrumentType* newInst)
{
	this->instrument = newInst;
}

// Perform selection using tourament model
void GA::SelectTournament(void)
{
	this->matingPool = new Individual[this->popSize];
	for (int j=0; j < this->popSize; j+=2) {
		Individual tournamentPool[7];
		for (int i=0; i < 7; i++) {
			tournamentPool[i] = this->population[this->rndGen.randInt(this->popSize-1)];
		}
		Individual *best;
		Individual *secondBest;
		best = &(tournamentPool[0]);
		secondBest = &(tournamentPool[0]);
		for (int i=0; i<7; i++) {
			if (tournamentPool[i].GetFitness() > best->GetFitness()) {
				secondBest = best;
				best = &(tournamentPool[i]);
			}
		}
		this->matingPool[j]= Individual(*best);
		this->matingPool[j+1] = Individual(*secondBest);
	}
}

// Recombine the mated population
void GA::Recombine(void)
{
	delete [] this->population;
	this->population = new Individual[this->popSize];
	for (int i=0; i < this->popSize; i+=2) {
		Individual& parent1 = this->matingPool[this->rndGen.randInt(this->popSize-1)];
		Individual& parent2 = this->matingPool[this->rndGen.randInt(this->popSize-1)];
		if (this->rndGen.randInt(99) < this->crossoverRate*100) {
			this->instrument->Crossover(parent1.GetNotes(), parent2.GetNotes());
			this->population[i] = Individual(parent1);
			this->population[i+1] = Individual(parent2);
		}
	}
	delete [] this->matingPool;
}

// Mutate the population
void GA::Mutate(void)
{
	for (int i=0; i < this->popSize; i++) {
		this->instrument->Mutate(this->population[i].GetNotes(),this->mutationRate);
	}
}
/* Original version
// Calculate the statistics for the current generation
void GA::CalcStatsForGeneration(void)
{
	float average=0.0;
	for (int i=0; i <this->popSize; i++) {
		this->population[i].SetFitness(this->instrument->Evaluate(this->population[i].GetNotes()));
	}
	this->bestInd = &(this->population[0]);
	for (int i=0; i < this->popSize; i++) {
		if (this->population[i].GetFitness() > this->bestInd->GetFitness())
			this->bestInd = &(this->population[i]);
		average += this->population[i].GetFitness();
	}
	this->avgFitness = average/this->popSize;
	this->variance = 0.0;
	for (int i=0; i < this->popSize; i++) {
		this->variance += pow(this->population[i].GetFitness()-average, 2);
	}
	this->variance /= this->popSize-1;
}
*/

// Calculate the statistics for the current generation
void GA::CalcStatsForGeneration(void)
{
	float average=0.0;
	float antiConvergenceWeight=0.0;

	if (this->includeDifference)
	{
		for (int i=0; i <this->popSize; i++) {
			float differenceSum=0.0;

			for (int j=0; j <this->popSize; j++)
				if(i!=j)
					differenceSum += CalcDifference(this->population[i], this->population[j]);

			float averageDifference = differenceSum/((this->popSize)-1);

			float fitness = this->instrument->Evaluate(this->population[i].GetNotes());
			fitness += averageDifference*antiConvergenceWeight;	// Addition, since we want to reward differences
			this->population[i].SetFitness(fitness);
		}
	}
	else
	{
		for (int i=0; i<this->popSize; i++)
			this->population[i].SetFitness(this->instrument->Evaluate(this->population[i].GetNotes()));
	}
	this->bestInd = &(this->population[0]);
	for (int i=0; i < this->popSize; i++) {
		if (this->population[i].GetFitness() > this->bestInd->GetFitness())
			this->bestInd = &(this->population[i]);
		average += this->population[i].GetFitness();
	}
	this->avgFitness = average/this->popSize;
	this->variance = 0.0;
	for (int i=0; i < this->popSize; i++) {
		this->variance += pow(this->population[i].GetFitness()-average, 2);
	}
	this->variance /= this->popSize-1;
}

float GA::CalcDifference(Individual ind1, Individual ind2) {
	// This function compares the note lists of the two passed individuals. Note how the comparison
	// is performed. It is recognized that two note lists will look more or less similar, depending
	// on how they are lined up relative to each other. So, each possible way to position the lists
	// relative to each other is checked. For each position, a distance is computed. The minimum
	// distance will be obtained where the two lists line up best. This minimum distance is returned
	// as the "difference" between the two lists.


	int size1=(int)ind1.GetNotes().size();
	int size2=(int)ind2.GetNotes().size();
	int largerSize=0, smallerSize=0, smaller=0;
	
	if(size1<size2) {
		smaller = 1;
		largerSize = size2;
		smallerSize = size1;
	} else {
		smaller = 2;
		largerSize = size1;
		smallerSize = size2;
	}

	int lastPosition = largerSize - smallerSize + 1;

	list<Note>::iterator it1 = ind1.GetNotes().begin();
	list<Note>::iterator it2 = ind2.GetNotes().begin();
	float minDistance=0.0;
	
	// Change these weights to adjust how similarity is calculated
	float valueWeight=1.0, velocityWeight=1.0, locationWeight=1.0, lengthWeight=1.0;

	// For each possible position of the smaller list alongside the larger list...
	for(int p=0; p<lastPosition; p++) {

		// Slide the smaller list relative to the larger list
		if(smaller==1)
			for (int n=0; n<p; n++)
				it2++;
		else
			for (int n=0; n<p; n++)
				it1++;

		// Now that the lists are in position, compare them note by note. Weighted Euclidean
		// distance is used as the metric.
		float sum=0.0, distanceTotal=0.0;
		for (int i=0; i < smallerSize; i++,it1++,it2++) {
			sum += lengthWeight*pow((it1->Length() - it2->Length()),2);
			sum += valueWeight*pow((it1->Value() - it2->Value()),2);
			sum += velocityWeight*pow((it1->Velocity() - it2->Velocity()),2);
			sum += locationWeight*pow((it1->Location() - it2->Location()),2);
			distanceTotal += sqrt(sum);
		}

		// Keep track of the lowest distance (i.e., from the position that maximized similarity)
		if((p==0)||(distanceTotal<minDistance))
			minDistance = distanceTotal;
	}

	// Return the distance (difference) between note lists, when optimally positioned
	return minDistance;
}


