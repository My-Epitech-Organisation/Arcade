// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesText
*/
#include "NCursesUtils/NCursesText.hpp"

void NCurses::NCursesText::drawText(WINDOW* window, const std::string &text,
                                   int x, int y) {
    if (!window)
        return;

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
