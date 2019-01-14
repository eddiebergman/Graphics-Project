#include "Camera.h"

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Whut did I do here? shame on me
const glm::vec3 WORLD_UP = { 0.0f, 1.0f, 0.0f };
const GLfloat FOV = 45.0f;
const GLfloat NEAR_CLIP = 0.01f;
const GLfloat FAR_CLIP = 1000.0f;

Camera::Camera(glm::vec3 position, glm::vec3 target, GLfloat aspectRatio)
	: Camera(position, target, WORLD_UP, FOV, aspectRatio, NEAR_CLIP, FAR_CLIP ) {}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, GLfloat fov, GLfloat aspectRatio, GLfloat nearClip, GLfloat farClip)
	: mCameraPosition(position), mWorldUp(up), mFov(fov), mAspectRatio(aspectRatio), mNearCLip(nearClip), mFarClip(farClip)
{
	mCameraFront = glm::normalize(position - target);
	mCameraRight = glm::normalize(glm::cross(mWorldUp, mCameraFront));
	mCameraUp = glm::cross(mCameraFront, mCameraRight);
}

glm::mat4 Camera::view()
{
	return glm::lookAt(mCameraPosition, mCameraPosition + facingDirection(), mCameraUp);
}

glm::mat4 Camera::projection()
{
	return glm::perspective(glm::radians(mFov), mAspectRatio, mNearCLip, mFarClip);
}

glm::vec3 Camera::getPosition()
{
	return mCameraPosition;
}

glm::vec3 Camera::getCameraUp()
{
	return mCameraUp;
}

glm::vec3 Camera::getCameraRight()
{
	return mCameraRight;
}

glm::vec3 Camera::getCameraFront()
{
	return mCameraFront;
}

void Camera::setFov(GLfloat fov) {
	mFov = fov;
}

void Camera::setPosition(glm::vec3 newPosition) {
	mCameraPosition = newPosition;
}

void Camera::translateCamera(glm::vec3 translation)
{
	mCameraPosition += translation;
}

void Camera::faceLocation(glm::vec3 location) 
{
	mCameraFront = glm::normalize(mCameraPosition - location);
	mCameraRight = glm::normalize(glm::cross(mWorldUp, mCameraFront));
	mCameraUp = glm::cross(mCameraFront, mCameraRight);
}

void Camera::faceDirection(glm::vec3 direction)
{
	mCameraFront = glm::normalize(-direction);
	mCameraRight = glm::normalize(glm::cross(mWorldUp, mCameraFront));
	mCameraUp = glm::cross(mCameraFront, mCameraRight);
}

glm::vec3 Camera::facingDirection() {
	return -mCameraFront;
}