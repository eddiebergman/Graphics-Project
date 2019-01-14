#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <string>

#include "Material.h"
#include "Light.h"

/*
	A class for handling setting up a Shader, some shader specific constants and interacting with our
	shader
*/
class Shader
{
public:
	GLuint mID;
	static Shader& getInstance() {
		static Shader instance;
		return instance;
	}

	static const GLuint ATTRIBUTE_POSITION = 0;
	static const GLuint ATTRIBUTE_TEXTURE = 1;
	static const GLuint ATTRIBUTE_NORMAL = 2;
	static const GLuint ATTRIBUTE_COLOUR = 3;

	static const GLuint DIRECTIONAL_LIGHT_LIMIT = 5;
	static const GLuint SPOT_LIGHT_LIMIT = 5;
	static const GLuint POINT_LIGHT_LIMIT = 22;

	static const std::string UNIFORM_MODEL;
	static const std::string UNIFORM_VIEW;
	static const std::string UNIFORM_PROJ;

	void loadShaders(const GLchar* vertexPath, const GLchar* fragmentPath);

	void setUniformMat4(const std::string& uniformName, glm::mat4 value);
	void setUniformVec3(const std::string& uniformName, glm::vec3 value);
	void setUniformFloat(const std::string& uniformName, float value);
	void setUniformInt(const std::string& uniformName, int value);
	void setMaterial(Material::Data material);

	void use();

private:
	
	Shader() {};
	Shader(Shader const&) = delete;    //So we can't create new instances     
	void operator=(Shader const&) = delete;

	GLuint compileShaderComponent(GLenum shaderType, const GLchar** source);
	GLuint compileShaderProgram(const GLchar* vertShader, const GLchar* fragShader);
	std::string readInFile(const GLchar* filename);
};

#endif
