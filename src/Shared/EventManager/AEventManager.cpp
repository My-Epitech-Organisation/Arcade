// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** AEventManager abstract class implementation
*/

#include <utility>
#include <iostream>
#include <functional>
#include "EventManager/AEventManager.hpp"
#include "Models/KeysType.hpp"
#include "Models/MouseButtonType.hpp"

AEventManager::AEventManager() {
    for (int i = 0; i < 53; i++) {
        _keyEvents.push_back(std::make_pair(i, false));
    }
}

void AEventManager::subscribe(const Arcade::KeyEvent& eventType,
    const Callback callback) {
    auto found = std::pair<Arcade::EventType, Arcade::Keys>
        (eventType.getType(), eventType.getKey());
    _subscribers[found].push_back(callback);
}

void AEventManager::publish(const Arcade::KeyEvent& eventType) {
    auto found = std::pair<Arcade::EventType, Arcade::Keys>
        (eventType.getType(), eventType.getKey());
    for (auto& callback : _subscribers[found]) {
        callback();
    }
}

void AEventManager::setMouseX(std::size_t x) {
    _mouseX = x;
}

void AEventManager::setMouseY(std::size_t y) {
    _mouseY = y;
}

void AEventManager::subscribe(const Arcade::MouseEvent& eventType,
    const Callback callback) {
    auto found = std::pair<Arcade::EventType,
        Arcade::MouseButton>(eventType.getType(),
    eventType.getButton());
    _mouseSubscribers[found].push_back(callback);
}

void AEventManager::publishMouseEvent(const Arcade::MouseEvent& eventType) {
    auto found = std::pair<Arcade::EventType,
        Arcade::MouseButton>(eventType.getType(),
        eventType.getButton());
    for (auto& callback : _mouseSubscribers[found]) {
        callback();
    }
}
