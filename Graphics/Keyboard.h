#pragma once

#include <glm/mat4x4.hpp>

#include "Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Key.h"

/*
	A class that represents the Keyboard object as a whole.
	Allows us to inform a Key it has been presssed and handles its own drawing
*/
class Keyboard
{
public:

	static const unsigned KEY_AMOUNT = 60;

	Keyboard(Model::Object& model);
	~Keyboard();

	Model::Node& getKeyNode(int number);
	Mesh::Data& Keyboard::getKeyMesh(int number);
	void update();
	void draw(Shader& shader, glm::mat4 parentMatrix);
	void onKeyPressed(int key, int amount);

private:
	std::vector<Key> mKeys;
	Model::Object& rModel;

	Model::Node& rModelNode;
	Mesh::Data& rMesh;

};

