// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesEvent
*/
#include "NCursesUtils/NCursesEvent.hpp"
#include <ncurses.h>
#include <functional>
#include <utility>
#include "Models/KeysType.hpp"
#include "Models/MouseButtonType.hpp"
#include "NCurses/NCursesKeys.hpp"


bool NCurses::NCursesEvent::isKeyPressed(int keyCode) const {
    int ch = getch();
    if (ch == ERR) {
        return false;
    }
    return ch == Arcade::NCursesKeyMap::getNCursesKey(
        static_cast<Arcade::Keys>(keyCode));
}

bool NCurses::NCursesEvent::isMouseButtonPressed(int button) const {
    MEVENT event;
    if (getmouse(&event) == OK) {
        int ncursesButton = Arcade::NCursesKeyMap::getNCursesButton(
            static_cast<Arcade::MouseButton>(button));
        return (event.bstate & ncursesButton) != 0;
    }
    return false;
}

std::pair<size_t, size_t> NCurses::NCursesEvent::getMousePosition() const {
    MEVENT event;
    if (getmouse(&event) == OK) {
        return {static_cast<size_t>(event.x), static_cast<size_t>(event.y)};
    }
    return {0, 0};
}
