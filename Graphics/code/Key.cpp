#include "Key.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Timer.h"

const float Key::DOWN_MAX_DEGREES = -4.0f;
const float Key::UP_MAX_DEGREES = 0.0f;
const float Key::INTERPOLATION_POINTS = 128.0f;
const float Key::ANGLE_PER_KEYFRAME = (UP_MAX_DEGREES + DOWN_MAX_DEGREES) / INTERPOLATION_POINTS;
const int Key::RAISE_SPEED = 10;

Key::Key(Model::Node& modelNode, Mesh::Data& mesh, Type keyType, Material::Data& material)
	: rModelNode(modelNode), rMesh(mesh), mKeyFrame(0), eType(keyType), rMaterial(material)
{
}


Key::~Key()
{
}

void Key::update() 
{
	mKeyFrame = (mKeyFrame - RAISE_SPEED < 0) ? 0 : mKeyFrame - RAISE_SPEED;
	mAngle = getAngleOfKey();
}

void Key::onPress(int amount) {
	mKeyFrame = amount;
}

void Key::draw(Shader& shader, glm::mat4 parentMatrix) 
{
	//float scale = Timer::getInstance().framesRendered / 5.0f;
	glm::mat4 m = glm::rotate(parentMatrix, glm::radians(mAngle), {1.0,0,0});
	shader.setUniformMat4(Shader::UNIFORM_MODEL, m);
	shader.setMaterial(rMaterial);
	glBindVertexArray(rMesh.VAO);
	glDrawElements(GL_TRIANGLES, rMesh.points(), GL_UNSIGNED_INT, 0);
}

float Key::getAngleOfKey()
{
	return UP_MAX_DEGREES + (mKeyFrame * ANGLE_PER_KEYFRAME);
}

bool Key::isWhite() {
	return eType == Type::WHITE;
}

bool Key::isBlack() {
	return eType == Type::BLACK;
}

