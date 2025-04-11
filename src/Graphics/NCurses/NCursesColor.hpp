// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** NCurses Color Conversion
*/

#ifndef SRC_GRAPHICS_NCURSES_NCURSESCOLOR_HPP_
#define SRC_GRAPHICS_NCURSES_NCURSESCOLOR_HPP_
#include <ncurses.h>
#include "Models/ColorType.hpp"

class NCursesColor {
 private:
    static bool _initialized;  // Just declare it here, don't initialize
    static const int COLOR_PAIR_OFFSET = 2;

 public:
    static void initColorPairs();
    static int convertColor(Arcade::Color color);
};

#endif  // SRC_GRAPHICS_NCURSES_NCURSESCOLOR_HPP_
