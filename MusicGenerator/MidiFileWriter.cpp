#include "StdAfx.h"
#include "midifilewriter.h"


/*
Default Constructor
*/
MidiFileWriter::MidiFileWriter(void)
{
	this->outFile = fopen("out.mid", "wb");
}

/*
Overloaded Constructor - prefered form
*/
MidiFileWriter::MidiFileWriter(char* fileName)
{
	this->outFile = fopen(fileName, "wb");
}

/*
Destructor
*/
MidiFileWriter::~MidiFileWriter(void)
{
	fclose(this->outFile);
}

/*
Write the MIDI Header to our output file
*/
void MidiFileWriter::WriteHeader(void)
{
	MTHD_CHUNK headerChunk;
	strncpy(headerChunk.ID, "MThd", 4);
	headerChunk.Format = 0;
	headerChunk.NumTracks = 1;
	headerChunk.NumTracks = this->ReverseBytes(headerChunk.NumTracks);
	headerChunk.Division = 96;
	headerChunk.Division = this->ReverseBytes(headerChunk.Division);
	headerChunk.Length = 6;
	headerChunk.Length = this->ReverseBytes(headerChunk.Length);
	int size = sizeof(headerChunk.ID) + sizeof(headerChunk.Division) + 
		sizeof(headerChunk.Format) + sizeof(headerChunk.Length) + sizeof(headerChunk.NumTracks);
	fwrite(&headerChunk, size, 1, this->outFile);
}

/*
Add a Note object to the list of notes to write.
All Notes must be added prior to writing.
*/
void MidiFileWriter::AddNote(Note *newNote)
{
	this->noteList.push_back(newNote);
}

/*
Return a bool representing whether there are Notes in the list _openList_
at the time specified by _currLoc_
*/
bool MidiFileWriter::MoreOpenNotes(std::list<Note*> &openList, int currLoc) {
	Note* tmpNote = openList.front();
	if (openList.empty() == false && tmpNote->Location() + tmpNote->Length() == currLoc)
		return true;
	else 
		return false;
}

/*
Return a bool representing whether there are Notes in the list _openList_
at the time specified by _currLoc_
*/
bool MidiFileWriter::MoreNewNotes(std::list<Note*> &openList, int currLoc) {
	Note* tmpNote = openList.front();
	if (openList.empty() == false && tmpNote->Location() == currLoc)
		return true;
	else 
		return false;
}

/*
Write the data track to the MIDI file.
*/
void MidiFileWriter::WriteTrack(void)
{
	MTRK_CHUNK trackChunk;
	unsigned long bufSize = 500;
	unsigned char *tmpBuf = (unsigned char*)malloc(sizeof(unsigned char)*bufSize);
	strncpy(trackChunk.ID, "MTrk", 4);
	Note *tmpNote;
	std::list<Note*> openList;
	int lastLocation = 0;
	unsigned long count = 0;
	int lastEventTime = 0;
	int distanceFromLastEvent = 0;
	int noteLength = (96.0 * (4.0/this->resolution));
	for (int i=0; i < this->resolution+1; i++)
	{
		if (count+100 > bufSize)
		{
			bufSize += 500;
			tmpBuf = (unsigned char*)realloc(tmpBuf, bufSize);
		}
		while (MoreOpenNotes(openList, i)) {
			//Send note off messages for currently open notes
			tmpNote = openList.front();
			distanceFromLastEvent = ((tmpNote->Location()+1)*noteLength)-lastEventTime;
			count += this->WriteVarLen(distanceFromLastEvent, tmpBuf+count);
			tmpBuf[count++] = 0x90 | this->channel;
			tmpBuf[count++] = (unsigned char) tmpNote->Value();
			tmpBuf[count++] = (unsigned char) 0;
			lastEventTime = (tmpNote->Location()+1)*noteLength;
			openList.pop_front();
		}
		while (MoreNewNotes(this->noteList, i)) {
			tmpNote = this->noteList.front();
			distanceFromLastEvent = (tmpNote->Location()*noteLength)-lastEventTime;
			count += this->WriteVarLen(distanceFromLastEvent, tmpBuf+count);
			tmpBuf[count++] = 0x90 | this->channel;
			tmpBuf[count++] = (unsigned char) tmpNote->Value();
			tmpBuf[count++] = (unsigned char) tmpNote->Velocity();
			openList.push_back(tmpNote);
			lastEventTime = tmpNote->Location()*noteLength;
			this->noteList.pop_front();
		}
	}
	tmpBuf[count++] = 0x00; tmpBuf[count++] = 0xFF; tmpBuf[count++] = 0x2F; tmpBuf[count++] = 0x00;
	//trackChunk.Data = (unsigned char*)malloc(sizeof(unsigned char)*count);
	//strncpy((char*)trackChunk.Data, (char*)tmpBuf, count);
	trackChunk.Length = this->ReverseBytes(count);
	int size = sizeof(trackChunk.ID) + sizeof(trackChunk.Length) + sizeof(char)*count;
	fwrite(&trackChunk.ID, sizeof(trackChunk.ID), 1, outFile);
	fwrite(&trackChunk.Length, sizeof(trackChunk.Length), 1, outFile);
	for (int i=0; i<(int)count; i++) {
		fwrite(&tmpBuf[i], sizeof(char), 1, this->outFile);
	}
	//delete trackChunk.Data;
	delete tmpBuf;
}

/*
Write a Variable Length Value as defined in the MIDI spec to the output buffer
_buf_ where _value_ is the value to write
*/
int MidiFileWriter::WriteVarLen(unsigned long value, unsigned char* buf)
{
	unsigned char ubTemp[4] = {0x00, 0x80, 0x80, 0x80};
	int nCount = 0;
	int i,j;

	for (i=0; i < 4; i++) {
		nCount++;
		ubTemp[i] |= value & 0x7F;
		value  >>= 7;
		if (value == 0) {
			break;
		}
	}
	for (i=nCount-1, j=0; i >=0; i--, j++) {
		*(buf+j) = ubTemp[i];
	}
	return j;
}


/*
Convert the endian format of _val_ and return the converted value
*/
unsigned short MidiFileWriter::ReverseBytes(unsigned short val) {
	unsigned short tmpVal = 0;
	tmpVal = (val & 0x00FF) << 8;
	tmpVal |= (val & 0xFF00) >> 8;
	return tmpVal;
}

/*
Convert the endian format of _val_ and return the converted value
*/
unsigned long MidiFileWriter::ReverseBytes(unsigned long val) {
	unsigned long tmpVal = 0;
	tmpVal = (val & 0x000000FF) << 24;
	tmpVal |= (val & 0x0000FF00) << 8;
	tmpVal |= (val & 0x00FF0000) >> 8;
	tmpVal |= (val & 0xFF0000000) >> 24;
	return tmpVal;

}
