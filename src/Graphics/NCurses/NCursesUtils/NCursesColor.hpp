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

namespace NCurses {
class NCursesColor {
 public:
    NCursesColor();
    void initColors();
    int getColorPair(const std::string &colorName);
    void addColorPair(const std::string &colorName, short fg, short bg);
    void applyColorPair(int colorPair);

 private:
    std::map<std::string, int> _colorPairs;
    int _nextPairId = 1;
};
}  // namespace NCurses

#endif  // SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESCOLOR_HPP_
