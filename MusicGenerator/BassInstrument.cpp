#include "StdAfx.h"
#include "bassinstrument.h"
#include ".\bassinstrument.h"

// Constructor
BassInstrument::BassInstrument(void)
: numNotesWeight(0)
, disonanceWeight(0)
, consonanceWeight(0)
, key(0)
, evenAccentWeight(0)
, oddAccentWeight(0)
{
}

// Destructor
BassInstrument::~BassInstrument(void)
{
}

// Initialize an individual to random values
void BassInstrument::Initialize(list<Note>& notes) {
	int coinToss = 0;
	// for every possible note location
	for (int i=0; i < this->resolution; i++)
	{
		//do we add a note?
		coinToss = this->rndGen.randInt(99);
		if (coinToss < 50) {
			// pick a random velocity and a random value
            int r = this->rndGen.randInt(Note::MaxVelocityValue);
			if (r != Note::VelocityValues::NoVelocity) {
				Note newNote;
				newNote.SetVelocityFromInt(r);
				newNote.Value(this->GetNoteValue(this->rndGen.randInt(11)));
				newNote.Location(i);
				newNote.Length(this->rndGen.randInt(this->resolution));
				if (newNote.Length() == 0) newNote.Length(1);
				notes.push_back(newNote);
			}
		}
	}
}

// Evaluate an individual
float BassInstrument::Evaluate(list<Note>& notes) {
	float fitness = 0.0;
	fitness += (float)(this->numConsonantNotes(notes)*(this->consonanceWeight/100.0));
	fitness += (float)(this->numDissonantNotes(notes)*(this->disonanceWeight/100.0));
	fitness += (float)(this->numNotesTotal(notes)*(this->numNotesWeight/100.0));
	fitness += (float)(this->numNotesOnEvenBeat(notes)*(this->evenAccentWeight/100.0));
	fitness += (float)(this->numNotesOnOddBeat(notes)*(this->oddAccentWeight/100.0));
	if (this->numNotInKey(notes) > 1)
		fitness *= -.20;
	return fitness;
}

//Mutate an individual
void BassInstrument::Mutate(list<Note>& genome, float pMut) {
	if (this->rndGen.randInt(99) < pMut*100 && genome.size() != 0) {
		if (this->rndGen.randInt(99) < 50) { // Erase a note
			list<Note>::iterator it = genome.begin();
			int mutatePoint = this->rndGen.randInt(this->numNotesTotal(genome)-1);
			for (int i=0; i<mutatePoint; i++)
				it++;
			genome.erase(it);
		}
		else { // Change random values in a random # of individuals
			int numToChange = this->rndGen.randInt(genome.size()-1);
			for (int i=0; i < numToChange; i++) {
				int stopAt = this->rndGen.randInt(genome.size()-1);
				list<Note>::iterator it = genome.begin();
				for (int j=0; j < stopAt; j++)
					it++;
				int changeWhat = this->rndGen.randInt(2);
				if (changeWhat == 0) { //change value
					int randValue = this->rndGen.randInt(11);
					Note tmpNote = *it;
					tmpNote.Value(this->GetNoteValue(randValue));
					int numTries = 0;
					while (DoesNoteExist(genome, tmpNote) == true && numTries < 4) {
						randValue = this->rndGen.randInt(11);
						tmpNote.Value(this->GetNoteValue(randValue));
						numTries++;
					}
					if (numTries == 4)
						return;
					it->Value(this->GetNoteValue(randValue));
				}
				else if (changeWhat == 1) { // change Velocity
					int randVelocity = this->rndGen.randInt(Note::MaxVelocityValue);
					Note tmpNote = *it;
					tmpNote.SetVelocityFromInt(randVelocity);
					int numTries =0;
					while (DoesNoteExist(genome, tmpNote) == true && numTries < 4) {
						randVelocity = this->rndGen.randInt(Note::MaxVelocityValue);
						tmpNote.SetVelocityFromInt(randVelocity);
						numTries++;
					}
					if (numTries == 4)
						return;
					it->SetVelocityFromInt(randVelocity);
				}
				else if (changeWhat == 2) { // change length
					int randLength = this->rndGen.randInt(this->resolution);
					Note tmpNote = *it;
					tmpNote.Length(randLength);
					if (tmpNote.Length() == 0) tmpNote.Length(1);
				}
			}
		}
	}
}

// Crossover two individuals
void BassInstrument::Crossover(list<Note>& mom, list<Note>& dad){
	if (mom.size() ==0 || dad.size() == 0)
		return;
	list<Note> bro;
	list<Note> sis;
	int maxLocation = mom.back().Location();
	if (dad.back().Location() < maxLocation)
		maxLocation = dad.back().Location();
	if (maxLocation == 0)
		return;
	int site = this->rndGen.randInt(maxLocation-1);
	Note& momTail = mom.back();
	Note& dadTail = dad.back();
	list<Note>::iterator iterMom;
	list<Note>::iterator iterDad;


	iterMom = mom.begin();
	iterDad = dad.begin();
	//move both parents to the chosen site
	while (iterMom->Location() < site) {
		sis.push_back(*iterMom); // add mom's values to the sister
		iterMom++;
	}
	while (iterDad->Location() < site) 
		iterDad++;
	while (*iterDad != dad.back()) {
		sis.push_back(*iterDad); // add dad's values to the sister
		iterDad++;
	}
	sis.push_back(*iterDad);
	
	//repeat the opposite way for brother
	iterMom = mom.begin();
	iterDad = dad.begin();
	while (iterDad->Location() < site) {
		bro.push_back(*iterDad);
		iterDad++;
	}
	while (iterMom->Location() < site) 
		iterMom++;
	while (*iterMom != mom.back()) {
		bro.push_back(*iterMom);
		iterMom++;
	}
	bro.push_back(*iterMom);
	//children replace parents
	mom = sis;
	dad = bro;
}

