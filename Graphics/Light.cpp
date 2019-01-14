#include "Light.h"
#include "Shader.h"
#include "Model.h"
#include "ModelImporter.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace Light {

	unsigned Spot::index = 0;
	unsigned Directional::index = 0;
	unsigned Point::index = 0;

	LightValues::LightValues(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		: ambient(ambient), diffuse(diffuse), specular(specular) {}

	Directional::Directional(glm::vec3 direction, glm::vec3 ambient
		, glm::vec3 diffuse, glm::vec3 specular)
		: direction(direction),
		light({ ambient,diffuse,specular }) {}

	int Directional::addLight(Light::Directional & light)
	{
		if (Directional::index == Shader::DIRECTIONAL_LIGHT_LIMIT)
			return -1;

		unsigned i = Directional::index;
		std::string lightRef = "directionalLights[" + std::to_string(i) + "].";
		Shader::getInstance().setUniformInt(lightRef + "on", 0);
		Shader::getInstance().setUniformVec3(lightRef + "direction", light.direction);
		Shader::getInstance().setUniformVec3(lightRef + "ambient", light.light.ambient);
		Shader::getInstance().setUniformVec3(lightRef + "diffuse", light.light.diffuse);
		Shader::getInstance().setUniformVec3(lightRef + "specular", light.light.specular);
		Directional::index++;
		return i;
	}

	void Directional::updateLightValues(int id, Light::Directional & light)
	{
		std::string ref = "directionalLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformVec3(ref + "ambient", light.light.ambient);
		Shader::getInstance().setUniformVec3(ref + "diffuse", light.light.diffuse);
		Shader::getInstance().setUniformVec3(ref + "specular", light.light.specular);
	}

	void Directional::updateDirection(int id, Light::Directional & light)
	{
		std::string ref = "directionalLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformVec3(ref + "direction", light.direction);
	}

	void Directional::updateState(int id, bool on)
	{
		std::string ref = "directionalLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformInt(ref + "on", (on ? 1 : 0));
	}

	Point::Point(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuationValues)
		: position(position), light({ambient, diffuse, specular})
		, constant(attenuationValues.x), linear(attenuationValues.y), quadratic(attenuationValues.z) {}

	int Point::addLight(Light::Point & light)
	{
		if (Point::index == Shader::POINT_LIGHT_LIMIT)
			return -1;

		int i = Point::index;
		std::string lightRef = "pointLights[" + std::to_string(i) + "].";
		Shader::getInstance().setUniformInt(lightRef + "on", 1);
		Shader::getInstance().setUniformVec3(lightRef + "position", light.position);
		Shader::getInstance().setUniformFloat(lightRef + "constant", light.constant);
		Shader::getInstance().setUniformFloat(lightRef + "linear", light.linear);
		Shader::getInstance().setUniformFloat(lightRef + "quadratic", light.quadratic);
		Shader::getInstance().setUniformVec3(lightRef + "ambient", light.light.ambient);
		Shader::getInstance().setUniformVec3(lightRef + "diffuse", light.light.diffuse);
		Shader::getInstance().setUniformVec3(lightRef + "specular", light.light.specular);
		Point::index++;
		return i;
	}

	void Point::updateLightValues(int id, Light::Point & light)
	{
		std::string ref = "pointLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformVec3(ref + "ambient", light.light.ambient);
		Shader::getInstance().setUniformVec3(ref + "diffuse", light.light.diffuse);
		Shader::getInstance().setUniformVec3(ref + "specular", light.light.specular);
	}

	void Point::updateStrengths(int id, Light::Point & light)
	{
		std::string ref = "pointLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformFloat(ref + "constant", light.constant);
		Shader::getInstance().setUniformFloat(ref + "linear", light.linear);
		Shader::getInstance().setUniformFloat(ref + "quadratic", light.quadratic);
	}

	void Point::updatePosition(int id, Light::Point & light)
	{
		std::string ref = "pointLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformVec3(ref + "position", light.position);
	}

	void Point::updateState(int id, bool on)
	{
		std::string ref = "pointLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformInt(ref + "on", (on ? 1 : 0));
	}

	Spot::Spot(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec2 cutOffs, glm::vec3 attenuationValues)
		: position(position), direction(direction), light({ ambient, diffuse, specular })
		, constant(attenuationValues.x), linear(attenuationValues.y), quadratic(attenuationValues.z)
		, cutOff(cutOffs.x), outerCutOff(cutOffs.y) {}

	int Spot::addLight(Light::Spot & light)
	{
		if (Spot::index == Shader::SPOT_LIGHT_LIMIT)
			return -1;

		unsigned i = Spot::index;
		std::string lightRef = "spotLights[" + std::to_string(i) + "].";
		Shader::getInstance().setUniformInt(lightRef + "on", 1);
		Shader::getInstance().setUniformVec3(lightRef + "position", light.position);
		Shader::getInstance().setUniformVec3(lightRef + "direction", light.direction);
		Shader::getInstance().setUniformFloat(lightRef + "cutOff", light.cutOff);
		Shader::getInstance().setUniformFloat(lightRef + "outerCutOff", light.outerCutOff);
		Shader::getInstance().setUniformFloat(lightRef + "constant", light.constant);
		Shader::getInstance().setUniformFloat(lightRef + "linear", light.linear);
		Shader::getInstance().setUniformFloat(lightRef + "quadratic", light.quadratic);
		Shader::getInstance().setUniformVec3(lightRef + "ambient", light.light.ambient);
		Shader::getInstance().setUniformVec3(lightRef + "diffuse", light.light.diffuse);
		Shader::getInstance().setUniformVec3(lightRef + "specular", light.light.specular);
		Spot::index++;
		return i;
	}

	void Spot::updateLightValues(int id, Light::Spot & light)
	{
		std::string ref = "spotLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformVec3(ref + "ambient", light.light.ambient);
		Shader::getInstance().setUniformVec3(ref + "diffuse", light.light.diffuse);
		Shader::getInstance().setUniformVec3(ref + "specular", light.light.specular);
	}

	void Spot::updateLightCutOffs(int id, Light::Spot & light)
	{
		std::string ref = "spotLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformFloat(ref + "cutOff", light.cutOff);
		Shader::getInstance().setUniformFloat(ref + "outerCutOff", light.outerCutOff);
	}

	void Spot::updateStrengths(int id, Light::Spot & light)
	{
		std::string ref = "spotLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformFloat(ref + "constant", light.constant);
		Shader::getInstance().setUniformFloat(ref + "linear", light.linear);
		Shader::getInstance().setUniformFloat(ref + "quadratic", light.quadratic);
	}

	void Spot::updatePosition(int id, Light::Spot & light)
	{
		std::string ref = "spotLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformVec3(ref + "position", light.position);
	}

	void Spot::updateDirection(int id, Light::Spot & light)
	{
		std::string ref = "spotLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformVec3(ref + "direction", light.direction);
	}

	void Spot::updateState(int id, bool on)
	{
		std::string ref = "spotLights[" + std::to_string(id) + "].";
		Shader::getInstance().setUniformInt(ref + "on", (on ? 1 : 0));
	}

}

