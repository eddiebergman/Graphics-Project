#pragma once

#include <string>

/*
	Namespace for handling conversions from a Note Enum to its string representations and its
	numerical representations
*/
namespace Note {

	static const unsigned NOTE_COUNT = 12;
	static const char * const notes[NOTE_COUNT] = { "C", "D", "E", "F", "G", "A"
		, "B" , "Db" , "Eb", "Gb", "Ab", "Bb" };

	static enum class T {
		C, D, E, F, G, A, 
		B, Db, Eb, Gb, Ab, Bb
	};

	static Note::T noteFromIndex(unsigned i) {
		return static_cast<Note::T>(i);
	}

	static std::string stringFromNote(Note::T note) {
		return notes[static_cast<int>(note)];
	}

	static std::string stringFromIndex(unsigned i) {
		return notes[i];
	}

	static Note::T noteFromString(std::string note) {
		for (unsigned i = 0; i < NOTE_COUNT; i++) {
			if (notes[i] == note)
				return noteFromIndex(i);
		}
		
		exit(0); //Error handling?
		return Note::T::C;
	}




}