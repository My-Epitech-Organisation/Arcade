// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesEvent
*/

#ifndef SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESEVENT_HPP_
    #define SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESEVENT_HPP_
    #include <ncurses.h>
    #include <functional>

namespace NCurses {
class NCursesEvent {
 public:
    NCursesEvent() = default;
    int pollEvent(WINDOW* window);
    bool isQuitEvent(int key) const;
    bool isDirectionEvent(int key) const;
    bool isActionEvent(int key) const;

    static const int KEY_UP_ARROW = KEY_UP;
    static const int KEY_DOWN_ARROW = KEY_DOWN;
    static const int KEY_LEFT_ARROW = KEY_LEFT;
    static const int KEY_RIGHT_ARROW = KEY_RIGHT;

    static const int KEY_ENTER_CODE = 10;
    static const int KEY_SPACE_CODE = 32;
    static const int KEY_ESC_CODE = 27;
};
}  // namespace NCurses

#endif  // SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESEVENT_HPP_
