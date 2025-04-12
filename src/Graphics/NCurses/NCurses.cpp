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
#include <cstring>
#include "NCurses/NCurses.hpp"
#include "NCurses/NCursesColor.hpp"
#include "Interface/IArcadeModule.hpp"
#include "Models/ModuleInfos.hpp"
#include "Interface/Display/IDrawableComponent.hpp"

NCursesModule::~NCursesModule() {
    stop();
}

void NCursesModule::calculateRatio() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    if (maxX > 0 && maxY > 0) {
        _pixelToCharX = static_cast<float>(maxX) / _referencePixelWidth;
        _pixelToCharY = 1.0f;
        _yPositionMap.clear();
        _reverseYMap.clear();
    }
}

int NCursesModule::pixelToCharX(int x) const {
    return static_cast<int>(x * _pixelToCharX);
}

int NCursesModule::pixelToCharY(int y) {
    if (_yPositionMap.find(y) != _yPositionMap.end()) {
        return _yPositionMap[y];
    }

    int nextY;
    if (_yPositionMap.empty()) {
        nextY = 1;
    } else {
        auto lastItem = _yPositionMap.rbegin();
        int keyDiff = y - lastItem->first;
        if (keyDiff < 10) {
            nextY = lastItem->second;
        } else if (keyDiff < 30) {
            nextY = lastItem->second + 1;
        } else {
            nextY = lastItem->second + 2;
        }
    }
    _yPositionMap[y] = nextY;
    _reverseYMap[nextY] = y;  // Store the reverse mapping too
    return nextY;
}

int NCursesModule::charToPixelX(int x) const {
    if (_pixelToCharX <= 0) return 0;
    return static_cast<int>(x / _pixelToCharX);
}

int NCursesModule::charToPixelY(int y) const {
    // Look up the reverse mapping
    auto it = _reverseYMap.find(y);
    if (it != _reverseYMap.end()) {
        return it->second;
    }

    // If no mapping exists, estimate based on the current position
    // This is approximate but better than returning character positions
    return y * 20;
}

void NCursesModule::init(const Arcade::IWindowModule& windowParam) {
    _windowWidth = static_cast<int>(windowParam.getWidth());
    _windowHeight = static_cast<int>(windowParam.getHeight());

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

        calculateRatio();
        _yPositionMap.clear();

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

void NCursesModule::drawDrawable(std::shared_ptr<Arcade::IDrawableComponent>
drawable) {
    if (!drawable->isRenderable())
        return;
    if (drawable->getText().size() == 1) {
        drawEntity(drawable->getPositionX(),
            drawable->getPositionY(),
            drawable->getCharacter());
    }
}

void NCursesModule::drawTexture(int x, int y, const std::string &textureId) {
    // Textures are not supported in NCurses
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
    _event.setModule(this);
    int ch = _window.getChar();
    if (ch == KEY_RESIZE) {
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
        _yPositionMap.clear();

        clearScreen();
        refreshScreen();
    } else if (ch == KEY_MOUSE) {
        MEVENT event;
        if (getmouse(&event) == OK) {
            _event.storeMouseEvent(event);
        }
    } else if (ch != ERR) {
        _event.storeKeyEvent(ch);
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

    Arcade::ModuleInfos module_infos() {
        return {"NCurses", "IDK",
            "IDK",
            "./lib/arcade_ncurses.so", Arcade::ModuleType::GRAPHIC_LIB};
    }
}
