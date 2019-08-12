#include "StdAfx.h"
#include "kitinstrument.h"
#include <iterator>


// Constructor
KitInstrument::KitInstrument(void)
{
}

// Destructor
KitInstrument::~KitInstrument(void)
{
}

// Initialize an individual
void KitInstrument::Initialize(list<Note>& notes)
{
	int coinToss = 0;
	// for each possible note location
	for (int i=0; i < this->resolution; i++)
	{	// for each possible note type
		for (int j=0; j<4; j++)
		{	// do we add a note?
			coinToss = this->rndGen.randInt(99);
			if (coinToss < 50) {
				// add with random velocity and value
                int r = this->rndGen.randInt(Note::MaxVelocityValue);
				if (r != Note::VelocityValues::NoVelocity) {
					Note newNote;
					newNote.SetVelocityFromInt(r);
					newNote.Value(this->GetNoteValue(this->rndGen.randInt(3)));
					newNote.Location(i);
					newNote.Length(1);
					notes.push_back(newNote);
				}
			}
		}
	}
}

// calculate the fitness for an individual
float KitInstrument::Evaluate(list<Note>& notes)
{
	float fitness =0.0;
	fitness += this->kickOnBeatCount(notes)*(this->kickOnBeatWeight/100.0);
	fitness += this->numSnareOnBeat(notes)*(this->snareOnBeatWeight/100.0);
	fitness += this->numCymbNotes(notes)*(this->numCymbNotesWeight/100.0);
	fitness += this->numPercNotes(notes)*(this->numPercNotesWeight/100.0);
	fitness += this->numSnareOnBeat(notes)*(this->snareOnBeatWeight/100.0);
	fitness += this->numLoudSnareNotes(notes)*(this->accentSnareWeight/100.0);
	fitness += this->numLoudKickNotes(notes)*(this->accentKickWeight/100.0);
	fitness += this->numLoudClosedHHNotes(notes)*(this->accentClosedHHWeight/100.0);
	if (this->numNotesTotal(notes) == 0)
		fitness *= -.20;
	if (this->openHHCount(notes)*2 > this->closedHHCount(notes))
		fitness *= -.20;
	if (this->numDuplicateNotes(notes) > 1)
		fitness *= -.20;
	//fitness += this->numNotesTotal(notes)*(-1);
	return fitness;
}

// mutate an individual
void KitInstrument::Mutate(list<Note>& genome, float pMut)
{
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
				int changeWhat = this->rndGen.randInt(1);
				if (changeWhat == 0) { //change value
					int randValue = this->rndGen.randInt(3);
					Note tmpNote = *it;
					tmpNote.Value(this->GetNoteValue(randValue));
					int numTries = 0;
					while (DoesNoteExist(genome, tmpNote) == true && numTries < 4) {
						randValue = this->rndGen.randInt(3);
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
			}
		}
	}
}

// return a note value from an int
int KitInstrument::GetNoteValue(int randInt) {
	switch (randInt) {
		case 0: {
			return KitInstrument::KickNote; // Kick Note
			break;
				}
		case 1: {
			return KitInstrument::SnareNote; // Snare Note
			break;
				}
		case 2: {
			return KitInstrument::HHClosedNote; //HiHat Close
			break;
				}
		case 3: {
			return KitInstrument::HHOpenNote; // HiHAt Open
			break;
				}
		default: {
			return 0;
				 }
	}
}

