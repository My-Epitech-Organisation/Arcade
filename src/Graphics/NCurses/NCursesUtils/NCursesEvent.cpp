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
    int key = _lastKeyPressed;
    
    auto arcadeKey = static_cast<Arcade::Keys>(keyCode);
    int ncursesKey = Arcade::NCursesKeyMap::getNCursesKey(arcadeKey);
    
    return (key != ERR && key == ncursesKey);
}

void NCurses::NCursesEvent::storeKeyEvent(int key) {
    _lastKeyPressed = key;
}

void NCurses::NCursesEvent::storeMouseEvent(const MEVENT& event) {
    _lastMouseEvent = event;
    _hasMouseEvent = true;
}

void NCurses::NCursesEvent::resetMouseEvent() {
    _hasMouseEvent = false;
}

bool NCurses::NCursesEvent::isMouseButtonPressed(int button) const {
    if (!_hasMouseEvent) {
        return false;
    }
    
    auto arcadeButton = static_cast<Arcade::MouseButton>(button);
    unsigned long ncursesButton = Arcade::NCursesKeyMap::getNCursesButton(arcadeButton);
    
    return (_lastMouseEvent.bstate & ncursesButton) != 0;
}

std::pair<size_t, size_t> NCurses::NCursesEvent::getMousePosition() const {
    if (_hasMouseEvent) {
        return {static_cast<size_t>(_lastMouseEvent.x), 
                static_cast<size_t>(_lastMouseEvent.y)};
    }
    return {0, 0};
}
