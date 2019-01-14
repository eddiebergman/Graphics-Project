
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "ModelImporter.h"

#include "Model.h"
#include "CameraController.h"
#include "KeyboardController.h"
#include "InputHandler.h"
#include "Keyboard.h"
#include "Timer.h"
#include "Sphere.h"
#include "Light.h"
#include "Scene.h"

#define SCREEN_X 1920
#define SCREEN_Y 1080

//Map of model file names and the names we associate with them
 std::unordered_map<std::string, std::string> modelNames {
	{"keyboard" , "keys.dae" },
	{"sphere" , "sphere.dae"},
	{"floor" , "floor.dae"},
	{"cube", "cube.dae"}
 };

void createWindowContext(int*argc, char** argv);
void display();

float aspectRatio = (float)SCREEN_X / (float)SCREEN_Y;

//Some variables for our Scene
Camera* camera = Scene::Cam::camera(aspectRatio);
CameraController* cameraController; 
glm::vec3 lightpos;

Keyboard* keyboard;
Sphere* sphere;
KeyboardController* keyboardController;


int main(int argc, char** argv) {

	//Init the basics [window, shaders, model importing and setting up lights in the shader]
	createWindowContext(&argc, argv);
	Shader::getInstance().loadShaders("none_textured.vert", "none_textured.frag");
	ModelImporter::loadModels(modelNames);
	Light::init();
	
	//Create a light in the scene and create a Keyboard and Sphere with our imported models
	int id1 = Light::Point::addLight(Scene::SpotLight::Keyboard::light());
	keyboard = new Keyboard(ModelImporter::getModel("keyboard"));
	sphere = new Sphere(ModelImporter::getModel("sphere"), Scene::SpotLight::Sphere::light());

	//Set up input control and link them up to the objects they control
	InputHandler& ih = InputHandler::getInstance();
	cameraController = new CameraController(*camera, ih);
	keyboardController = new KeyboardController(*keyboard, *sphere, ih);
	
	//Start the timer (used for frame counting) and main loop
	Timer::getInstance().start();
	glutMainLoop();

	return 0;
}

void display() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Set up our view and projection from the camera
	Shader::getInstance().use();
	Shader::getInstance().setUniformMat4(Shader::UNIFORM_PROJ, camera->projection());
	Shader::getInstance().setUniformMat4(Shader::UNIFORM_VIEW, camera->view());
	Shader::getInstance().setUniformVec3("cameraPosition", camera->getPosition());

	//Place the keyboard to where it should be in the scene, update and then draw
	glm::mat4 keyboardTransform = glm::mat4(1.0f);	
	keyboardTransform = glm::translate(keyboardTransform, Scene::Keyboard::position);
	keyboardTransform = glm::scale(keyboardTransform, Scene::Keyboard::scale);
	keyboardTransform = glm::rotate(keyboardTransform, Scene::Keyboard::rotationAngle, Scene::Keyboard::rotationVector);
	
	keyboard->update();
	keyboard->draw(Shader::getInstance(), keyboardTransform);

	//Some rotations to cause the sphere to gradually spin around seemingly randomly
	float scaleFactor = glm::cos(Timer::getInstance().framesRendered / 40.0f) + 2;
	float xRotFactor = glm::sin((Timer::getInstance().framesRendered) / 120.0f) * 360.0f;
	float yRotFactor = glm::cos((Timer::getInstance().framesRendered) / 180.0f) * 360.0f;
	float zRotFactor = glm::sin((Timer::getInstance().framesRendered) / 240.0f) * 360.0f;

	//Place the sphere to where it should be in the scene, update and then draw
	glm::mat4 sphereTransform = glm::mat4(1.0f);
	sphereTransform = glm::translate(sphereTransform, Scene::Sphere::position);

	sphereTransform = glm::scale(sphereTransform, (3.0f+ scaleFactor) * glm::vec3{ 1.0f, 1.0f, 1.0f });
	sphereTransform = glm::rotate(sphereTransform, glm::radians(xRotFactor) ,{ 1.0f, 0.0f, 0.0f });
	sphereTransform = glm::rotate(sphereTransform, glm::radians(yRotFactor), { 0.0f, 1.0f, 0.0f });
	sphereTransform = glm::rotate(sphereTransform, glm::radians(zRotFactor), { 0.0f, 0.0f, 1.0f });

	sphere->update(sphereTransform);
	sphere->draw(Shader::getInstance(), sphereTransform);

	//Draw our floor
	Model::Object& floor = ModelImporter::getModel("floor");
	Mesh::Data& floor_mesh = *floor.meshes[floor.getNamedNode("Plane")->meshIndices[0]];
	glm::mat4 m = glm::mat4(1.0f);
	m = glm::translate(m, Scene::Floor::position);
	m = glm::scale(m, Scene::Floor::scale);
	
	Shader::getInstance().setUniformMat4(Shader::UNIFORM_MODEL, m);
	Shader::getInstance().setMaterial(Material::get("floor"));

	glBindVertexArray(floor_mesh.VAO);
	glDrawElements(GL_TRIANGLES, floor_mesh.points(), GL_UNSIGNED_INT, 0);
	
	//These can be used to draw a cubes where lights are in the scene
	//Light::showLightCube(Scene::SpotLight::Keyboard::light());
	//Light::showLightCube(Scene::SpotLight::Sphere::light());
	
	glutSwapBuffers();
}

void createWindowContext(int*argc, char** argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_X, SCREEN_Y);
	glutCreateWindow("Hello Triangle");
	glutDisplayFunc(display);
	GLenum res = glewInit();	
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		exit(1);
	}
}