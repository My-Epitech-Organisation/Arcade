// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesEvent
*/
#include "NCursesUtils/NCursesEvent.hpp"

int NCurses::NCursesEvent::pollEvent(WINDOW* window) {
    if (!window)
        return ERR;

    return wgetch(window);
}

bool NCurses::NCursesEvent::isQuitEvent(int key) const {
    return key == 'q' || key == 'Q' || key == KEY_ESC_CODE;
}

bool NCurses::NCursesEvent::isDirectionEvent(int key) const {
    return key == KEY_UP_ARROW || key == KEY_DOWN_ARROW ||
           key == KEY_LEFT_ARROW || key == KEY_RIGHT_ARROW;
}

bool NCurses::NCursesEvent::isActionEvent(int key) const {
    return key == KEY_ENTER_CODE || key == KEY_SPACE_CODE;
}
