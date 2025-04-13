// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** EventManager implementation
*/

#include <iostream>
#include <utility>
#include <string>
#include <vector>
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
    try {
        for (const auto& [key, isPressed] : state.keyStates) {
            bool wasPressed = _keyStates[key];
            if (isPressed && !wasPressed) {
                publish(KeyEvent(key, EventType::KEY_PRESSED));
            } else if (!isPressed && wasPressed) {
                publish(KeyEvent(key, EventType::KEY_RELEASED));
            }
            _keyStates[key] = isPressed;
        }
    } catch (const ArcadeException& e) {
        throw InputException("Error processing key state updates: " +
            std::string(e.what()));
    } catch (const std::exception& e) {
        throw InputException("Unexpected error during key state update: " +
            std::string(e.what()));
    }
}

void EventManager::updateMouseButtonState(const RawInputState& state) {
    try {
        for (const auto& [button, isPressed] : state.mouseButtons) {
            bool wasPressed = _mouseButtonStates[button];
            if (isPressed && !wasPressed) {
                publish(MouseEvent(button,
                    EventType::MOUSE_BUTTON_PRESSED,
                    state.mouseX, state.mouseY));
            } else if (!isPressed && wasPressed) {
                publish(MouseEvent(button,
                    EventType::MOUSE_BUTTON_RELEASED,
                    state.mouseX, state.mouseY));
            }
            _mouseButtonStates[button] = isPressed;
        }
    } catch (const ArcadeException& e) {
        throw InputException("Error processing mouse button updates: " +
            std::string(e.what()));
    } catch (const std::exception& e) {
        throw InputException("Unexpected error during mouse button update: " +
            std::string(e.what()));
    }
}

void EventManager::updateMousePosition(const RawInputState& state) {
    try {
        if (state.mouseX != _mouseX || state.mouseY != _mouseY) {
            publish(MouseEvent(MouseButton::NONE,
                EventType::MOUSE_MOVED, state.mouseX, state.mouseY));
            _mouseX = state.mouseX;
            _mouseY = state.mouseY;
        }
    } catch (const ArcadeException& e) {
        throw InputException("Error processing mouse position updates: " +
            std::string(e.what()));
    } catch (const std::exception& e) {
        throw InputException("Unexpected error during mouse position update: " +
            std::string(e.what()));
    }
}

void EventManager::updateInputState(const RawInputState& state) {
    try {
        updateKeyState(state);
        updateMouseButtonState(state);
        updateMousePosition(state);
    } catch (const InputException& e) {
        std::cerr << "Input processing error: " << e.what() << std::endl;
    } catch (const ArcadeException& e) {
        std::cerr <<
        "Arcade error during input processing: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr <<
        "Unexpected error during input processing: " << e.what() << std::endl;
    }
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

void EventManager::unsubscribe(const IEvent& eventType, Callback callback) {
    (void)eventType;
    (void)callback;
    try {
        _subscribers.clear();
        _mouseSubscribers.clear();
    } catch (const std::exception& e) {
        throw InputException("Error unsubscribing events: " +
            std::string(e.what()));
    }
}

void EventManager::resetKeys() {
    try {
        for (auto& [key, _] : _keyStates) {
            _keyStates[key] = false;
        }
        for (auto& [button, _] : _mouseButtonStates) {
            _mouseButtonStates[button] = false;
        }
    } catch (const std::exception& e) {
        throw InputException("Error resetting keys: " +
            std::string(e.what()));
    }
}

void EventManager::setKeyState(Keys key) {
    try {
        _keyStates[key] = true;
    } catch (const std::exception& e) {
        throw InputException("Error setting key state: " +
            std::string(e.what()));
    }
}

void EventManager::unsubscribeAll() {
    try {
        _subscribers.clear();
        _mouseSubscribers.clear();
    } catch (const std::exception& e) {
        throw InputException("Error unsubscribing events: " +
            std::string(e.what()));
    }
}

void EventManager::unsubscribeAll(const IEvent& eventType) {
    try {
        // Check if the event is a MouseEvent
        if (const MouseEvent* mouseEvent = dynamic_cast<const MouseEvent*>(&eventType)) {
            // For MouseEvents - remove subscriptions matching the event type and mouse button
            auto mouseEventId = std::pair<EventType, MouseButton>(
                mouseEvent->getType(), mouseEvent->getMouseButton());
            _mouseSubscribers.erase(mouseEventId);
        } 
        // Check if the event is a KeyEvent
        else if (const KeyEvent* keyEvent = dynamic_cast<const KeyEvent*>(&eventType)) {
            // For KeyEvents - remove subscriptions matching the event type and key
            auto keyEventId = std::pair<EventType, Keys>(
                keyEvent->getType(), keyEvent->getKey());
            _subscribers.erase(keyEventId);
        } else {
            // For other event types, log the issue but don't fail
            std::cerr << "Warning: Cannot unsubscribe from unknown event type" << std::endl;
        }
    } catch (const std::exception& e) {
        throw InputException("Error unsubscribing specific event type: " +
            std::string(e.what()));
    }
}

void EventManager::publish(const IEvent& eventType) {
    try {
        std::vector<Callback> callbacksToInvoke;
        if (dynamic_cast<const MouseEvent*>(&eventType) != nullptr) {
            auto mouseEvent = dynamic_cast<const MouseEvent*>(&eventType);
            auto mouseFound = std::pair<EventType, MouseButton>
                (mouseEvent->getType(), mouseEvent->getMouseButton());
            auto it = _mouseSubscribers.find(mouseFound);
            if (it != _mouseSubscribers.end()) {
                callbacksToInvoke = it->second;
            }
            for (const auto& callback : callbacksToInvoke) {
                if (callback) {
                    callback(eventType);
                }
            }
        } else if (dynamic_cast<const KeyEvent*>(&eventType) != nullptr) {
            auto keyEvent = dynamic_cast<const KeyEvent*>(&eventType);
            auto keyFound = std::pair<EventType, Keys>
                (eventType.getType(), eventType.getKey());
            auto it = _subscribers.find(keyFound);
            if (it != _subscribers.end()) {
                callbacksToInvoke = it->second;
            }
            for (const auto& callback : callbacksToInvoke) {
                if (callback) {
                    callback(eventType);
                }
            }
        } else {
            throw InputException("Unknown event type for publishing");
        }
    } catch (const ArcadeException& e) {
        throw InputException("Error publishing event: " +
            std::string(e.what()));
    } catch (const std::exception& e) {
        throw InputException("Unexpected error while publishing event: " +
            std::string(e.what()));
    }
}

}  // namespace Arcade
