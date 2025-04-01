// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** OpenGL key mapping
*/

#ifndef SRC_GRAPHICS_OPENGL_OPENGLKEYS_HPP_
    #define SRC_GRAPHICS_OPENGL_OPENGLKEYS_HPP_
    #include <GLFW/glfw3.h>
    #include <unordered_map>
    #include "Shared/Models/KeysType.hpp"
    #include "Shared/Models/MouseButtonType.hpp"

namespace Arcade {

class OpenGLKeyMap {
 private:
    static const std::unordered_map<Keys, int> _keyMap;
    static const std::unordered_map<MouseButton, int> _mouseButtonMap;

 public:
    static int getGLFWKey(Keys key) {
        auto it = _keyMap.find(key);
        return (it != _keyMap.end()) ? it->second : GLFW_KEY_UNKNOWN;
    }

    static int getGLFWButton(MouseButton button) {
        auto it = _mouseButtonMap.find(button);
        return (it != _mouseButtonMap.end()) ? it->second : -1;
    }
};
}  // namespace Arcade

#endif  // SRC_GRAPHICS_OPENGL_OPENGLKEYS_HPP_