// crossover two individuals
void KitInstrument::Crossover(list<Note>& mom, list<Note>& dad)
{
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
		sis.push_back(*iterMom);  // add mom's values to the sister
		iterMom++;
	}
	while (iterDad->Location() < site) 
		iterDad++;
	while (*iterDad != dad.back()) {
		sis.push_back(*iterDad);  // add dad's values to the sister
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


// Get/Set resolution
int KitInstrument::GetResolution(void)
{
	return this->resolution;
}
void KitInstrument::SetResolution(int newRes)
{
	resolution = newRes;
}

// the # of times that any hi hat note is played on the same beat
int KitInstrument::HiHatClashCount(list<Note>& notes)
{
	int clashCount = 0;
	int lastLocation = 0;
	bool seenHatBefore = false;
	list<Note>::iterator it = notes.begin();
	for (int i=0; i < this->numNotesTotal(notes); i++, it++) {
		if (it->Location() == lastLocation) {
			if ((it->Value() == KitInstrument::HHClosedNote || 
				it->Value() == KitInstrument::HHOpenNote) && 
				seenHatBefore == true) {
				clashCount++;
			}
			else if ((it->Value() == KitInstrument::HHClosedNote || 
				it->Value()==KitInstrument::HHOpenNote) && 
				seenHatBefore == false)
				seenHatBefore = true;
		}
		else
		{
			seenHatBefore = false;
			lastLocation = it->Location();
		}
	}
	return clashCount;
}

// the number of notes that are considered duplicates
int KitInstrument::numDuplicateNotes(list<Note>& notes) {
	list<Note>::iterator it = notes.begin();
	list<Note>::iterator it2 = notes.begin();
	int numDuplicates = 0;
	it++;
	for (int i=0; i < this->numNotesTotal(notes)-1; i++) {
		if (*it == *it2)
			numDuplicates++;
		it++, it2++;
	}
	return numDuplicates;
}


// does the specified note _newNote_ exist in the list _notes_
bool KitInstrument::DoesNoteExist(list<Note>& notes, Note& newNote) {
	list<Note>::iterator it = notes.begin();
	for (int i=0; i < this->numNotesTotal(notes); i++, it++) {
		if ((it->Value() == newNote.Value()) && (it->Location() == newNote.Location()))
			return true;
	}
	return false;
}


// the # of snare hits that occur on an odd beat (even location)
int KitInstrument::numSnareOnBeat(list<Note>& notes)
{
	int snareOnBeatCount = 0;
	list<Note>::iterator it = notes.begin();
	for (int i=0; i<this->numNotesTotal(notes); i++, it++) {
		Note n = *it;
		if (it->Value() == KitInstrument::SnareNote && it->Location()%2 != 0)
			snareOnBeatCount++;
	}
	return snareOnBeatCount;		
}

// the # of kick notes that occur on an even beat (odd location)
int KitInstrument::kickOnBeatCount(list<Note>& notes)
{
	int kickOnBeatCount = 0;
	list<Note>::iterator it = notes.begin();
	for (int i=0; i<this->numNotesTotal(notes); i++, it++) {
		if (it->Value() == KitInstrument::KickNote && it->Location()%2 == 0)
			kickOnBeatCount++;
	}
	return kickOnBeatCount;	
}

// the # of open hihat notes
int KitInstrument::openHHCount(list<Note>& notes)
{
	int openHHCount = 0;
	list<Note>::iterator it = notes.begin();
	for (int i=0; i<this->numNotesTotal(notes); i++, it++) {
		if (it->Value() == KitInstrument::HHOpenNote)
			openHHCount++;
	}
	return openHHCount;	
}

// the # of closed hi hat notes
int KitInstrument::closedHHCount(list<Note>& notes)
{
	int closedHHCount = 0;
	list<Note>::iterator it = notes.begin();
	for (int i=0; i<this->numNotesTotal(notes); i++, it++) {
		if (it->Value() == KitInstrument::HHClosedNote)
			closedHHCount++;
	}
	return closedHHCount;	
}

// the total # of notes in the list
int KitInstrument::numNotesTotal(list<Note>& notes)
{
	return (int)notes.size();
}

// the # of notes that are percussion notes
int KitInstrument::numPercNotes(list<Note>& notes)
{
	int numNotes = 0;
	list<Note>::iterator it = notes.begin(); 
	for (int i=0; i < this->numNotesTotal(notes); i++, it++)
		if (it->Value() == KitInstrument::KickNote || 
			it->Value() == KitInstrument::SnareNote)
			numNotes++;
	return numNotes;
}

// the # of notes that are cymbal notes
int KitInstrument::numCymbNotes(list<Note>& notes)
{
	int numNotes = 0;
	list<Note>::iterator it = notes.begin(); 
	for (int i=0; i < this->numNotesTotal(notes); i++, it++)
		if (it->Value() == KitInstrument::HHClosedNote || 
			it->Value() == KitInstrument::HHOpenNote)
			numNotes++;
	return numNotes;
}

// the # of snare notes that are at least Mezzo Forte in velociy
int KitInstrument::numLoudSnareNotes(list<Note>& notes)
{
	int numNotes = 0;
	list<Note>::iterator it = notes.begin(); 
	for (int i=0; i < this->numNotesTotal(notes); i++, it++)
		if (it->Value() == KitInstrument::SnareNote && 
			it->Velocity() >= Note::VelocityValues::MezzoForte)
			numNotes++;
	return numNotes;
}

// the # of kick notes that are at least Mezzzo Forte in Velocity
int KitInstrument::numLoudKickNotes(list<Note>& notes)
{
	int numNotes = 0;
	list<Note>::iterator it = notes.begin(); 
	for (int i=0; i < this->numNotesTotal(notes); i++, it++)
		if (it->Value() == KitInstrument::KickNote && 
			it->Velocity() >= Note::VelocityValues::MezzoForte)
			numNotes++;
	return numNotes;
}

// the # of hi hat notes that are at least Forte in velocity
int KitInstrument::numLoudClosedHHNotes(list<Note>& notes)
{
	int numNotes = 0;
	list<Note>::iterator it = notes.begin(); 
	for (int i=0; i < this->numNotesTotal(notes); i++, it++)
		if (it->Value() == KitInstrument::HHClosedNote && 
			it->Velocity() >= Note::VelocityValues::Forte)
			numNotes++;
	return numNotes;
}

// Get/Set the NumPercNotesWeight
int KitInstrument::GetNumPercNotesWeight() {
	return this->numPercNotesWeight;
}
int KitInstrument::SetNumPercNotesWeight(int newWeight) {
	return this->numPercNotesWeight = newWeight;
}

// Get/Set the NumCymbNotesWeight
int KitInstrument::GetNumCymbNotesWeight() {
	return this->numCymbNotesWeight;
}
int KitInstrument::SetNumCymbNotesWeight(int newWeight) {
	return this->numCymbNotesWeight = newWeight;
}

// Get/Set the KickOnBeatWeight
int KitInstrument::GetKickOnBeatWeight() {
	return this->kickOnBeatWeight;
}
int KitInstrument::SetKickOnBeatWeight(int newWeight) {
	return this->kickOnBeatWeight = newWeight;
}

// Get/Set the SnareOnBeatWeight
int KitInstrument::GetSnareOnBeatWeight() {
	return this->snareOnBeatWeight;
}
int KitInstrument::SetSnareOnBeatWeight(int newWeight) {
	return this->snareOnBeatWeight = newWeight;
}

// Get/Set the SnareOnBeatWeight
int KitInstrument::GetAccentSnareWeight() {
	return this->accentSnareWeight;
}
int KitInstrument::SetAccentSnareWeight(int newWeight) {
	return this->accentSnareWeight = newWeight;
}

// Get/Set the AccentKickWeight
int KitInstrument::GetAccentKickWeight() {
	return this->accentKickWeight;
}
int KitInstrument::SetAccentKickWeight(int newWeight) {
	return this->accentKickWeight = newWeight;
}

// Get/Set the AccentClosedHHWeight
int KitInstrument::GetAccentClosedHHWeight() {
	return this->accentClosedHHWeight;
}
int KitInstrument::SetAccentClosedHHWeight(int newWeight) {
	return this->accentClosedHHWeight = newWeight;
}

InstrumentType::InstTypes KitInstrument::GetType() {
	return InstrumentType::InstTypes::KitInstrument;
}
