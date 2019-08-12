/***********************************************************************
Author:		John Pinkerton
File:		BassIntrument.h
Purpose:	Header file for the implementation details of the BassInstrument
			instrument.
***********************************************************************/
#pragma once
#include "instrumenttype.h"
#include "MersenneTwister.h"

class BassInstrument :
	public InstrumentType
{
private:
	int resolution;	//# of notes per bar
	MTRand rndGen;	//random # generator
	enum Notes {	//Note value definition
		C = 60,
		Cs = 61,
		D = 62,
		Ds = 63,
		E = 64,
		F = 65,
		Fs = 66,
		G = 67,
		Gs = 68,
		A = 69,
		As = 70,
		B = 71
	};

	// Get a value for a note (from the Notes enum) from a corresponding int
	int GetNoteValue(int randInt);
	// return the total number of notes in a list
	int numNotesTotal(list<Note> notes);
	// does _newNote_ exist in the supplied note list?
	bool DoesNoteExist(list<Note>& notes, Note& newNote);
	// number of notes that fit the consonance description
	int numConsonantNotes(list<Note> & notes);
	// number of notes that fit the dissonance description
	int numDissonantNotes(list<Note> & notes);
	// number of notes that are not in the current key
	int numNotInKey(list<Note> & notes);
	// number of notes that are not on an odd beat
	int numNotesOnOddBeat(list<Note> & notes);
	// number of notes that are not on an even beat
	int numNotesOnEvenBeat(list<Note> & notes);
public:
	// constructor
	BassInstrument(void);
	// destructor
	~BassInstrument(void);
	// Initialize an individual for the bass instrument
	void Initialize(list<Note>& genome);
	// Evaluate an individual for the bass instrument
	float Evaluate(list<Note>& genome);
	// Mutate an individual with _pMut_ probability
	void Mutate(list<Note>& genome, float pMut);
	// Crossover _mom_ and _dad_
	void Crossover(list<Note>& mom, list<Note>& dad);
	// get/set the resolution
	int GetResolution(void);
	void SetResolution(int newRes);
	// Which type of instrument are we?
	InstrumentType::InstTypes GetType();
	// weight for the number of notes
	float numNotesWeight;
	// weight for the dissonance factor
	float disonanceWeight;
	// weight for the consonance factor
	float consonanceWeight;
	// current key (0 is major, 1 is minor)
	int key;
	// weight for the even accent factor
	int evenAccentWeight;
	// weight for the odd accent factor
	int oddAccentWeight;

};
