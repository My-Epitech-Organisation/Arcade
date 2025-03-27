// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCurses
*/
#include "NCurses/NCurses.hpp"
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>

NCursesModule::~NCursesModule() {
    stop();
}

void NCursesModule::init(float width, float height) {
    _windowWidth = static_cast<int>(width);
    _windowHeight = static_cast<int>(height);

    try {
        _window.createWindow(_windowWidth, _windowHeight);
        _window.enableKeypad(true);
        _colorManager.initColors();
    } catch (const std::exception &e) {
        std::cerr << "NCurses initialization error: " << e.what() << std::endl;
        _running = false;
    }
}

void NCursesModule::stop() {
    _window.closeWindow();
    _running = false;
}

void NCursesModule::clearScreen() {
    _window.clearWindow();
}

void NCursesModule::refreshScreen() {
    _window.refreshWindow();
}

void NCursesModule::drawEntity(int x, int y, char symbol) {
    if (!_window.isOpen() || x < 0 || y < 0 ||
        x >= _windowWidth || y >= _windowHeight) {
        return;
    }

    WINDOW* win = _window.getWindow();
    int colorPair = _entity.getEntityColor(symbol);

    _entity.drawEntity(win, x, y, symbol, colorPair);
}

void NCursesModule::drawTexture(int x, int y, const std::string &textureId) {
}

void NCursesModule::drawText(int x, int y, const std::string &text) {
    if (!_window.isOpen() || x < 0 || y < 0 ||
        x >= _windowWidth || y >= _windowHeight) {
        return;
    }

    WINDOW* win = _window.getWindow();
    _text.drawText(win, text, x, y);
}

void NCursesModule::pollEvents() {
    if (!_window.isOpen()) {
        _running = false;
        return;
    }

    int key = _window.getChar();
    if (_event.isQuitEvent(key)) {
        _running = false;
    }
}

bool NCursesModule::isOpen() const {
    return _running && _window.isOpen();
}

const std::string& NCursesModule::getName() const {
    return _name;
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
        return new NCursesModule();
    }
}
