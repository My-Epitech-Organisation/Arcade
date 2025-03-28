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
    using Color = std::array<float, 4>;

    static Color convertColor(Arcade::Color color) {
        switch (color) {
            case Arcade::Color::RED:
                return {1.0f, 0.0f, 0.0f, 1.0f};
            case Arcade::Color::GREEN:
                return {0.0f, 1.0f, 0.0f, 1.0f};
            case Arcade::Color::BLUE:
                return {0.0f, 0.0f, 1.0f, 1.0f};
            case Arcade::Color::YELLOW:
                return {1.0f, 1.0f, 0.0f, 1.0f};
            case Arcade::Color::MAGENTA:
                return {1.0f, 0.0f, 1.0f, 1.0f};
            case Arcade::Color::CYAN:
                return {0.0f, 1.0f, 1.0f, 1.0f};
            case Arcade::Color::ORANGE:
                return {1.0f, 0.65f, 0.0f, 1.0f};
            case Arcade::Color::BROWN:
                return {0.65f, 0.16f, 0.16f, 1.0f};
            case Arcade::Color::GREY:
                return {0.5f, 0.5f, 0.5f, 1.0f};
            case Arcade::Color::WHITE:
                return {1.0f, 1.0f, 1.0f, 1.0f};
            case Arcade::Color::BLACK:
                return {0.0f, 0.0f, 0.0f, 1.0f};
            default:
                return {1.0f, 1.0f, 1.0f, 1.0f};
        }
    }
};

#endif  // SRC_GRAPHICS_OPENGL_OPENGLCOLOR_HPP_