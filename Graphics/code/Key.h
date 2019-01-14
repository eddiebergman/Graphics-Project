#pragma once

#include <glm/mat4x4.hpp>

#include "Model.h"
#include "Shader.h"
#include "Material.h"

/*
	A class that represents an individual key on the keyboard
	Handles its own animation as well, abstracting it out of the keyboard class
*/
class Key
{
public:

	static const float DOWN_MAX_DEGREES;
	static const float UP_MAX_DEGREES;
	static const float INTERPOLATION_POINTS;
	static const float ANGLE_PER_KEYFRAME;
	static const int RAISE_SPEED;
	const enum Type {
		WHITE, BLACK
	};

	Key(Model::Node& modelNode, Mesh::Data& mesh, Type keyType, Material::Data& material);
	~Key();

	void update();
	void draw(Shader& shader, glm::mat4 parentMatrix);
	void onPress(int amount);
	bool isWhite();
	bool isBlack();

private:
	int mKeyFrame;
	float mAngle;
	Type eType;
	Model::Node& rModelNode;
	Mesh::Data& rMesh;
	Material::Data& rMaterial;

	float getAngleOfKey();

};

