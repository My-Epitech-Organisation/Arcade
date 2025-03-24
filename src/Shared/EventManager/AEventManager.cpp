// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** AEventManager abstract class implementation
*/

#include "EventManager/AEventManager.hpp"
#include <utility>

AEventManager::AEventManager() {
    for (int i = 0; i < 53; i++) {
        _keyEvents.push_back(std::make_pair(i, false));
    }
}

void AEventManager::subscribe(const Arcade::IEvent& eventType,
    const Callback callback) {
    _subscribers[eventType.getType()].push_back(callback);
}

void AEventManager::publish(const Arcade::IEvent& eventType) {
    for (auto& callback : _subscribers[eventType.getType()]) {
        callback();
    }
}

void AEventManager::setMouseX(std::size_t x) {
    _mouseX = x;
}

void AEventManager::setMouseY(std::size_t y) {
    _mouseY = y;
}
