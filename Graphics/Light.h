#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

/*
	A namespace for handling Lights in our program. We have Directional, Point and Spot
	lights. Also easy functionality for updating state in the Shaders for a light of a certain ID
*/
namespace Light {

	//The colour of our lights
	struct LightValues {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		LightValues(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	};

	struct Directional {
		//Used to keep track of what ID's to return back, 0 -> DIRECTIONAL_LIGHT_LIMIT
		static unsigned index;

		LightValues light;
		glm::vec3 direction;
		
		Directional(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
		
		static int addLight(Light::Directional& light);
		static void updateLightValues(int id, Light::Directional& light);
		static void updateDirection(int id, Light::Directional& light);
		static void updateState(int id, bool on);
	};

	struct Point {
	
		//Same as Directional
		static unsigned index;
		
		LightValues light;
		glm::vec3 position;
		
		float constant;
		float linear;
		float quadratic;

		Point(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular
			, glm::vec3 attenuationValues);

		static int addLight(Light::Point& light);
		static void updateLightValues(int id, Light::Point& light);
		static void updateStrengths(int id, Light::Point& light);
		static void updatePosition(int id, Light::Point& light);
		static void updateState(int id, bool on);
	};

	struct Spot {

		//Same as Point
		static unsigned index;

		LightValues light;
		glm::vec3 position;
		glm::vec3 direction;

		float cutOff;
		float outerCutOff;
		float constant;
		float linear;
		float quadratic;

		Spot(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular
			,glm::vec2 cutOffs, glm::vec3 attenuationValues);

		static int addLight(Light::Spot& light);
		static void updateLightValues(int id, Light::Spot& light);
		static void updateLightCutOffs(int id, Light::Spot& light);
		static void updateStrengths(int id, Light::Spot& light);
		static void updatePosition(int id, Light::Spot& light);
		static void updateDirection(int id, Light::Spot& light);
		static void updateState(int id, bool on);
		
	};
	
	//Turns all lights off initially
	void init();

	//Debug functionality
	void showLightCube(Light::Point light);
	void showLightCube(Light::Spot light);
	void showLightCube(glm::vec3 position, Light::LightValues vals);
};

