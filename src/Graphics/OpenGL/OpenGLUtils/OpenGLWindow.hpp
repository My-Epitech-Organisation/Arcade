// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLWindow
*/

#ifndef SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLWINDOW_HPP_
    #define SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLWINDOW_HPP_
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <memory>
    #include <string>

namespace GL {

class OpenGLWindow {
 public:
    OpenGLWindow() : _window(nullptr, glfwDestroyWindow), _width(0), _height(0) {}
    ~OpenGLWindow() = default;

    void createWindow(int width, int height);
    GLFWwindow* getWindow() const;
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    bool shouldClose() const;

 private:
    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> _window;
    int _width;
    int _height;
};

}  // namespace GL

#endif  // SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLWINDOW_HPP_
