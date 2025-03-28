// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** GTK+ Color Conversion
*/

#ifndef SRC_GRAPHICS_GTK__GTKCOLOR_HPP_
#define SRC_GRAPHICS_GTK__GTKCOLOR_HPP_

#include <gtk/gtk.h>
#include "Models/ColorType.hpp"

class GTKColor {
 public:
    static GdkRGBA convertColor(Arcade::Color color) {
        GdkRGBA gtkColor = {0.0, 0.0, 0.0, 1.0};
        switch (color) {
            case Arcade::Color::RED:
                gtkColor.red = 1.0;
                break;
            case Arcade::Color::GREEN:
                gtkColor.green = 1.0;
                break;
            case Arcade::Color::BLUE:
                gtkColor.blue = 1.0;
                break;
            case Arcade::Color::YELLOW:
                gtkColor.red = 1.0;
                gtkColor.green = 1.0;
                break;
            case Arcade::Color::MAGENTA:
                gtkColor.red = 1.0;
                gtkColor.blue = 1.0;
                break;
            case Arcade::Color::CYAN:
                gtkColor.green = 1.0;
                gtkColor.blue = 1.0;
                break;
            case Arcade::Color::ORANGE:
                gtkColor.red = 1.0;
                gtkColor.green = 165.0/255.0;
                break;
            case Arcade::Color::BROWN:
                gtkColor.red = 165.0/255.0;
                gtkColor.green = 42.0/255.0;
                gtkColor.blue = 42.0/255.0;
                break;
            case Arcade::Color::GREY:
                gtkColor.red = 128.0/255.0;
                gtkColor.green = 128.0/255.0;
                gtkColor.blue = 128.0/255.0;
                break;
            case Arcade::Color::WHITE:
                gtkColor.red = 1.0;
                gtkColor.green = 1.0;
                gtkColor.blue = 1.0;
                break;
            case Arcade::Color::BLACK:
                // Already set to black
                break;
            default:
                gtkColor.red = 1.0;
                gtkColor.green = 1.0;
                gtkColor.blue = 1.0;
                break;
        }
        return gtkColor;
    }
};

#endif  // SRC_GRAPHICS_GTK__GTKCOLOR_HPP_
