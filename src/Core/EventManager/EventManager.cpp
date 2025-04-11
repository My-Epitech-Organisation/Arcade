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

void EventManager::unsubscribeAll() {
    try {
        _subscribers.clear();
        _mouseSubscribers.clear();
    } catch (const std::exception& e) {
        throw InputException("Error unsubscribing events: " +
            std::string(e.what()));
    }
}

// Added to handle publishing errors with proper exceptions
// ...existing code...

void EventManager::publish(const IEvent& eventType) {
    try {
        std::vector<Callback> callbacksToInvoke;
        if (dynamic_cast<const MouseEvent*>(&eventType) != nullptr) {
            auto mouseEvent = dynamic_cast<const MouseEvent*>(&eventType);
            auto mouseFound = std::pair<EventType, MouseButton>
                (mouseEvent->getType(), mouseEvent->getButton());
            // Find and copy callbacks safely
            auto it = _mouseSubscribers.find(mouseFound);
            if (it != _mouseSubscribers.end()) {
                callbacksToInvoke = it->second;
            }
            // Execute the callbacks from our safe copy
            for (const auto& callback : callbacksToInvoke) {
                if (callback) {  // Verify callback is valid
                    callback();
                }
            }
        } else if (dynamic_cast<const KeyEvent*>(&eventType) != nullptr) {
            auto keyEvent = dynamic_cast<const KeyEvent*>(&eventType);
            auto keyFound = std::pair<EventType, Keys>
                (eventType.getType(), eventType.getKey());
            // Find and copy callbacks safely
            auto it = _subscribers.find(keyFound);
            if (it != _subscribers.end()) {
                callbacksToInvoke = it->second;
            }
            // Execute the callbacks from our safe copy
            for (const auto& callback : callbacksToInvoke) {
                if (callback) {  // Verify callback is valid
                    callback();
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
