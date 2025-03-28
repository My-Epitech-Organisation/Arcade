// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesText
*/

#include "NCursesUtils/NCursesText.hpp"
#include <string>
#include <NCurses/NCursesColor.hpp>

void NCurses::NCursesText::drawText(WINDOW* window, const std::string &text,
    int x, int y, Arcade::Color color) {
    if (!window)
        return;
    if (NCursesColor::convertColor(color))
        drawTextWithColor(window, text, x, y,
            NCursesColor::convertColor(color));
    else
        mvwprintw(window, y, x, "%s", text.c_str());
}

void NCurses::NCursesText::drawTextWithColor(WINDOW* window,
    const std::string &text,
    int x, int y, int colorPair) {
    if (!window)
        return;
    wattron(window, COLOR_PAIR(colorPair));
    mvwprintw(window, y, x, "%s", text.c_str());
    wattroff(window, COLOR_PAIR(colorPair));
}
