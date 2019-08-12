/***********************************************************************
Author:		John Pinkerton
File:		KitInstrument.h
Purpose:	Definition of a DrumKitInstrument.
***********************************************************************/

#pragma once
#include "instrumenttype.h"
#include "MersenneTwister.h"

class KitInstrument :
	public InstrumentType
{
private:
	// member vars for weights, etc.
	int resolution;
	int numPercNotesWeight;
	int numCymbNotesWeight;
	int kickOnBeatWeight;
	int snareOnBeatWeight;
	int accentSnareWeight;
	int accentKickWeight;
	int accentClosedHHWeight;
	// random # generator
	MTRand rndGen;
	// the number of hi hat notes that are clashing
	int HiHatClashCount(list<Note>& notes);
	// number of snare notes on the beat
	int numSnareOnBeat(list<Note>& notes);
	// number of kick notes that are on the beat
	int kickOnBeatCount(list<Note>& notes);
	// number of total notes
	int numNotesTotal(list<Note>& notes);
	// number of notes that are percussion notes
	int numPercNotes(list<Note>& notes);
	// number of notes that are cymbal notes
	int numCymbNotes(list<Note>& notes);
	// number of snare notes that have a high velocity
	int numLoudSnareNotes(list<Note>& notes);
	// number of kick notes that have a high velocity
	int numLoudKickNotes(list<Note>& notes);
	// number of closed hi hat notes that heva a high velocity
	int numLoudClosedHHNotes(list<Note>& notes);
	// number of closed hi hat notes
	int closedHHCount(list<Note>& notes);
	// number of open hi hat notes
	int openHHCount(list<Note>& notes);
	// number of notes that are duplicates
	int numDuplicateNotes(list<Note>& notes);
	// does the specified note exist?
	bool DoesNoteExist(list<Note>& notes, Note& newNote);
public:
	// constructor/destructor
	KitInstrument(void);
	~KitInstrument(void);
	// initialize the provided genome
	void Initialize(list<Note>& genome);
	// evaluate the provided genome
	float Evaluate(list<Note>& genome);
	// mutate the provided genome
	void Mutate(list<Note>& genome, float pMut);
	// crossover _mom_ and _dad_
	void Crossover(list<Note>& mom, list<Note>& dad);
	// get/set weights
	int GetResolution(void);
	void SetResolution(int newRes);
	int GetNoteValue(int randInt);
	int GetNumPercNotesWeight();
	int SetNumPercNotesWeight(int newWeight);
	int GetNumCymbNotesWeight();
	int SetNumCymbNotesWeight(int newWeight);
	int GetKickOnBeatWeight();
	int SetKickOnBeatWeight(int newWeight);
	int GetSnareOnBeatWeight();
	int SetSnareOnBeatWeight(int newWeight);
	int GetAccentSnareWeight();
	int SetAccentSnareWeight(int newWeight);
	int GetAccentKickWeight();
	int SetAccentKickWeight(int newWeight);
	int GetAccentClosedHHWeight();
	int SetAccentClosedHHWeight(int newWeight);
	// return the type of this instrument
	InstrumentType::InstTypes GetType();
	// definitions for the note values
	const static int KickNote = 36;
	const static int SnareNote = 38;
	const static int HHClosedNote = 44;
	const static int HHOpenNote = 46;
};
