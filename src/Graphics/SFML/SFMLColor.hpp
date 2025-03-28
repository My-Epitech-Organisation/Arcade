// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** SFML Color Conversion
*/

#ifndef SRC_GRAPHICS_SFML_SFMLCOLOR_HPP_
#define SRC_GRAPHICS_SFML_SFMLCOLOR_HPP_

#include <SFML/Graphics.hpp>
#include "Models/ColorType.hpp"

class SFMLColor {
 public:
    static sf::Color convertColor(Arcade::Color color) {
        switch (color) {
            case Arcade::Color::RED:
                return sf::Color::Red;
            case Arcade::Color::GREEN:
                return sf::Color::Green;
            case Arcade::Color::BLUE:
                return sf::Color::Blue;
            case Arcade::Color::YELLOW:
                return sf::Color::Yellow;
            case Arcade::Color::MAGENTA:
                return sf::Color::Magenta;
            case Arcade::Color::CYAN:
                return sf::Color::Cyan;
            case Arcade::Color::ORANGE:
                return sf::Color(255, 165, 0);
            case Arcade::Color::BROWN:
                return sf::Color(165, 42, 42);
            case Arcade::Color::GREY:
                return sf::Color(128, 128, 128);
            case Arcade::Color::WHITE:
                return sf::Color::White;
            case Arcade::Color::BLACK:
                return sf::Color::Black;
            default:
                return sf::Color::White;
        }
    }
};

#endif  // SRC_GRAPHICS_SFML_SFMLCOLOR_HPP_
