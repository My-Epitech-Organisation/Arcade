// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLWindow
*/
#include <stdexcept>
#include <string>
#include "OpenGLUtils/OpenGLWindow.hpp"

namespace GL {

void OpenGLWindow::createWindow(int width, int height) {
    _width = width;
    _height = height;

    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    _window = std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>(
        glfwCreateWindow(width, height, "OpenGL - Arcade", nullptr, nullptr),
        glfwDestroyWindow);

    if (!_window)
        throw std::runtime_error("Failed to create GLFW window");
    
    glfwMakeContextCurrent(_window.get());
    glfwSwapInterval(1);  // Enable vsync
}

GLFWwindow* OpenGLWindow::getWindow() const {
    return _window.get();
}

bool OpenGLWindow::shouldClose() const {
    return glfwWindowShouldClose(_window.get());
}

}  // namespace GL
