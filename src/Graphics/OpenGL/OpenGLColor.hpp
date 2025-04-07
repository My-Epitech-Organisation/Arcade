// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** OpenGL Color Conversion
*/

#ifndef SRC_GRAPHICS_OPENGL_OPENGLCOLOR_HPP_
#define SRC_GRAPHICS_OPENGL_OPENGLCOLOR_HPP_

#include <array>
#include "Models/ColorType.hpp"

class OpenGLColor {
 public:
    static std::array<float, 4> convertColor(Arcade::Color color) {
        std::array<float, 4> glColor = {0.0f, 0.0f, 0.0f, 1.0f};
        switch (color) {
            case Arcade::Color::RED:
                glColor = {1.0f, 0.0f, 0.0f, 1.0f};
                break;
            case Arcade::Color::GREEN:
                glColor = {0.0f, 1.0f, 0.0f, 1.0f};
                break;
            case Arcade::Color::BLUE:
                glColor = {0.0f, 0.0f, 1.0f, 1.0f};
                break;
            case Arcade::Color::YELLOW:
                glColor = {1.0f, 1.0f, 0.0f, 1.0f};
                break;
            case Arcade::Color::MAGENTA:
                glColor = {1.0f, 0.0f, 1.0f, 1.0f};
                break;
            case Arcade::Color::CYAN:
                glColor = {0.0f, 1.0f, 1.0f, 1.0f};
                break;
            case Arcade::Color::ORANGE:
                glColor = {1.0f, 0.65f, 0.0f, 1.0f};
                break;
            case Arcade::Color::BROWN:
                glColor = {0.65f, 0.16f, 0.16f, 1.0f};
                break;
            case Arcade::Color::GREY:
                glColor = {0.5f, 0.5f, 0.5f, 1.0f};
                break;
            case Arcade::Color::WHITE:
                glColor = {1.0f, 1.0f, 1.0f, 1.0f};
                break;
            case Arcade::Color::BLACK:
                break;
            default:
                glColor = {1.0f, 1.0f, 1.0f, 1.0f};  // Default to white
                break;
        }
        return glColor;
    }
    static void setGLColor(Arcade::Color color) {
        auto glColor = convertColor(color);
        glColor4f(glColor[0], glColor[1], glColor[2], glColor[3]);
    }
};

#endif  // SRC_GRAPHICS_OPENGL_OPENGLCOLOR_HPP_
