// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** OpenGL Key Mapping
*/

#ifndef SRC_GRAPHICS_OPENGL_OPENGLKEYS_HPP_
    #define SRC_GRAPHICS_OPENGL_OPENGLKEYS_HPP_
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <unordered_map>
    #include "Models/KeysType.hpp"
    #include "Models/MouseButtonType.hpp"

namespace Arcade {

class OpenGLKeyMap {
 private:
    static const std::unordered_map<Keys, int> _keyMap;
    static const std::unordered_map<MouseButton, int> _mouseButtonMap;

 public:
    static int getGLFWKey(Keys key) {
        auto it = _keyMap.find(key);
        return (it != _keyMap.end()) ? it->second : 0;
    }

    static Keys getArcadeKey(int glfwKey) {
        for (const auto& pair : _keyMap) {
            if (pair.second == glfwKey) {
                return pair.first;
            }
        }
        return Keys::NONE;
    }

    static int getGLFWButton(MouseButton button) {
        auto it = _mouseButtonMap.find(button);
        return (it != _mouseButtonMap.end()) ? it->second : 0;
    }
};

const std::unordered_map<Keys, int> OpenGLKeyMap::_keyMap = {
    {Keys::UP, GLFW_KEY_UP},
    {Keys::DOWN, GLFW_KEY_DOWN},
    {Keys::LEFT, GLFW_KEY_LEFT},
    {Keys::RIGHT, GLFW_KEY_RIGHT},
    {Keys::ENTER, GLFW_KEY_ENTER},
    {Keys::SPACE, GLFW_KEY_SPACE},
    {Keys::ESC, GLFW_KEY_ESCAPE},
    {Keys::A, GLFW_KEY_A},
    {Keys::B, GLFW_KEY_B},
    {Keys::C, GLFW_KEY_C},
    {Keys::D, GLFW_KEY_D},
    {Keys::E, GLFW_KEY_E},
    {Keys::F, GLFW_KEY_F},
    {Keys::G, GLFW_KEY_G},
    {Keys::H, GLFW_KEY_H},
    {Keys::I, GLFW_KEY_I},
    {Keys::J, GLFW_KEY_J},
    {Keys::K, GLFW_KEY_K},
    {Keys::L, GLFW_KEY_L},
    {Keys::M, GLFW_KEY_M},
    {Keys::N, GLFW_KEY_N},
    {Keys::O, GLFW_KEY_O},
    {Keys::P, GLFW_KEY_P},
    {Keys::Q, GLFW_KEY_Q},
    {Keys::R, GLFW_KEY_R},
    {Keys::S, GLFW_KEY_S},
    {Keys::T, GLFW_KEY_T},
    {Keys::U, GLFW_KEY_U},
    {Keys::V, GLFW_KEY_V},
    {Keys::W, GLFW_KEY_W},
    {Keys::X, GLFW_KEY_X},
    {Keys::Y, GLFW_KEY_Y},
    {Keys::Z, GLFW_KEY_Z},
    {Keys::NUM0, GLFW_KEY_0},
    {Keys::NUM1, GLFW_KEY_1},
    {Keys::NUM2, GLFW_KEY_2},
    {Keys::NUM3, GLFW_KEY_3},
    {Keys::NUM4, GLFW_KEY_4},
    {Keys::NUM5, GLFW_KEY_5},
    {Keys::NUM6, GLFW_KEY_6},
    {Keys::NUM7, GLFW_KEY_7},
    {Keys::NUM8, GLFW_KEY_8},
    {Keys::NUM9, GLFW_KEY_9}
};

const std::unordered_map<MouseButton, int> OpenGLKeyMap::_mouseButtonMap = {
    {MouseButton::LEFT, GLFW_MOUSE_BUTTON_LEFT},
    {MouseButton::MIDDLE, GLFW_MOUSE_BUTTON_MIDDLE},
    {MouseButton::RIGHT, GLFW_MOUSE_BUTTON_RIGHT},
    {MouseButton::X1, GLFW_MOUSE_BUTTON_4},
    {MouseButton::X2, GLFW_MOUSE_BUTTON_5}
};

}  // namespace Arcade

#endif  // SRC_GRAPHICS_OPENGL_OPENGLKEYS_HPP_
