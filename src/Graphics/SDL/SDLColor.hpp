// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** SDL Color Conversion
*/

#ifndef SRC_GRAPHICS_SDL_SDLCOLOR_HPP_
#define SRC_GRAPHICS_SDL_SDLCOLOR_HPP_
#include <SDL2/SDL.h>
#include "Models/ColorType.hpp"

class SDLColor {
 public:
    static SDL_Color convertColor(Arcade::Color color) {
        SDL_Color sdlColor = {0, 0, 0, 255};
        switch (color) {
            case Arcade::Color::RED:
                sdlColor.r = 255;
                break;
            case Arcade::Color::GREEN:
                sdlColor.g = 255;
                break;
            case Arcade::Color::BLUE:
                sdlColor.b = 255;
                break;
            case Arcade::Color::YELLOW:
                sdlColor.r = 255;
                sdlColor.g = 255;
                break;
            case Arcade::Color::MAGENTA:
                sdlColor.r = 255;
                sdlColor.b = 255;
                break;
            case Arcade::Color::CYAN:
                sdlColor.g = 255;
                sdlColor.b = 255;
                break;
            case Arcade::Color::ORANGE:
                sdlColor.r = 255;
                sdlColor.g = 165;
                break;
            case Arcade::Color::BROWN:
                sdlColor.r = 165;
                sdlColor.g = 42;
                sdlColor.b = 42;
                break;
            case Arcade::Color::GREY:
                sdlColor.r = 128;
                sdlColor.g = 128;
                sdlColor.b = 128;
                break;
            case Arcade::Color::WHITE:
                sdlColor.r = 255;
                sdlColor.g = 255;
                sdlColor.b = 255;
                break;
            case Arcade::Color::BLACK:
                break;
            default:
                sdlColor.r = 255;
                sdlColor.g = 255;
                sdlColor.b = 255;
                break;
        }
        return sdlColor;
    }
};

#endif  // SRC_GRAPHICS_SDL_SDLCOLOR_HPP_
