// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** OpenGL key mapping implementation
*/

#include "OpenGLKeys.hpp"

namespace Arcade {

// Define the static mapping 
const std::unordered_map<Keys, int> OpenGLKeyMap::_keyMap = {
    {Keys::UP, GLFW_KEY_UP},
    {Keys::DOWN, GLFW_KEY_DOWN},
    {Keys::LEFT, GLFW_KEY_LEFT},
    {Keys::RIGHT, GLFW_KEY_RIGHT},
    {Keys::ENTER, GLFW_KEY_ENTER},
    {Keys::ESC, GLFW_KEY_ESCAPE},
    {Keys::NUM1, GLFW_KEY_1},
    {Keys::NUM2, GLFW_KEY_2},
    {Keys::NUM3, GLFW_KEY_3},
    {Keys::NUM4, GLFW_KEY_4},
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
    {Keys::Z, GLFW_KEY_Z}
};

const std::unordered_map<MouseButton, int> OpenGLKeyMap::_mouseButtonMap = {
    {MouseButton::LEFT, GLFW_MOUSE_BUTTON_LEFT},
    {MouseButton::MIDDLE, GLFW_MOUSE_BUTTON_MIDDLE},
    {MouseButton::RIGHT, GLFW_MOUSE_BUTTON_RIGHT},
    {MouseButton::X1, GLFW_MOUSE_BUTTON_4},
    {MouseButton::X2, GLFW_MOUSE_BUTTON_5}
};

}  // namespace Arcade