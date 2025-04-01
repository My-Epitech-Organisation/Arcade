// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLEvent
*/

#ifndef SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLEVENTS_HPP_
    #define SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLEVENTS_HPP_

    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <utility>
    #include <unordered_map>
    #include "Shared/Models/KeysType.hpp"
    #include "Shared/Models/MouseButtonType.hpp"
    #include "OpenGL/OpenGLKeys.hpp"

namespace GL {
class OpenGLEvent {
 private:
    std::unordered_map<int, bool> _keyStates;
    std::unordered_map<int, bool> _mouseButtonStates;
    double _mouseX;
    double _mouseY;

 public:
    OpenGLEvent();
    ~OpenGLEvent() = default;

    void pollEvents(GLFWwindow* window);
    bool isKeyPressed(int keyCode) const;
    bool isMouseButtonPressed(int button) const;
    std::pair<size_t, size_t> getMousePosition() const;
};
}  // namespace GL

#endif  // SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLEVENTS_HPP_