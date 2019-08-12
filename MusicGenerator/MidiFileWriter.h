/***********************************************************************
Author:		John Pinkerton
File:		BassIntrument.h
Purpose:	library routines used to write MIDI data files.
***********************************************************************/
#pragma once

#include <stdio.h>
#include <list>
#include <string>
#include "Note.h"

class MidiFileWriter
{
private:
	// the output file
	FILE *outFile;
	// the notes to write
	std::list<Note*> noteList;
	
public:
	// number of notes per bar
	int resolution;
	// midi channel to use
	char channel;
	// constructor/destructor
	MidiFileWriter(void);
	MidiFileWriter(char* fileName);
	~MidiFileWriter(void);
	// write the midi header
	void WriteHeader(void);
	// add a note to the note list
	void AddNote(Note *note);
	// write the midi track data
	void WriteTrack(void);
	// reverse the bytes (endian conversion)
	unsigned short ReverseBytes(unsigned short val);
	unsigned long ReverseBytes(unsigned long val);
	// write a variable length quantity as defined in the MIDI spec
	int WriteVarLen(unsigned long value, unsigned char* buf);
	// are there notes that have not been stopped?
	bool MoreOpenNotes(std::list<Note*> &openList, int currLoc);
	// are there notes that need to be played?
	bool MoreNewNotes(std::list<Note*> &openList, int currLoc);
};

// MIDI File header
struct MTHD_CHUNK
{
   /* Here's the 8 byte header that all chunks must have */
   char           ID[4];  /* This will be 'M','T','h','d' */
   unsigned long  Length; /* This will be 6 */

   /* Here are the 6 bytes */
   unsigned short Format;
   unsigned short NumTracks;
   unsigned short Division;
};

// MIDI File track data
struct MTRK_CHUNK
{
   /* Here's the 8 byte header that all chunks must have */
   char           ID[4];   /* This will be 'M','T','r','k' */
   unsigned long  Length;  /* This will be the actual size of Data[] */

   /* Here are the data bytes */
   unsigned char*  Data;  /* Its actual size is Data[Length] */
};
