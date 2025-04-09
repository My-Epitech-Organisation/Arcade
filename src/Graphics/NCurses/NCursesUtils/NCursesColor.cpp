// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesColor
*/

#include "NCursesUtils/NCursesColor.hpp"
#include <string>
#include <iostream>
#include "Models/ColorType.hpp"

NCurses::NCursesColor::NCursesColor() {
}

void NCurses::NCursesColor::initColors() {
    if (has_colors()) {
        start_color();
        try {
            addColorPair("default", COLOR_WHITE, COLOR_BLACK);
            addColorPair("red", COLOR_RED, COLOR_BLACK);
            addColorPair("green", COLOR_GREEN, COLOR_BLACK);
            addColorPair("blue", COLOR_BLUE, COLOR_BLACK);
            addColorPair("yellow", COLOR_YELLOW, COLOR_BLACK);
            addColorPair("magenta", COLOR_MAGENTA, COLOR_BLACK);
            addColorPair("cyan", COLOR_CYAN, COLOR_BLACK);
            addColorPair("orange", COLOR_RED, COLOR_BLACK);
            addColorPair("brown", COLOR_RED, COLOR_BLACK);
            addColorPair("grey", COLOR_WHITE, COLOR_BLACK);
            addColorPair("white", COLOR_WHITE, COLOR_BLACK);
            addColorPair("black", COLOR_BLACK, COLOR_BLACK);

            for (int i = 0; i <= static_cast<int>(Arcade::Color::NOCOLOR); i++) {
                addColorPairByEnum(static_cast<Arcade::Color>(i));
            }
        } catch (const std::exception &e) {
            std::cerr << "Failed to initialize color pairs: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Terminal does not support colors" << std::endl;
    }
}

int NCurses::NCursesColor::getColorPair(const std::string &colorName) {
    auto it = _colorPairs.find(colorName);
    if (it != _colorPairs.end()) {
        return it->second;
    }
    return 0;
}

int NCurses::NCursesColor::getColorPair(Arcade::Color color) {
    auto it = _enumColorPairs.find(color);
    if (it != _enumColorPairs.end()) {
        return it->second;
    }
    return 0;
}

void NCurses::NCursesColor::addColorPair(const std::string &colorName,
                                         int16_t fg, int16_t bg) {
    init_pair(_nextPairId, fg, bg);
    _colorPairs[colorName] = _nextPairId++;
}

void NCurses::NCursesColor::addColorPairByEnum(Arcade::Color color) {
    int16_t fg = COLOR_WHITE;

    switch (color) {
        case Arcade::Color::RED:
            fg = COLOR_RED;
            break;
        case Arcade::Color::GREEN:
            fg = COLOR_GREEN;
            break;
        case Arcade::Color::BLUE:
            fg = COLOR_BLUE;
            break;
        case Arcade::Color::YELLOW:
            fg = COLOR_YELLOW;
            break;
        case Arcade::Color::MAGENTA:
            fg = COLOR_MAGENTA;
            break;
        case Arcade::Color::CYAN:
            fg = COLOR_CYAN;
            break;
        case Arcade::Color::ORANGE:
            fg = COLOR_RED;
            break;
        case Arcade::Color::BROWN:
            fg = COLOR_RED;
            break;
        case Arcade::Color::GREY:
            fg = COLOR_WHITE;
            break;
        case Arcade::Color::WHITE:
            fg = COLOR_WHITE;
            break;
        case Arcade::Color::BLACK:
            fg = COLOR_BLACK;
            break;
        default:
            fg = COLOR_WHITE;
            break;
    }

    init_pair(_nextPairId, fg, COLOR_BLACK);
    _enumColorPairs[color] = _nextPairId++;
}

void NCurses::NCursesColor::applyColorPair(int colorPair) {
    attron(COLOR_PAIR(colorPair));
}

void NCurses::NCursesColor::applyColor(Arcade::Color color) {
    int pair = getColorPair(color);
    attron(COLOR_PAIR(pair));
}
