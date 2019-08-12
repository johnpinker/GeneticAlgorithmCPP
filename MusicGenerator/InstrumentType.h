/***********************************************************************
Author:		John Pinkerton
File:		IntrumentType.h
Purpose:	Interface used to define an Instrument.  Implement this to
			create a new intrument type.
***********************************************************************/
#pragma once
#include "Note.h"
#include <list>

using namespace std;

interface InstrumentType
{
public:
	// types of instruments currently defined
	enum InstTypes {
		KitInstrument,
		BassInstrument
	};
	// method prototypes that must be implemented
	virtual void Initialize(list<Note>& genome) = 0;
	virtual float Evaluate(list<Note>& genome) = 0;
	virtual void Mutate(list<Note>& genome, float pMut) = 0;
	virtual void Crossover(list<Note>& mom, list<Note>& dad) = 0;
	virtual int GetResolution(void) =0;
	virtual void SetResolution(int newRes) = 0;
	virtual InstTypes GetType() = 0;
};
