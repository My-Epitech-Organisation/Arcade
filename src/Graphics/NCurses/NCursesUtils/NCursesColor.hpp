// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesColor
*/

#ifndef SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESCOLOR_HPP_
    #define SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESCOLOR_HPP_
    #include <ncurses.h>
    #include <string>
    #include <map>
    #include "Models/ColorType.hpp"

namespace NCurses {
class NCursesColor {
 public:
    NCursesColor();
    void initColors();
    int getColorPair(const std::string &colorName);
    int getColorPair(Arcade::Color color);
    void addColorPair(const std::string &colorName, int16_t fg, int16_t bg);
    void addColorPairByEnum(Arcade::Color color);
    void applyColorPair(int colorPair);
    void applyColor(Arcade::Color color);

 private:
    std::map<std::string, int> _colorPairs;
    std::map<Arcade::Color, int> _enumColorPairs;
    int _nextPairId = 1;
};
}  // namespace NCurses

#endif  // SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESCOLOR_HPP_
