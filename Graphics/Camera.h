#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GL/freeglut.h>

/*
	A Camera represents our view and projection matrices for a scene and keeps all
	variables and state together. You can obtain the view and projection matrices
	using the corresponding functiongs view() and projection()
*/
class Camera {

public:

	Camera(glm::vec3 position, glm::vec3 target, GLfloat aspectRatio);
	Camera(glm::vec3 position, glm::vec3 up, glm::vec3 target, GLfloat fov, GLfloat aspectRatio, GLfloat nearClip, GLfloat farClip);

	//Gets the view/projection matrices
	glm::mat4 view();
	glm::mat4 Camera::projection();

	//Get specific information about camera orientation
	glm::vec3 getPosition();
	glm::vec3 getCameraUp();
	glm::vec3 getCameraRight();
	glm::vec3 getCameraFront();

	//Alter cameras state
	void setFov(GLfloat fov);
	void setPosition(glm::vec3 newPosition);
	void faceLocation(glm::vec3 location);
	void faceDirection(glm::vec3 direction);
	glm::vec3 facingDirection();
	void translateCamera(glm::vec3 translation);
	
private:

	//Projection Matrix variables
	GLfloat mFov;
	GLfloat mAspectRatio;
	GLfloat mNearCLip;
	GLfloat mFarClip;

	//Camera variables
	glm::vec3 mWorldUp;
	glm::vec3 mCameraFront;
	glm::vec3 mCameraPosition;
	glm::vec3 mCameraRight;
	glm::vec3 mCameraUp;
};