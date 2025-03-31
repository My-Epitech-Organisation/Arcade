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
#include <map>
#include "Models/KeysType.hpp"

class OpenGLKeys {
 public:
    static int convertKeyToGLFW(Arcade::Keys key) {
        static const std::map<Arcade::Keys, int> keyMap = {
            {Arcade::Keys::UP, GLFW_KEY_UP},
            {Arcade::Keys::DOWN, GLFW_KEY_DOWN},
            {Arcade::Keys::LEFT, GLFW_KEY_LEFT},
            {Arcade::Keys::RIGHT, GLFW_KEY_RIGHT},
            {Arcade::Keys::ENTER, GLFW_KEY_ENTER},
            {Arcade::Keys::SPACE, GLFW_KEY_SPACE},
            {Arcade::Keys::ESC, GLFW_KEY_ESCAPE},
            {Arcade::Keys::A, GLFW_KEY_A},
            {Arcade::Keys::B, GLFW_KEY_B},
            {Arcade::Keys::C, GLFW_KEY_C},
            {Arcade::Keys::D, GLFW_KEY_D},
            {Arcade::Keys::E, GLFW_KEY_E},
            {Arcade::Keys::F, GLFW_KEY_F},
            {Arcade::Keys::G, GLFW_KEY_G},
            {Arcade::Keys::H, GLFW_KEY_H},
            {Arcade::Keys::I, GLFW_KEY_I},
            {Arcade::Keys::J, GLFW_KEY_J},
            {Arcade::Keys::K, GLFW_KEY_K},
            {Arcade::Keys::L, GLFW_KEY_L},
            {Arcade::Keys::M, GLFW_KEY_M},
            {Arcade::Keys::N, GLFW_KEY_N},
            {Arcade::Keys::O, GLFW_KEY_O},
            {Arcade::Keys::P, GLFW_KEY_P},
            {Arcade::Keys::Q, GLFW_KEY_Q},
            {Arcade::Keys::R, GLFW_KEY_R},
            {Arcade::Keys::S, GLFW_KEY_S},
            {Arcade::Keys::T, GLFW_KEY_T},
            {Arcade::Keys::U, GLFW_KEY_U},
            {Arcade::Keys::V, GLFW_KEY_V},
            {Arcade::Keys::W, GLFW_KEY_W},
            {Arcade::Keys::X, GLFW_KEY_X},
            {Arcade::Keys::Y, GLFW_KEY_Y},
            {Arcade::Keys::Z, GLFW_KEY_Z},
            {Arcade::Keys::NUM0, GLFW_KEY_0},
            {Arcade::Keys::NUM1, GLFW_KEY_1},
            {Arcade::Keys::NUM2, GLFW_KEY_2},
            {Arcade::Keys::NUM3, GLFW_KEY_3},
            {Arcade::Keys::NUM4, GLFW_KEY_4},
            {Arcade::Keys::NUM5, GLFW_KEY_5},
            {Arcade::Keys::NUM6, GLFW_KEY_6},
            {Arcade::Keys::NUM7, GLFW_KEY_7},
            {Arcade::Keys::NUM8, GLFW_KEY_8},
            {Arcade::Keys::NUM9, GLFW_KEY_9}
        };
        auto it = keyMap.find(key);
        if (it != keyMap.end())
            return it->second;
        return 0;
    }

    static Arcade::Keys convertKeyFromGLFW(int keyCode) {
        switch (keyCode) {
            case GLFW_KEY_UP: return Arcade::Keys::UP;
            case GLFW_KEY_DOWN: return Arcade::Keys::DOWN;
            case GLFW_KEY_LEFT: return Arcade::Keys::LEFT;
            case GLFW_KEY_RIGHT: return Arcade::Keys::RIGHT;
            case GLFW_KEY_ENTER: return Arcade::Keys::ENTER;
            case GLFW_KEY_SPACE: return Arcade::Keys::SPACE;
            case GLFW_KEY_ESCAPE: return Arcade::Keys::ESC;
            case GLFW_KEY_A: return Arcade::Keys::A;
            case GLFW_KEY_B: return Arcade::Keys::B;
            case GLFW_KEY_C: return Arcade::Keys::C;
            case GLFW_KEY_D: return Arcade::Keys::D;
            case GLFW_KEY_E: return Arcade::Keys::E;
            case GLFW_KEY_F: return Arcade::Keys::F;
            case GLFW_KEY_G: return Arcade::Keys::G;
            case GLFW_KEY_H: return Arcade::Keys::H;
            case GLFW_KEY_I: return Arcade::Keys::I;
            case GLFW_KEY_J: return Arcade::Keys::J;
            case GLFW_KEY_K: return Arcade::Keys::K;
            case GLFW_KEY_L: return Arcade::Keys::L;
            case GLFW_KEY_M: return Arcade::Keys::M;
            case GLFW_KEY_N: return Arcade::Keys::N;
            case GLFW_KEY_O: return Arcade::Keys::O;
            case GLFW_KEY_P: return Arcade::Keys::P;
            case GLFW_KEY_Q: return Arcade::Keys::Q;
            case GLFW_KEY_R: return Arcade::Keys::R;
            case GLFW_KEY_S: return Arcade::Keys::S;
            case GLFW_KEY_T: return Arcade::Keys::T;
            case GLFW_KEY_U: return Arcade::Keys::U;
            case GLFW_KEY_V: return Arcade::Keys::V;
            case GLFW_KEY_W: return Arcade::Keys::W;
            case GLFW_KEY_X: return Arcade::Keys::X;
            case GLFW_KEY_Y: return Arcade::Keys::Y;
            case GLFW_KEY_Z: return Arcade::Keys::Z;
            case GLFW_KEY_0: return Arcade::Keys::NUM0;
            case GLFW_KEY_1: return Arcade::Keys::NUM1;
            case GLFW_KEY_2: return Arcade::Keys::NUM2;
            case GLFW_KEY_3: return Arcade::Keys::NUM3;
            case GLFW_KEY_4: return Arcade::Keys::NUM4;
            case GLFW_KEY_5: return Arcade::Keys::NUM5;
            case GLFW_KEY_6: return Arcade::Keys::NUM6;
            case GLFW_KEY_7: return Arcade::Keys::NUM7;
            case GLFW_KEY_8: return Arcade::Keys::NUM8;
            case GLFW_KEY_9: return Arcade::Keys::NUM9;
            default: return Arcade::Keys::NONE;
        }
    }
};

#endif  // SRC_GRAPHICS_OPENGL_OPENGLKEYS_HPP_
