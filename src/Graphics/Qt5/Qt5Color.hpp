// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Qt5 Color Conversion
*/

#ifndef SRC_GRAPHICS_QT5_QT5COLOR_HPP_
#define SRC_GRAPHICS_QT5_QT5COLOR_HPP_

#include <QColor>
#include "Models/ColorType.hpp"

class Qt5Color {
 public:
    static QColor convertColor(Arcade::Color color) {
        switch (color) {
            case Arcade::Color::RED:
                return QColor(255, 0, 0);
            case Arcade::Color::GREEN:
                return QColor(0, 255, 0);
            case Arcade::Color::BLUE:
                return QColor(0, 0, 255);
            case Arcade::Color::YELLOW:
                return QColor(255, 255, 0);
            case Arcade::Color::MAGENTA:
                return QColor(255, 0, 255);
            case Arcade::Color::CYAN:
                return QColor(0, 255, 255);
            case Arcade::Color::ORANGE:
                return QColor(255, 165, 0);
            case Arcade::Color::BROWN:
                return QColor(165, 42, 42);
            case Arcade::Color::GREY:
                return QColor(128, 128, 128);
            case Arcade::Color::WHITE:
                return QColor(255, 255, 255);
            case Arcade::Color::BLACK:
                return QColor(0, 0, 0);
            default:
                return QColor(255, 255, 255);
        }
    }
};

#endif  // SRC_GRAPHICS_QT5_QT5COLOR_HPP_
