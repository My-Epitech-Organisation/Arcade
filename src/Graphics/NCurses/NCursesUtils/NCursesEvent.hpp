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
    MEVENT _lastMouseEvent;
    bool _hasMouseEvent;
    int _lastKeyPressed;

    const NCursesModule* _module;

 public:
    NCursesEvent() : _hasMouseEvent(false), _lastKeyPressed(ERR),
        _module(nullptr) {}

    bool isKeyPressed(int keyCode) const;
    bool isMouseButtonPressed(int button) const;

    std::pair<size_t, size_t> getRawMousePosition() const;

    std::pair<size_t, size_t> getMousePosition() const;

    void setModule(const NCursesModule* module) { _module = module; }

    void storeMouseEvent(const MEVENT& event);
    void storeKeyEvent(int key);
    void resetMouseEvent();
};
}  // namespace NCurses

#endif  // SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESEVENT_HPP_
