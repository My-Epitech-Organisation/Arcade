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

class NCursesModule;

namespace NCurses {
class NCursesEvent {
 private:
    // Store the last mouse event and its state
    MEVENT _lastMouseEvent;
    bool _hasMouseEvent;
    int _lastKeyPressed;

 public:
    NCursesEvent() : _hasMouseEvent(false), _lastKeyPressed(ERR) {}

    bool isKeyPressed(int keyCode) const;
    bool isMouseButtonPressed(int button) const;

    // Get raw mouse position (in character coordinates)
    std::pair<size_t, size_t> getRawMousePosition() const;

    // Get mouse position (converted to pixel coordinates)
    std::pair<size_t, size_t> getMousePosition(const NCursesModule* module) const;

    // Legacy method for backwards compatibility
    std::pair<size_t, size_t> getMousePosition() const;

    // Add methods to update state from the NCurses module
    void storeMouseEvent(const MEVENT& event);
    void storeKeyEvent(int key);
    void resetMouseEvent();
};
}  // namespace NCurses

#endif  // SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESEVENT_HPP_
