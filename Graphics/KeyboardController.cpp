#include "KeyboardController.h"

#include "Note.h"

KeyboardController::KeyboardController(Keyboard& keyboard, Sphere& sphere, InputHandler& inputHandler)
	: rKeyboard(keyboard), rInputHandler(inputHandler), rSphere(sphere), mOctave(0)
{
	//TODO: Change this to also have on keyup
	inputHandler.onKeyPress([=](unsigned char key, int x, int y) {
		int amount = 128;
		switch (key) {
		case ',': mOctave -= (mOctave > 0) ? 1 : 0; break;
		case '.': mOctave += (mOctave < 5) ? 1 : 0; break;
		case 'r': 
			rKeyboard.onKeyPressed(mOctave * 12 + 1, amount); 
			rSphere.activateNote(Note::T::C, amount);
			break;
		case '5': 
			rKeyboard.onKeyPressed(mOctave * 12 + 2, amount); 
			rSphere.activateNote(Note::T::Db, amount);
			break;
		case 't': 
			rKeyboard.onKeyPressed(mOctave * 12 + 3, amount);
			rSphere.activateNote(Note::T::D, amount);
			break;
		case '6': 
			rKeyboard.onKeyPressed(mOctave * 12 + 4, amount); 
			rSphere.activateNote(Note::T::Eb, amount);
			break;
		case 'y': 
			rKeyboard.onKeyPressed(mOctave * 12 + 5, amount); 
			rSphere.activateNote(Note::T::E, amount);
			break;
		case 'u': 
			rKeyboard.onKeyPressed(mOctave * 12 + 6, amount); 
			rSphere.activateNote(Note::T::F, amount);
			break;
		case '8': 
			rKeyboard.onKeyPressed(mOctave * 12 + 7, amount); 
			rSphere.activateNote(Note::T::Gb, amount);
			break;
		case 'i': 
			rKeyboard.onKeyPressed(mOctave * 12 + 8, amount);
			rSphere.activateNote(Note::T::G, amount);
			break;
		case '9': 
			rKeyboard.onKeyPressed(mOctave * 12 + 9, amount);
			rSphere.activateNote(Note::T::Ab, amount);
			break;
		case 'o': 
			rKeyboard.onKeyPressed(mOctave * 12 + 10, amount);
			rSphere.activateNote(Note::T::A, amount);
			break;
		case '0': 
			rKeyboard.onKeyPressed(mOctave * 12 + 11, amount);
			rSphere.activateNote(Note::T::Bb, amount);
			break;
		case 'p': 
			rKeyboard.onKeyPressed(mOctave * 12 + 12, amount); 
			rSphere.activateNote(Note::T::B, amount);
			break;
		}
	});
}


KeyboardController::~KeyboardController()
{
}
