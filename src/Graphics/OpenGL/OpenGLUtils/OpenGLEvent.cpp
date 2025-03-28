// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLEvent
*/

#include "OpenGLUtils/OpenGLEvent.hpp"
#include <iostream>

namespace GL {

OpenGLEvent::OpenGLEvent() : _mouseX(0), _mouseY(0)
{
    // Initialize key states
    for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; i++) {
        _keyStates[i] = false;
    }
    
    // Initialize mouse button states
    for (int i = GLFW_MOUSE_BUTTON_1; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
        _mouseButtonStates[i] = false;
    }
}

void OpenGLEvent::pollEvents(GLFWwindow* window)
{
    if (!window) {
        return;
    }

    // Process GLFW events
    glfwPollEvents();

    // Update keyboard state
    for (auto& pair : _keyStates) {
        pair.second = glfwGetKey(window, pair.first) == GLFW_PRESS;
    }
    
    // Update mouse button state
    for (auto& pair : _mouseButtonStates) {
        pair.second = glfwGetMouseButton(window, pair.first) == GLFW_PRESS;
    }
    
    // Update mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    _mouseX = xpos;
    _mouseY = ypos;
}

bool OpenGLEvent::isKeyPressed(int keyCode) const
{
    // Convert arcade key code to GLFW key code
    int glfwKey = Arcade::OpenGLKeyMap::getGLFWKey(static_cast<Arcade::Keys>(keyCode));
    
    auto it = _keyStates.find(glfwKey);
    if (it != _keyStates.end()) {
        return it->second;
    }
    return false;
}

bool OpenGLEvent::isMouseButtonPressed(int button) const
{
    // Convert arcade mouse button to GLFW mouse button
    int glfwButton = Arcade::OpenGLKeyMap::getGLFWButton(static_cast<Arcade::MouseButton>(button));
    
    auto it = _mouseButtonStates.find(glfwButton);
    if (it != _mouseButtonStates.end()) {
        return it->second;
    }
    return false;
}

std::pair<size_t, size_t> OpenGLEvent::getMousePosition() const
{
    return {static_cast<size_t>(_mouseX), static_cast<size_t>(_mouseY)};
}

}  // namespace GL