// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesUtils
*/
#include "NCurses/NCurses.hpp"
#include <stdexcept>
#include <string>

void NCurses::createWindow(int width, int height) {
    resizeterm(height, width);
}

void NCurses::initColors() {
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
}

void NCurses::handleKeyPress(int key) {
    switch (key) {
        case 'q':
        case 'Q':
        case 27:
            _running = false;
            break;
        default:
            break;
    }
}

const std::string& NCurses::getName() const {
    return _name;
}
