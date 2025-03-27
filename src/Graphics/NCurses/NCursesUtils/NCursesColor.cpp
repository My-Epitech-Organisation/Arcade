// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesColor
*/
#include "NCursesUtils/NCursesColor.hpp"

NCurses::NCursesColor::NCursesColor() {
}

void NCurses::NCursesColor::initColors() {
    if (has_colors()) {
        start_color();

        addColorPair("default", COLOR_WHITE, COLOR_BLACK);
        addColorPair("red", COLOR_RED, COLOR_BLACK);
        addColorPair("green", COLOR_GREEN, COLOR_BLACK);
        addColorPair("blue", COLOR_BLUE, COLOR_BLACK);
        addColorPair("yellow", COLOR_YELLOW, COLOR_BLACK);
        addColorPair("magenta", COLOR_MAGENTA, COLOR_BLACK);
        addColorPair("cyan", COLOR_CYAN, COLOR_BLACK);
    }
}

int NCurses::NCursesColor::getColorPair(const std::string &colorName) {
    auto it = _colorPairs.find(colorName);
    if (it != _colorPairs.end()) {
        return it->second;
    }
    return 0;
}

void NCurses::NCursesColor::addColorPair(const std::string &colorName,
                                          short fg, short bg) {
    init_pair(_nextPairId, fg, bg);
    _colorPairs[colorName] = _nextPairId++;
}

void NCurses::NCursesColor::applyColorPair(int colorPair) {
    attron(COLOR_PAIR(colorPair));
}
