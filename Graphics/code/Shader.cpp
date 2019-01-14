#include "Shader.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <glm/gtc/type_ptr.hpp>

const std::string Shader::UNIFORM_MODEL = "model";
const std::string Shader::UNIFORM_VIEW = "view";
const std::string Shader::UNIFORM_PROJ = "proj";

void Shader::loadShaders(const GLchar* vert, const GLchar* frag)
{
	std::string defaultVertCode = readInFile(vert);
	std::string defaultFragCode = readInFile(frag);

	//Explicit extra step seems uncessary but didn't want to cooperate otherwise
	const GLchar* defaultVertCodeCstr = (const GLchar*) defaultVertCode.c_str();
	const GLchar* defaultFragCodeCstr = (const GLchar*) defaultFragCode.c_str();

	this->mID = compileShaderProgram(defaultVertCodeCstr, defaultFragCodeCstr);
	this->use();
}

void Shader::use(){
	glUseProgram(mID);
}

void Shader::setUniformMat4(const std::string & uniformName, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(mID, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniformVec3(const std::string & uniformName, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(mID, uniformName.c_str()), 1, glm::value_ptr(value));
}

void Shader::setUniformFloat(const std::string & uniformName, float value)
{
	glUniform1f(glGetUniformLocation(mID, uniformName.c_str()), value);
}

void Shader::setUniformInt(const std::string & uniformName, int value)
{
	glUniform1i(glGetUniformLocation(mID, uniformName.c_str()), value);
}

void Shader::setMaterial(Material::Data material)
{
	setUniformVec3("material.diffuseReflectance", material.diffuseColour);
	setUniformVec3("material.specularReflectance", material.specularColour);
	setUniformFloat("material.shininess", material.shininess);
}

GLuint Shader::compileShaderComponent(GLenum shaderType, const GLchar** source) {
	GLchar infoLog[512];
	GLint success;

	GLuint shaderID = glCreateShader(shaderType);
	if (shaderID == 0) {
		std::cerr << "Error getting object ID\n";
		exit(0);
	}

	glShaderSource(shaderID, 1, source, NULL);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (success == 0) {
		glGetShaderInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
		exit(1);
	}
	
	return shaderID;
}

GLuint Shader::compileShaderProgram(const GLchar * vertShader, const GLchar * fragShader)
{
	GLchar infoLog[512];
	GLuint programID = glCreateProgram();
	if (programID == 0) {
		std::cerr << "Error getting object ID\n"; //TODO: more descriptive error
		exit(0);
	}

	GLuint vertexShaderID = compileShaderComponent(GL_VERTEX_SHADER, &vertShader);
	GLuint fragmentShaderID = compileShaderComponent(GL_FRAGMENT_SHADER, &fragShader);

	//Attach the vertex shader and the fragment shader to the  overall Shader program and then link the program
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	//Link and check if it linked okay
	GLint successShaderProgramLink;
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &successShaderProgramLink);
	if (!successShaderProgramLink) {
		glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", infoLog);
		exit(1);
	}

	//Finally check its all validated
	GLint successShaderValidation;
	glValidateProgram(programID);
	glGetProgramiv(programID, GL_VALIDATE_STATUS, &successShaderValidation);
	if (!successShaderValidation) {
		glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", infoLog);
		exit(1);
	}
	return programID;
}

std::string Shader::readInFile(const GLchar* filename) {
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		shaderFile.open(filename);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		code = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		exit(0);
	}
	return code;
}
