// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLWindow
*/

#include "OpenGLUtils/OpenGLWindow.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

namespace GL {

void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

OpenGLWindow::OpenGLWindow() : _window(nullptr), _width(0),
_height(0), _isOpen(false) {}

OpenGLWindow::~OpenGLWindow() {
    close();
}

void OpenGLWindow::init(float width, float height, const std::string &title) {
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _width = static_cast<int>(width);
    _height = static_cast<int>(height);
    _window = glfwCreateWindow(_width, _height,
        title.c_str(), nullptr, nullptr);

    if (!_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(_window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLEW");
    }

    glViewport(0, 0, _width, _height);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _isOpen = true;
}

void OpenGLWindow::close() {
    if (_window) {
        glfwDestroyWindow(_window);
        _window = nullptr;
        glfwTerminate();
        _isOpen = false;
    }
}

void OpenGLWindow::clearScreen() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::refreshScreen() {
    glfwSwapBuffers(_window);
    glfwPollEvents();

    if (glfwWindowShouldClose(_window))
        _isOpen = false;
}

bool OpenGLWindow::isOpen() const {
    return _isOpen;
}

GLFWwindow* OpenGLWindow::getWindow() const {
    return _window;
}

int OpenGLWindow::getWidth() const {
    return _width;
}

int OpenGLWindow::getHeight() const {
    return _height;
}

}  // namespace GL
