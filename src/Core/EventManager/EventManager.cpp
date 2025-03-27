// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** EventManager implementation
*/

#include <iostream>
#include "EventManager/EventManager.hpp"
#include "Models/KeysType.hpp"
#include "Interface/Display/IDisplayModule.hpp"
#include "Models/EventType.hpp"
#include "EventManager/KeyEvent/RawInputState.hpp"

namespace Arcade {

// Add default constructor
EventManager::EventManager()
    : AEventManager(), _mouseX(0), _mouseY(0) {}

void EventManager::updateInputState(const RawInputState& state) {
    for (const auto& [key, isPressed] : state.keyStates) {
        bool wasPressed = _keyStates[key];
        if (isPressed && !wasPressed) {
            publish(KeyEvent(key, EventType::KEY_PRESSED));
        } else if (!isPressed && wasPressed) {
            publish(KeyEvent(key, EventType::KEY_RELEASED));
        }
        _keyStates[key] = isPressed;
    }

    for (const auto& [button, isPressed] : state.mouseButtons) {
        bool wasPressed = _mouseButtonStates[button];
        if (isPressed && !wasPressed) {
            publishMouseEvent(MouseEvent(button,
                EventType::MOUSE_BUTTON_PRESSED, state.mouseX, state.mouseY));
        } else if (!isPressed && wasPressed) {
            publishMouseEvent(MouseEvent(button,
                EventType::MOUSE_BUTTON_RELEASED, state.mouseX, state.mouseY));
        }
        _mouseButtonStates[button] = isPressed;
    }
    if (state.mouseX != _mouseX || state.mouseY != _mouseY) {
        publishMouseEvent(MouseEvent(MouseButton::NONE,
            EventType::MOUSE_MOVED, state.mouseX, state.mouseY));
        _mouseX = state.mouseX;
        _mouseY = state.mouseY;
    }
}

void EventManager::setKeyPressed(Arcade::Keys key, bool pressed) {
    _keyStates[key] = pressed;
}

bool EventManager::isKeyPressed(Arcade::Keys key) const {
    auto it = _keyStates.find(key);
    if (it != _keyStates.end()) {
        return it->second;
    }
    return false;
}

void EventManager::setMousePosition(std::size_t x, std::size_t y) {
    _mouseX = x;
    _mouseY = y;
}

std::pair<std::size_t, std::size_t> EventManager::getMousePosition() const {
    return std::make_pair(_mouseX, _mouseY);
}

}  // namespace Arcade
