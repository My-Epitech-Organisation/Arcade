// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCurses
*/
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include <map>
#include "NCurses/NCurses.hpp"
#include "NCurses/NCursesColor.hpp"
#include "Interface/IArcadeModule.hpp"

NCursesModule::~NCursesModule() {
    stop();
}

void NCursesModule::calculateRatio() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    if (maxX > 0 && maxY > 0) {
        _pixelToCharX = static_cast<float>(maxX) / _referencePixelWidth;
        // For Y coordinates, we'll use the map instead of a direct ratio
        _pixelToCharY = 1.0f; // Keep it at 1.0 for reference
        
        // Clear the position map when recalculating
        _yPositionMap.clear();
    }
}

int NCursesModule::pixelToCharX(int x) const {
    return static_cast<int>(x * _pixelToCharX);
}

int NCursesModule::pixelToCharY(int y) {
    // If we have this Y position mapped already, return the mapped position
    if (_yPositionMap.find(y) != _yPositionMap.end()) {
        return _yPositionMap[y];
    }
    
    // Otherwise, calculate a new position
    int nextY;
    
    if (_yPositionMap.empty()) {
        // First element goes at line 1
        nextY = 1;
    } else {
        // Find the last used Y position
        auto lastItem = _yPositionMap.rbegin();
        // Calculate next position based on the key difference
        int keyDiff = y - lastItem->first;
        
        if (keyDiff < 10) {
            // If positions are very close, use same line
            nextY = lastItem->second;
        } else if (keyDiff < 30) {
            // Medium distance, use next line
            nextY = lastItem->second + 1;
        } else {
            // Large distance, add more spacing
            nextY = lastItem->second + 2;
        }
    }
    
    // Store in map and return
    _yPositionMap[y] = nextY;
    return nextY;
}

void NCursesModule::init(float width, float height) {
    _windowWidth = static_cast<int>(width);
    _windowHeight = static_cast<int>(height);

    try {
        _window.createWindow(_windowWidth, _windowHeight);
        _window.enableKeypad(true);

        if (has_colors()) {
            start_color();
            NCursesColor::initColorPairs();
            _colorManager.initColors();
        } else {
            std::cerr << "Terminal does not support colors" << std::endl;
        }

        // Calculate the pixel-to-character ratio after window creation
        calculateRatio();
        _yPositionMap.clear(); // Ensure map is cleared on initialization

        clearScreen();
        refreshScreen();

        WINDOW* win = _window.getWindow();
        if (win) {
            box(win, 0, 0);
            mvwprintw(win, 1, 1, "NCurses Module Initialized");
            wrefresh(win);
        }
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
    int charX = pixelToCharX(x);
    int charY = pixelToCharY(y);
    
    if (!_window.isOpen() || charX < 0 || charY < 0 ||
        charX >= _window.getWidth() || charY >= _window.getHeight()) {
        return;
    }

    WINDOW* win = _window.getWindow();
    int colorPair = _entity.getEntityColor(symbol);

    _entity.drawEntity(win, charX, charY, symbol, colorPair);
}

void NCursesModule::drawTexture(int x, int y, const std::string &textureId) {
    int charX = pixelToCharX(x);
    int charY = pixelToCharY(y);
    
    // Implementation (if any) would go here
}

void NCursesModule::drawText(const std::string &text,
    int x, int y, Arcade::Color color) {
    int charX = pixelToCharX(x);
    int charY = pixelToCharY(y);
    
    if (!_window.isOpen() || charX < 0 || charY < 0 ||
        charX >= _window.getWidth() || charY >= _window.getHeight()) {
        return;
    }
    WINDOW* win = _window.getWindow();
    _text.drawText(win, text, charX, charY, color);
}

void NCursesModule::pollEvents() {
    if (!_window.isOpen()) {
        _running = false;
        return;
    }

    int ch = _window.getChar();
    if (ch == 27) {
        ungetch(ch);
    } else if (ch == KEY_RESIZE) {
        int newMaxY, newMaxX;
        endwin();
        refresh();
        getmaxyx(stdscr, newMaxY, newMaxX);
        _windowWidth = newMaxX;
        _windowHeight = newMaxY;
        mmask_t currentMouseMask;
        mousemask(0, &currentMouseMask);
        _window.closeWindow();
        _window.createWindow(_windowWidth, _windowHeight);
        _window.enableKeypad(true);
        mousemask(currentMouseMask, NULL);
        
        calculateRatio();
        _yPositionMap.clear(); // Reset the mapping on resize
        
        clearScreen();
        refreshScreen();
    } else if (ch == KEY_MOUSE) {
        MEVENT event;
        if (getmouse(&event) == OK) {
            // Store the mouse event for later processing
            // This ensures isMouseButtonPressed will have access to the latest event
        }
    } else if (ch != ERR) {
        ungetch(ch);
    }
}

bool NCursesModule::isOpen() const {
    return _running && _window.isOpen();
}

const std::string& NCursesModule::getName() const {
    return _name;
}

int NCursesModule::getWidth() const {
    return _windowWidth;
}

int NCursesModule::getHeight() const {
    return _windowHeight;
}

bool NCursesModule::isKeyPressed(int keyCode) {
    return _event.isKeyPressed(keyCode);
}

bool NCursesModule::isMouseButtonPressed(int button) const {
    return _event.isMouseButtonPressed(button);
}

std::pair<size_t, size_t> NCursesModule::getMousePosition() const {
    return _event.getMousePosition();
}

extern "C" {
    __attribute__((constructor))
    const char *init_ncurses(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_ncurses(void) {
    }

    Arcade::IArcadeModule* entryPoint(void) {
        return new NCursesModule();
    }
}
