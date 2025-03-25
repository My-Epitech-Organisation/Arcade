// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCurses
*/
#include "NCurses/NCurses.hpp"
#include <stdexcept>
#include <string>
#include <iostream>

NCurses::~NCurses() {
    stop();
}

void NCurses::init(float x, float y) {
    _window = initscr();
    if (!_window) {
        throw std::runtime_error("NCurses could not initialize window!");
    }

    cbreak();
    noecho();
    keypad(_window, TRUE);
    nodelay(_window, TRUE);
    curs_set(0);

    _windowWidth = static_cast<int>(x);
    _windowHeight = static_cast<int>(y);

    if (has_colors()) {
        start_color();
        initColors();
    }
}

void NCurses::stop() {
    if (_window) {
        endwin();
        _window = nullptr;
    }
    _running = false;
}

void NCurses::clearScreen() {
    clear();
}

void NCurses::refreshScreen() {
    refresh();
}

void NCurses::drawEntity(int x, int y, char symbol) {
    mvaddch(y, x, symbol);
}

void NCurses::drawTexture(int x, int y, const std::string &textureId) {
    // Not implemented
    (void)x;
    (void)y;
    (void)textureId;
}

void NCurses::drawText(int x, int y, const std::string &text) {
    mvprintw(y, x, "%s", text.c_str());
}

void NCurses::pollEvents() {
    int ch = getch();
    if (ch != ERR) {
        handleKeyPress(ch);
    }
}

bool NCurses::isOpen() const {
    return _running;
}

extern "C" {
    __attribute__((constructor))
    const char *init_ncurses(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_ncurses(void) {
    }

    Arcade::IDisplayModule* entryPoint(void) {
        return new NCurses();
    }
}
