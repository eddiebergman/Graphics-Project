# pragma once

#include <GL/freeglut.h>
#include <functional>
#include <vector>

//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

/*
	A wrapper around freeGluts input handling, allows us to have objects as callbacks to events rather than plain
	static functions
*/
class InputHandler {

public:

	//Use this method to access an instance of the InputHandler
	static InputHandler& getInstance(){
		static InputHandler instance;
		return instance;
	}

private:

	//Vectors of callbacks to call once an event is registered in the handler
	std::vector<std::function<void(int, int, int, int)>> mMousePressObservers;
	std::vector<std::function<void(unsigned char, int, int)>>  mKeyPressObservers;
	std::vector<std::function<void(int, int)>> mMouseMoveActiveObservers;
	std::vector<std::function<void(int, int)>> mMouseMovePassiveObservers;

public:

	//Registers the InputHandler with Glut
	void setupEventListening();

	/*
		Register a callback for when the mouse is pressed
		Params: button, state(up/down), mouse_x, mouse_y
	*/
	void onMousePress(std::function<void(int, int, int, int)> callback);

	/*
		Register a callback for when a key is pressed
		Params: keypressed, mouse_x, mouse_y
	*/
	void onKeyPress(std::function<void(unsigned char, int, int)> callback);

	/*
		Register a callback for when the mouse is moved while a mouse button is pressed
		Params: mouse_x, mouse_y
	*/
	void onMouseMoveActive(std::function<void(int, int)> callback);

	/*
		Register a callback for when the mouse is moved while no mouse button is pressed
		Params: mouse_x, mouse_y
	*/
	void onMouseMovePassive(std::function<void(int, int)> callback);


private:

	//Setup as Singleton stuff
	InputHandler();
	InputHandler(InputHandler const&) = delete;    //So we can't create new instances     
	void operator=(InputHandler const&) = delete;

	//Function handlers
	void handleMousePressed(int button, int state, int x, int y);
	void handleKeyPressed(unsigned char key, int x, int y);
	void handleMouseMoveActive(int x, int y);
	void handleMouseMovePassive(int x, int y);

};