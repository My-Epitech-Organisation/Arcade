// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesEntity
*/
#include "NCursesUtils/NCursesEntity.hpp"

NCurses::NCursesEntity::NCursesEntity() {
}

void NCurses::NCursesEntity::drawEntity(WINDOW* window, int x, int y,
                                       char symbol, int colorPair) {
    if (!window)
        return;

    wattron(window, COLOR_PAIR(colorPair));
    mvwaddch(window, y, x, symbol);
    wattroff(window, COLOR_PAIR(colorPair));
}

void NCurses::NCursesEntity::registerEntitySymbol(char symbol, int colorPair) {
    _entityColors[symbol] = colorPair;
}

int NCurses::NCursesEntity::getEntityColor(char symbol) const {
    auto it = _entityColors.find(symbol);
    if (it != _entityColors.end()) {
        return it->second;
    }
    return 0;
}
