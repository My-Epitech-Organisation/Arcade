// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLEvent implementation
*/

#include <GL/glew.h>
#include <iostream>
#include <map>
#include <utility>
#include "OpenGLUtils/OpenGLEvent.hpp"
#include "OpenGL/OpenGLKeys.hpp"
#include "Models/KeysType.hpp"

namespace GL {

static std::map<GLFWwindow*, OpenGLEvent*> windowEventMap;

OpenGLEvent::OpenGLEvent()
: _mouseX(0), _mouseY(0) {
    _keyStates.clear();
    _mouseButtonStates.clear();
}

void OpenGLEvent::setupCallbacks(GLFWwindow* window) {
    windowEventMap[window] = this;

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
}

void OpenGLEvent::keyCallback(GLFWwindow* window, int key, int scancode,
int action, int mods) {
    auto eventObj = windowEventMap.find(window);
    if (eventObj != windowEventMap.end()) {
        if (action == GLFW_PRESS)
            eventObj->second->_keyStates[key] = true;
        else if (action == GLFW_RELEASE)
            eventObj->second->_keyStates[key] = false;
    }
}

void OpenGLEvent::mouseButtonCallback(GLFWwindow* window, int button,
int action, int mods) {
    auto eventObj = windowEventMap.find(window);
    if (eventObj != windowEventMap.end()) {
        if (action == GLFW_PRESS)
            eventObj->second->_mouseButtonStates[button] = true;
        else if (action == GLFW_RELEASE)
            eventObj->second->_mouseButtonStates[button] = false;
    }
}

void OpenGLEvent::cursorPosCallback(GLFWwindow* window, double xpos,
double ypos) {
    auto eventObj = windowEventMap.find(window);
    if (eventObj != windowEventMap.end()) {
        eventObj->second->_mouseX = static_cast<size_t>(xpos);
        eventObj->second->_mouseY = static_cast<size_t>(ypos);
    }
}

void OpenGLEvent::pollEvents(GLFWwindow* window) {
    glfwPollEvents();
}

bool OpenGLEvent::isKeyPressed(int keyCode) const {
    auto arcadeKey = static_cast<Arcade::Keys>(keyCode);
    int glfwKey = Arcade::OpenGLKeyMap::getGLFWKey(arcadeKey);

    auto it = _keyStates.find(glfwKey);
    if (it != _keyStates.end())
        return it->second;
    return false;
}

bool OpenGLEvent::isMouseButtonPressed(int button) const {
    auto it = _mouseButtonStates.find(button);
    if (it != _mouseButtonStates.end())
        return it->second;
    return false;
}

std::pair<size_t, size_t> OpenGLEvent::getMousePosition() const {
    return {_mouseX, _mouseY};
}

void OpenGLEvent::resetEvents() {
    _keyStates.clear();
    _mouseButtonStates.clear();
}

}  // namespace GL
