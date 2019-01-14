#include "Mesh.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shader.h"

namespace Mesh {


	Data::Data(aiMesh* aMesh)
		: vertexCount(aMesh->mNumVertices), facesCount(aMesh->mNumFaces)
	{
		//Allocate space for everything 
		vertices = (glm::vec3*) malloc(vertexCount * sizeof(glm::vec3));
		normals = (glm::vec3*) malloc(vertexCount * sizeof(glm::vec3));
		textureCoords = (glm::vec2*) malloc(vertexCount * sizeof(glm::vec2));
		indices = (unsigned*)malloc(facesCount * 3 * sizeof(unsigned));
		
		//Assuming they all have positions, normals and texcoords
		if (aMesh->HasPositions()) {
			for (unsigned i = 0; i < vertexCount; i++) {
				const aiVector3D* vertex = &(aMesh->mVertices[i]);
				vertices[i] = { vertex->x, vertex->y, vertex->z };
			}
		}
		if (aMesh->HasNormals()) {
			for (unsigned i = 0; i < vertexCount; i++) {
				const aiVector3D* normal = &(aMesh->mNormals[i]);
				normals[i] = { normal->x, normal->y, normal->z };
			}
		}
		if (aMesh->HasTextureCoords(0)) {
			for (unsigned i = 0; i < vertexCount; i++) {
				const aiVector3D* texCoords = &(aMesh->mTextureCoords[0][i]);
				textureCoords[i] = { texCoords->x, texCoords->y };
			}
		}

		//Assuming they have faces
		for (unsigned i = 0; i < facesCount; i++) {
			const aiFace* face = &(aMesh->mFaces[i]);
			for (unsigned j = 0; j < face->mNumIndices; j++) {
				indices[i * 3 + j] = face->mIndices[j];
			}
		}

		//Generate VAO, VBO and EBO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		//Total buffer size is all the vertices + normals + texturecoords
		unsigned totalBufferSize = vertexCount * (sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2));
		glBufferData(GL_ARRAY_BUFFER, totalBufferSize, NULL, GL_STATIC_DRAW);

		//Insert Vertices, Normals and TexCoords into buffer
		unsigned verticesSize = vertexCount * sizeof(glm::vec3);
		unsigned normalsSize = vertexCount * sizeof(glm::vec3);
		unsigned texCoordsSize = vertexCount * sizeof(glm::vec2);
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, (void*)vertices);
		glBufferSubData(GL_ARRAY_BUFFER, verticesSize, normalsSize, (void*)normals);
		glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize, texCoordsSize, (void*)textureCoords);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, facesCount * 3 * sizeof(unsigned int),
			&indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(Shader::ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0
			, (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(Shader::ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0
			, (void*)verticesSize);
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(Shader::ATTRIBUTE_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0
			, (void*)(verticesSize + normalsSize));

		glBindVertexArray(0);
	}

	Data::~Data() 
	{
		delete[] vertices;
		delete[] normals;
		delete[] textureCoords;
		delete[] indices;
	}

	unsigned Data::points()
	{
		return facesCount * 3;
	}

}

