#include "Keyboard.h"

#include <glm/gtc/matrix_transform.hpp>

//Helper function for constructor
bool isBlackKey(int index);

Keyboard::Keyboard(Model::Object& model)
	: rModel(model)
	, rModelNode(*model.getNamedNode("Board"))	
	, rMesh(*model.meshes[rModelNode.meshIndices[0]])
{
	mKeys.reserve(KEY_AMOUNT);
	for (int i = 1; i <= KEY_AMOUNT; i++) {
		Key::Type type = isBlackKey(i) ? Key::Type::BLACK : Key::Type::WHITE;
		Material::Data& material = (type == Key::Type::BLACK) 
			? Material::get("blackKey")
			: Material::get("whiteKey");
		mKeys.emplace_back(getKeyNode(i), getKeyMesh(i), type, material);
	}
}


Keyboard::~Keyboard()
{
}

Model::Node & Keyboard::getKeyNode(int number)
{
	return *rModel.getNamedNode("KEY" + std::to_string(number));
}

Mesh::Data& Keyboard::getKeyMesh(int number) 
{
	return *rModel.meshes[getKeyNode(number).meshIndices[0]];
}

void Keyboard::update() 
{
	for (auto& it : mKeys) {
		it.update();
	}
}

void Keyboard::onKeyPressed(int key, int amount)
{
	mKeys.at(key - 1).onPress(amount);
}

void Keyboard::draw(Shader& shader, glm::mat4 parentMatrix)
{	
	//Board
	shader.setUniformMat4(Shader::UNIFORM_MODEL, parentMatrix);
	shader.setMaterial(Material::get("board"));
	glBindVertexArray(rMesh.VAO);
	glDrawElements(GL_TRIANGLES, rMesh.points(), GL_UNSIGNED_INT, 0);

	//Keys
	for (auto& it : mKeys) {
		it.draw(shader, parentMatrix);
	}
}

bool isBlackKey(int index) {
	switch (index%12) {
		case 2:
		case 4:
		case 7:
		case 9:
		case 11:
			return true;
		default:
			return false;
	}
}