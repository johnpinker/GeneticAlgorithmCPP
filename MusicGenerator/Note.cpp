#include "StdAfx.h"
#include "note.h"

/*
Constructor
*/
Note::Note(void)
{
}

/*
Destructor
*/
Note::~Note(void)
{
}

// Copy constructor
Note::Note(const Note& newNote) {
	this->length = newNote.length;
	this->location = newNote.location;
	this->value = newNote.value;
	this->velocity = newNote.velocity;
}


// Getter/Setter for value
int Note::Value() {
	return this->value;
}
int Note::Value(int newVal) {
	return (this->value = newVal);
}

// Getter/Setter for velocity
int Note::Velocity() {
	return this->velocity;
}
int Note::Velocity(int newVel) {
	return (this->velocity = newVel);
}

// Getter/Setter for length
int Note::Length() {
	return this->length;
}
int Note::Length(int newLen) {
	return (this->length = newLen);
}

// Getter/Setter for location
int Note::Location() {
	return this->location;
}
int Note::Location(int newLocation) {
	return (this->location = newLocation);
}

// overloaded operators
bool Note::operator!=(Note &compNote) {
	return !(*this == compNote);
}
bool Note::operator==(Note& compNote) {
	if (this->value == compNote.value &&
		this->velocity == compNote.velocity &&
		this->location == compNote.location)
		return true;
	else
		return false;
}
/*
Set the velocity to an appropriate value for one of
Piano, Mezzo Forte or Forte from a value from 0 to 3
*/
void Note::SetVelocityFromInt(int vel) {
	ASSERT(vel<=3 && vel>=0);
	if (vel == 1)
		this->velocity = Note::VelocityValues::Piano;
	else if (vel == 2)
		this->velocity = Note::VelocityValues::MezzoForte;
	else
		this->velocity = Note::VelocityValues::Forte;
}



