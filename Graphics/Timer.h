#pragma once
#include <GL/freeglut.h>

/*
	A singleton class for use throughout the program wwhich keeps track of 
	how many frames have been rendered
*/
class Timer
{
public:
	const unsigned REFRESH_RATE = 1000 / 60;
	unsigned framesRendered = 0;
		
	static Timer& getInstance() {
		static Timer instance;
		return instance;
	};

	void start() {
		ticker();
	}

	void ticker() {
		framesRendered++;
		glutPostRedisplay();
		glutTimerFunc(REFRESH_RATE, [](int){ Timer::getInstance().ticker(); }, 0);
	}

	~Timer() = default;

private:
	//Setup as Singleton stuff
	Timer() {};
	Timer(Timer const&) = delete;    //So we can't create new instances     
	void operator=(Timer const&) = delete;

	
};
