// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** EventManager implementation
*/

#include <iostream>
#include <utility>
#include "EventManager/EventManager.hpp"
#include "Shared/Models/KeysType.hpp"
#include "Shared/Interface/Display/IDisplayModule.hpp"
#include "Shared/Models/EventType.hpp"
#include "EventManager/KeyEvent/RawInputState.hpp"
#include "EventManager/KeyEvent/KeyEvent.hpp"
#include "EventManager/KeyEvent/MouseEvent.hpp"

namespace Arcade {

EventManager::EventManager()
: _mouseX(0), _mouseY(0), _displayModule(nullptr) {
    _subscribers.clear();
    _mouseSubscribers.clear();
    _keyStates.clear();
    _mouseButtonStates.clear();
}

void EventManager::updateKeyState(const RawInputState& state) {
    for (const auto& [key, isPressed] : state.keyStates) {
        bool wasPressed = _keyStates[key];
        if (isPressed && !wasPressed) {
            publish(KeyEvent(key, EventType::KEY_PRESSED));
        } else if (!isPressed && wasPressed) {
            publish(KeyEvent(key, EventType::KEY_RELEASED));
        }
        _keyStates[key] = isPressed;
    }
}

void EventManager::updateMouseButtonState(const RawInputState& state) {
    for (const auto& [button, isPressed] : state.mouseButtons) {
        bool wasPressed = _mouseButtonStates[button];
        if (isPressed && !wasPressed) {
            publish(MouseEvent(button,
                EventType::MOUSE_BUTTON_PRESSED, state.mouseX, state.mouseY));
        } else if (!isPressed && wasPressed) {
            publish(MouseEvent(button,
                EventType::MOUSE_BUTTON_RELEASED, state.mouseX, state.mouseY));
        }
        _mouseButtonStates[button] = isPressed;
    }
}

void EventManager::updateMousePosition(const RawInputState& state) {
    if (state.mouseX != _mouseX || state.mouseY != _mouseY) {
        publish(MouseEvent(MouseButton::NONE,
            EventType::MOUSE_MOVED, state.mouseX, state.mouseY));
        _mouseX = state.mouseX;
        _mouseY = state.mouseY;
    }
}

void EventManager::updateInputState(const RawInputState& state) {
    updateKeyState(state);
    updateMouseButtonState(state);
    updateMousePosition(state);
}

void EventManager::setKeyPressed(Keys key, bool pressed) {
    _keyStates[key] = pressed;
}

bool EventManager::isKeyPressed(Keys key) const {
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

void EventManager::unsubscribeAll() {
    _subscribers.clear();
    _mouseSubscribers.clear();
}

}  // namespace Arcade
