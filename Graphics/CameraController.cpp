#include "CameraController.h"
#include "InputHandler.h"
#include "Timer.h"

const float DEFAULT_SPEED = 0.01f;

CameraController::CameraController(Camera& camera, InputHandler& inputHandler)
	: mCamera(camera), mInputHandler(inputHandler)
	, mVerticalSpeed(DEFAULT_SPEED), mHorizontalSpeed(DEFAULT_SPEED), mForwardSpeed(DEFAULT_SPEED)
{
	//Set up Camera controller
	inputHandler.onKeyPress([=](auto key,auto x, auto y) {cameraMovementHandler(key, x, y); });
}

void CameraController::cameraMovementHandler(unsigned char key, int x, int y)
{
	float timeDiff = Timer::getInstance().REFRESH_RATE;
	//Could probs be optimized (less branching)
	float horizontalVel = mHorizontalSpeed * (timeDiff);
	float verticalVel = mVerticalSpeed * (timeDiff);
	float forwardVel = mForwardSpeed * (timeDiff);
	glm::vec3 translation{0.0f, 0.0f, 0.0f};
	switch (key)
	{	
	case 'd':
		translation = mCamera.getCameraRight() * horizontalVel;
		break;
	case 'a':
		translation = -mCamera.getCameraRight() * horizontalVel;
		break;
	case 'w':
		translation = mCamera.getCameraFront() * forwardVel;
		break;
	case 's':
		translation = -mCamera.getCameraFront() * forwardVel;
		break;
	case 'q':
		translation = mCamera.getCameraUp() * verticalVel;
		break;
	case 'e':
		translation = -mCamera.getCameraUp() * verticalVel;
		break;
	default:
		break;
	}
	mCamera.translateCamera(translation);
}

void CameraController::setUniformSpeed(GLfloat speed)
{
	setVerticalSpeed(speed);
	setHorizontalSpeed(speed);
	setForwardSpeed(speed);
}

void CameraController::setVerticalSpeed(GLfloat speed)
{
	mVerticalSpeed = speed;
}

void CameraController::setForwardSpeed(GLfloat speed)
{
	mForwardSpeed = speed;
}

GLfloat CameraController::getForwardSpeed()
{
	return mForwardSpeed;
}

GLfloat CameraController::getVerticalSpeed()
{
	return mVerticalSpeed;
}

void CameraController::setHorizontalSpeed(GLfloat speed)
{
	mHorizontalSpeed = speed;
}

GLfloat CameraController::getHorizontalSpeed()
{
	return mHorizontalSpeed;
}