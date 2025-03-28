// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Allegro5 Color Conversion
*/

#ifndef SRC_GRAPHICS_ALLEGRO5_ALLEGRO5COLOR_HPP_
#define SRC_GRAPHICS_ALLEGRO5_ALLEGRO5COLOR_HPP_

#include <allegro5/allegro.h>
#include "Models/ColorType.hpp"

class Allegro5Color {
 public:
    static ALLEGRO_COLOR convertColor(Arcade::Color color) {
        switch (color) {
            case Arcade::Color::RED:
                return al_map_rgb(255, 0, 0);
            case Arcade::Color::GREEN:
                return al_map_rgb(0, 255, 0);
            case Arcade::Color::BLUE:
                return al_map_rgb(0, 0, 255);
            case Arcade::Color::YELLOW:
                return al_map_rgb(255, 255, 0);
            case Arcade::Color::MAGENTA:
                return al_map_rgb(255, 0, 255);
            case Arcade::Color::CYAN:
                return al_map_rgb(0, 255, 255);
            case Arcade::Color::ORANGE:
                return al_map_rgb(255, 165, 0);
            case Arcade::Color::BROWN:
                return al_map_rgb(165, 42, 42);
            case Arcade::Color::GREY:
                return al_map_rgb(128, 128, 128);
            case Arcade::Color::WHITE:
                return al_map_rgb(255, 255, 255);
            case Arcade::Color::BLACK:
                return al_map_rgb(0, 0, 0);
            default:
                return al_map_rgb(255, 255, 255);
        }
    }
};

#endif  // SRC_GRAPHICS_ALLEGRO5_ALLEGRO5COLOR_HPP_
