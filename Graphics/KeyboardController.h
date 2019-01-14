#pragma once

#include "Keyboard.h"
#include "Sphere.h"
#include "InputHandler.h"

/*
	Controls a Keyboard and Sphere, informing them which note has been pressed
	and which octave its on. Should probably have a seperate controller for 
	the Sphere
*/
class KeyboardController
{
public:
	KeyboardController(Keyboard& keyboard, Sphere& sphere, InputHandler& inputHandler);
	~KeyboardController();

private:
	unsigned mOctave;
	Keyboard& rKeyboard;
	Sphere& rSphere;
	InputHandler& rInputHandler;
};

