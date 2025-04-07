// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesWindow
*/
#include "NCursesUtils/NCursesWindow.hpp"
#include <stdexcept>
#include <cstdio>

NCurses::NCursesWindow::~NCursesWindow() {
    if (_isOpen) {
        closeWindow();
    }
}

void NCurses::NCursesWindow::createWindow(int width, int height) {
    if (!isendwin()) {
        initscr();
    } else {
        refresh();
    }
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, TRUE);
    timeout(100);

    mmask_t mouseMask = ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION;
    mousemask(mouseMask, NULL);
    mouseinterval(0);

    // Enable extended mouse tracking (works in xterm and similar)
    printf("\033[?1003h\n");

    _window = newwin(height, width, 0, 0);
    if (!_window) {
        endwin();
        throw std::runtime_error("Failed to create NCurses window");
    }
    _windowHeight = height;
    _windowWidth = width;
    _isOpen = true;
    wrefresh(_window);
}

void NCurses::NCursesWindow::clearWindow() {
    if (!_isOpen) return;
    wclear(_window);
}

void NCurses::NCursesWindow::refreshWindow() {
    if (!_isOpen) return;
    wrefresh(_window);
}

void NCurses::NCursesWindow::closeWindow() {
    if (!_isOpen) return;

    // Disable extended mouse tracking before closing
    printf("\033[?1003l\n");

    if (_window) {
        werase(_window);
        wrefresh(_window);
        delwin(_window);
        _window = nullptr;
    }
    endwin();
    _isOpen = false;
}

WINDOW* NCurses::NCursesWindow::getWindow() const {
    return _window;
}

bool NCurses::NCursesWindow::isOpen() const {
    return _isOpen;
}

void NCurses::NCursesWindow::enableKeypad(bool enable) {
    if (!_isOpen) return;
    keypad(_window, enable ? TRUE : FALSE);
}

int NCurses::NCursesWindow::getChar() {
    if (!_isOpen) return ERR;
    return wgetch(_window);
}

int NCurses::NCursesWindow::getWidth() const {
    return _windowWidth;
}

int NCurses::NCursesWindow::getHeight() const {
    return _windowHeight;
}
