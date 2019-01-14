#include <GL/freeglut.h>
#include <iostream>
#include <algorithm>

#include "InputHandler.h"

InputHandler::InputHandler()
{
		//Registers callbacks for the various events Glut emits
		glutMouseFunc([](int button, int state, int x, int y) -> void {
			InputHandler::getInstance().handleMousePressed(button, state, x, y);
		});
		glutKeyboardFunc([](unsigned char key, int x, int y) -> void {
			InputHandler::getInstance().handleKeyPressed(key, x, y);
		});
		glutMotionFunc([](int x, int y) -> void {
			InputHandler::getInstance().handleMouseMoveActive(x, y);
		});
		glutPassiveMotionFunc([](int x, int y) -> void {
			InputHandler::getInstance().handleMouseMovePassive(x, y);
		});
}

void InputHandler::onMousePress(std::function<void(int, int, int, int)> callback)
{
	mMousePressObservers.push_back(callback);
}

void InputHandler::onKeyPress(std::function<void(unsigned char, int, int)> callback)
{
	mKeyPressObservers.push_back(callback);
}

void InputHandler::onMouseMoveActive(std::function<void(int, int)> callback)
{
	mMouseMoveActiveObservers.push_back(callback);
}

void InputHandler::onMouseMovePassive(std::function<void(int, int)> callback)
{
	mMouseMovePassiveObservers.push_back(callback);
}

void InputHandler::handleMousePressed(int button, int state, int x, int y)
{
	auto& vec = this->mMousePressObservers;
	std::for_each(vec.begin(), vec.end(), [=](auto& callback) { callback(button, state, x, y); });
}

void InputHandler::handleKeyPressed(unsigned char key, int x, int y) {
	auto& vec = this->mKeyPressObservers;
	std::for_each(vec.begin(), vec.end(), [=](auto& callback) { callback(key, x, y); });
}

void InputHandler::handleMouseMoveActive(int x, int y)
{
	auto& vec = this->mMouseMoveActiveObservers;
	std::for_each(vec.begin(), vec.end(), [=](auto& callback) { callback(x, y); });
}

void InputHandler::handleMouseMovePassive(int x, int y)
{
	auto& vec = this->mMouseMovePassiveObservers;
	std::for_each(vec.begin(), vec.end(), [=](auto& callback) { callback(x, y); });
}
