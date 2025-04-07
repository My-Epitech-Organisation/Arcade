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
    static int lastKey = ERR;
    int key = lastKey;

    if (key == ERR) {
        key = getch();
        if (key != ERR) {
            lastKey = key;
        }
    }

    bool result = (key != ERR && key == Arcade::NCursesKeyMap::getNCursesKey(
        static_cast<Arcade::Keys>(keyCode)));
    if (result) {
        lastKey = ERR;
    }
    return result;
}

bool NCurses::NCursesEvent::isMouseButtonPressed(int button) const {
    static MEVENT lastEvent;
    static bool hasEvent = false;

    if (!hasEvent) {
        int ch = getch();
        if (ch == KEY_MOUSE) {
            if (getmouse(&lastEvent) == OK) {
                hasEvent = true;
            } else {
                return false;
            }
        } else if (ch != ERR) {
            ungetch(ch);
            return false;
        } else {
            return false;
        }
    }
    int ncursesButton = Arcade::NCursesKeyMap::getNCursesButton(
        static_cast<Arcade::MouseButton>(button));
    bool result = (lastEvent.bstate & ncursesButton) != 0;
    if (result) {
        hasEvent = false;
    }
    return result;
}

std::pair<size_t, size_t> NCurses::NCursesEvent::getMousePosition() const {
    MEVENT event;
    if (getmouse(&event) == OK) {
        return {static_cast<size_t>(event.x), static_cast<size_t>(event.y)};
    }
    static std::pair<size_t, size_t> lastPosition = {0, 0};
    return lastPosition;
}
