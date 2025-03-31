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
 private:
    GLFWwindow *_window;
    int _width;
    int _height;
    bool _isOpen;

 public:
    OpenGLWindow();
    ~OpenGLWindow();

    void init(float width, float height, const std::string &title);
    void close();
    void clearScreen();
    void refreshScreen();
    bool isOpen() const;
    GLFWwindow *getWindow() const;
    int getWidth() const;
    int getHeight() const;
};

}  // namespace GL

#endif  // SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLWINDOW_HPP_
