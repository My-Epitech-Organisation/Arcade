// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** NCurses Color Implementation
*/

#include "NCurses/NCursesColor.hpp"

// Initialize static member here
bool NCursesColor::_initialized = false;

void NCursesColor::initColorPairs() {
    if (_initialized) return;
    start_color();
    init_pair(COLOR_PAIR_OFFSET + static_cast<int>(Arcade::Color::RED),
              COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_OFFSET + static_cast<int>(Arcade::Color::GREEN),
              COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_OFFSET + static_cast<int>(Arcade::Color::BLUE),
              COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_PAIR_OFFSET + static_cast<int>(Arcade::Color::YELLOW),
              COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_OFFSET + static_cast<int>(Arcade::Color::MAGENTA),
              COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_PAIR_OFFSET + static_cast<int>(Arcade::Color::CYAN),
              COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_OFFSET + static_cast<int>(Arcade::Color::ORANGE),
              COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_OFFSET + static_cast<int>(Arcade::Color::BROWN),
              COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_OFFSET + static_cast<int>(Arcade::Color::GREY),
              COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_OFFSET + static_cast<int>(Arcade::Color::WHITE),
              COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_OFFSET + static_cast<int>(Arcade::Color::BLACK),
              COLOR_BLACK, COLOR_BLACK);
    _initialized = true;
}

int NCursesColor::convertColor(Arcade::Color color) {
    if (!_initialized) {
        initColorPairs();
    }
    return COLOR_PAIR(COLOR_PAIR_OFFSET + static_cast<int>(color));
}
