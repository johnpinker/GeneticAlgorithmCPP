/***********************************************************************
Author:		John Pinkerton
File:		Note.h
Purpose:	The definition of a Note object.
***********************************************************************/
#pragma once

class Note
{

private:
	//int duration;
	int value;
	int velocity;
	int location;
	int length;
public:
	// constructor/destructor/copy constructor
	Note(void);
	~Note(void);
	Note(const Note& newNote);
	// get/set value
	int Value();
	int Value(int newVal);
	// get/set velocity
	int Velocity();
	int Velocity(int newVel);
	// get/set length
	int Length();
	int Length(int newLen);
	// operator definitions
	bool operator!=(Note& compNote);
	bool operator==(Note& compNote);
	// allowable veolcity values
	enum VelocityValues {
		NoVelocity = 0,
		Piano = 0x20,
		MezzoForte = 0x40,
		Forte = 0x7F
	};
	// how many velocity values do we have?
	const static int MaxVelocityValue = 3;
	// set the veocity value from an in [0,MaxVelocityValue]
	void SetVelocityFromInt(int vel);
	// get/set location
	int Location();
	int Location(int newLocation);
};
