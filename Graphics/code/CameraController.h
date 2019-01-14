#pragma once

#include <GL/freeglut.h>
#include "Camera.h"
#include "InputHandler.h"

/*
	Allows you to control a Camera object once given an input handler 
	it can register with
*/
class CameraController {
	
public:

	CameraController(Camera& camera, InputHandler &input);
	
	//Getters and setters for the cameras movement speed
	void setUniformSpeed(GLfloat speed);

	void setVerticalSpeed(GLfloat speed);
	GLfloat getVerticalSpeed();
	
	void setHorizontalSpeed(GLfloat speed);
	GLfloat getHorizontalSpeed();

	void setForwardSpeed(GLfloat speed);
	GLfloat getForwardSpeed();


private:
	Camera& mCamera;
	InputHandler& mInputHandler;

	GLfloat mHorizontalSpeed;
	GLfloat mVerticalSpeed;
	GLfloat mForwardSpeed;

	void cameraMovementHandler(unsigned char key, int x, int y);
};