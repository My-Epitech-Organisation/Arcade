// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLEvent
*/

#ifndef SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLEVENT_HPP_
#define SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLEVENT_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <utility>
#include <map>
#include "OpenGL/OpenGLKeys.hpp"

namespace GL {

class OpenGLEvent {
 private:
      std::map<int, bool> _keyStates;
      std::map<int, bool> _mouseButtonStates;
      size_t _mouseX;
      size_t _mouseY;

      static void keyCallback(GLFWwindow* window, int key, int scancode,
         int action, int mods);
      static void mouseButtonCallback(GLFWwindow* window, int button,
         int action, int mods);
      static void cursorPosCallback(GLFWwindow* window, double xpos,
         double ypos);

 public:
      OpenGLEvent();
      ~OpenGLEvent() = default;
      void setupCallbacks(GLFWwindow* window);
      void pollEvents(GLFWwindow* window);
      bool isKeyPressed(int keyCode) const;
      bool isMouseButtonPressed(int button) const;
      std::pair<size_t, size_t> getMousePosition() const;
      void resetEvents();
};

}  // namespace GL

#endif  // SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLEVENT_HPP_