// Get/Set Resolution
int BassInstrument::GetResolution(void) {
	return this->resolution;
}
void BassInstrument::SetResolution(int newRes) {
	this->resolution = newRes;
}

// Return a note value from an int
int BassInstrument::GetNoteValue(int randInt) {
	switch (randInt) {
		case 0: {
			return BassInstrument::Notes::A;
			break;
				}
		case 1: {
			return BassInstrument::Notes::As;
			break;
				}
		case 2: {
			return BassInstrument::Notes::B;
			break;
				}
		case 3: {
			return BassInstrument::Notes::C;
			break;
				}
		case 4: {
			return BassInstrument::Notes::Cs;
			break;
				}
		case 5: {
			return BassInstrument::Notes::D;
			break;
				}
		case 6: {
			return BassInstrument::Notes::Ds;
			break;
				}
		case 7: {
			return BassInstrument::Notes::E;
			break;
				}
		case 8: {
			return BassInstrument::Notes::F;
			break;
				}
		case 9: {
			return BassInstrument::Notes::Fs;
			break;
				}
		case 10: {
			return BassInstrument::Notes::G;
			break;
				}
 		case 11: {
			return BassInstrument::Notes::Gs;
			break;
				}
		default: {
			return 0;
				 }
	}
}

// return the total number of notes in the list
int BassInstrument::numNotesTotal(list<Note> notes) {
	return notes.size();
}

// does a specific note _newNote_ exist in _notes_
bool BassInstrument::DoesNoteExist(list<Note>& notes, Note& newNote) {
	list<Note>::iterator it = notes.begin();
	for (int i=0; i < this->numNotesTotal(notes); i++, it++) {
		if (it->Location() == newNote.Location())
			return true;
	}
	return false;
}

InstrumentType::InstTypes BassInstrument::GetType() {
	return InstrumentType::InstTypes::BassInstrument;
}

/*
Return the # of notes that are either a 1, 4, 6 or (maj/min 3rd)
Assume the key of C
*/
int BassInstrument::numConsonantNotes(list<Note> & notes)
{
	int numNotes = 0;
	list<Note>::iterator it = notes.begin(); 
	for (int i=0; i < this->numNotesTotal(notes); i++, it++)
	{
		if (it->Value() == BassInstrument::Notes::C ||
			it->Value() == BassInstrument::Notes::F ||
			it->Value() == BassInstrument::Notes::A) // 1, 4 or 6
			numNotes++;
		if (this->key == 0 && it->Value() == BassInstrument::Notes::E) //major
			numNotes++;
		else if (this->key == 1 && it->Value() == BassInstrument::Notes::Ds) // minor key
			numNotes++;
	}
	return numNotes;
}

/*
Return the # of notes that are either a 2, 5 or 7
Assume the key of C
*/
int BassInstrument::numDissonantNotes(list<Note> & notes)
{
	int numNotes = 0;
	list<Note>::iterator it = notes.begin(); 
	for (int i=0; i < this->numNotesTotal(notes); i++, it++)
	{
		if (it->Value() == BassInstrument::Notes::D ||
			it->Value() == BassInstrument::Notes::G ||
			it->Value() == BassInstrument::Notes::B) // 2, 5 or 7
			numNotes++;
	}
	return numNotes;
}

/*
Number of notes not in the current key (maj/min) assumed to be in C
*/
int BassInstrument::numNotInKey(list<Note> & notes)
{
	int numNotes = 0;
	list<Note>::iterator it = notes.begin(); 
	for (int i=0; i < this->numNotesTotal(notes); i++, it++)
	{
		if (it->Value() == BassInstrument::Notes::D ||
			it->Value() == BassInstrument::Notes::G ||
			it->Value() == BassInstrument::Notes::B ||
			it->Value() == BassInstrument::Notes::C ||
			it->Value() == BassInstrument::Notes::F ||
			it->Value() == BassInstrument::Notes::A) // 1,2,4,5,6,7
			numNotes++;
		if (this->key == 0 && it->Value() == BassInstrument::Notes::E) //major
			numNotes++;
		else if (this->key == 1 && it->Value() == BassInstrument::Notes::Ds) // minor key
			numNotes++;
	}
	return numNotesTotal(notes) - numNotes;
}

int BassInstrument::numNotesOnOddBeat(list<Note> & notes)
{
	int numNotes = 0;
	list<Note>::iterator it = notes.begin();
	for (int i=0; i < this->numNotesTotal(notes); i++, it++)
	{
		if (it->Location()%1 == 0)
			numNotes++;
	}
	return numNotes;
}

int BassInstrument::numNotesOnEvenBeat(list<Note> & notes)
{
		int numNotes = 0;
	list<Note>::iterator it = notes.begin();
	for (int i=0; i < this->numNotesTotal(notes); i++, it++)
	{
		if (it->Location()%2 == 0)
			numNotes++;
	}
	return numNotes;
}