void Light::init()
{
	for (int i = 0; i < Shader::DIRECTIONAL_LIGHT_LIMIT; i++) {
		Light::Directional::updateState(i, false);
	}
	for (int i = 0; i < Shader::SPOT_LIGHT_LIMIT; i++) {
		Light::Spot::updateState(i, false);
	}
	for (int i = 0; i < Shader::POINT_LIGHT_LIMIT; i++) {
		Light::Point::updateState(i, false);
	}
}

void Light::showLightCube(Light::Point light) {
	showLightCube(light.position, light.light);
}

void Light::showLightCube(Light::Spot light) {
	showLightCube(light.position, light.light);
}

void Light::showLightCube(glm::vec3 position, Light::LightValues vals) {
	Model::Object& floor = ModelImporter::getModel("cube");
	Mesh::Data& floor_mesh = *floor.meshes[floor.getNamedNode("Cube")->meshIndices[0]];
	glm::mat4 m = glm::mat4(1.0f);
	m = glm::translate(m, position);
	Shader::getInstance().setUniformMat4(Shader::UNIFORM_MODEL, m);

	Material::Data lightMat = { vals.diffuse, vals.specular, 1 };
	Shader::getInstance().setMaterial(lightMat);
	glBindVertexArray(floor_mesh.VAO);
	glDrawElements(GL_TRIANGLES, floor_mesh.points(), GL_UNSIGNED_INT, 0);
}

