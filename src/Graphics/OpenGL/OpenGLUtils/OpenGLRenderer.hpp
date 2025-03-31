// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLRenderer
*/

#ifndef SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLRENDERER_HPP_
    #define SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLRENDERER_HPP_
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <memory>

namespace GL {

class OpenGLRenderer {
 public:
    OpenGLRenderer() = default;
    ~OpenGLRenderer() = default;

    void init(GLFWwindow* window);
    void clearScreen();
    void refreshScreen(GLFWwindow* window);
    unsigned int compileShader(const char* vertexShaderSource,
        const char* fragmentShaderSource);

 private:
    unsigned int _shaderProgram = 0;
};

}  // namespace GL

#endif  // SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLRENDERER_HPP_
