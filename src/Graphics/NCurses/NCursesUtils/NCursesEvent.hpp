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
    #include <utility>

namespace NCurses {
class NCursesEvent {
 public:
    NCursesEvent() = default;

    bool isKeyPressed(int keyCode) const;
    bool isMouseButtonPressed(int button) const;
    std::pair<size_t, size_t> getMousePosition() const;
};
}  // namespace NCurses

#endif  // SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESEVENT_HPP_
