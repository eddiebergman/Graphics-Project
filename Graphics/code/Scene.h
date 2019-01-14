#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Light.h"
#include "Camera.h"

/*
	These values are just for initial model space and some handy setup
	Yeah a JSON object would be nice or a nice kind of map structure but ehh
*/
namespace Scene {

	//Issue with namespace and Type Camera overlapping: changed to Cam to fix
	namespace Cam {
		//const glm::vec3 position = { 0,-80,30 };
		const glm::vec3 position = { 0,-50,15 };
		const glm::vec3 target = { 0,0,0 };
		Camera* camera(float aspectRatio) {
			return new Camera(position, target, aspectRatio);
		}
	}

	namespace Keyboard {
		const float scaleFactor = 0.3f;
		const glm::vec3 scale = scaleFactor * glm::vec3{ 1.0f, 1.0f,1.0f };
		const glm::vec3 position = glm::vec3{ 0, -41, 9.2 };
		const float rotationAngle = glm::radians(180.0f);
		const glm::vec3 rotationVector = { 0.0f,1.0f,1.0f};
	}

	namespace Sphere {
		const glm::vec3 position = { 0.0f,0.0f,5.0f };
		const float rotationAngle = glm::radians(0.0f);
		const glm::vec3 rotationVector = { 0.0f,0.0f,0.0f };
		const glm::vec3 scale = 0.3f * glm::vec3{ 1.0f, 1.0f,1.0f };
	}

	namespace Floor {
		const glm::vec3 position = { 0.0f,0.0f,-5.0f };
		const float rotationAngle = glm::radians(0.0f);
		const glm::vec3 rotationVector = { 0.0f,0.0f,0.0f };
		const glm::vec3 scale = 100.0f * glm::vec3{ 1.0f, 1.0f,1.0f };
	}

	namespace SpotLight {

		namespace Keyboard {
			const glm::vec3 position = Scene::Keyboard::position + glm::vec3{0.0f,0.0f,20.0f};
			const glm::vec3 direction = position - Scene::Keyboard::position;

			const glm::vec3 ambient = { 1.0f, 1.0f, 1.0f };
			const glm::vec3 diffuse = { 1.0f, 1.0f, 1.0f };
			const glm::vec3 specular = { 1.0f, 1.0f, 1.0f };

			const glm::vec3 attenuation = { 1.0f, 0.09f, 0.062f };
			Light::Point light() {
				return Light::Point{
					SpotLight::Keyboard::position,
					SpotLight::Keyboard::ambient,
					SpotLight::Keyboard::diffuse,
					SpotLight::Keyboard::specular,
					SpotLight::Keyboard::attenuation
				};
			}
		}

		namespace Sphere {
			const glm::vec3 position = Sphere::position + glm::vec3{ 0.0f,0.0f,20.0f };
			const glm::vec3 direction = { 0.0f, 0.0f, 1.0f };

			const glm::vec3 ambient = { 0.0f, 0.0f, 0.0f };
			const glm::vec3 diffuse = { 0.0f, 0.0f, 0.0f };
			const glm::vec3 specular = { 0.0f, 0.0f, 0.0f };

			const glm::vec2 cutoffs = { glm::radians(5.0f), glm::radians(10.0f) };
			const glm::vec3 attenuation = { 1.0f, 0.008f, 0.002f };
			Light::Spot light() {
				return Light::Spot{
					SpotLight::Sphere::position,
					SpotLight::Sphere::direction,
					SpotLight::Sphere::ambient,
					SpotLight::Sphere::diffuse,
					SpotLight::Sphere::specular,
					SpotLight::Sphere::cutoffs,
					SpotLight::Sphere::attenuation
				};
			}
		}
	}

}