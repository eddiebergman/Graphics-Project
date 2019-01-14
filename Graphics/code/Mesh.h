#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <assimp/scene.h>

/*
	All the information needed for interfacing with a Mesh.
	Also handles setting up its own VAO for easier use
*/
namespace Mesh {

	struct Data {
		unsigned vertexCount;
		unsigned facesCount;
		unsigned VAO, VBO, EBO;

		glm::vec3* vertices;
		glm::vec3* normals;
		glm::vec2* textureCoords;
		unsigned* indices;

		Data(aiMesh* aMesh);
		~Data();

		unsigned points();
	};
}