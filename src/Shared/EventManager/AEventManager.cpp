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

bool AEventManager::getEventState(int key) {
    return _keyEvents[key].second;
}

void AEventManager::setEventState(int key, bool state) {
    _keyEvents[key].second = state;
}

std::size_t AEventManager::getMouseX() const {
    return _mouseX;
}

std::size_t AEventManager::getMouseY() const {
    return _mouseY;
}

void AEventManager::setMouseX(std::size_t x) {
    _mouseX = x;
}

void AEventManager::setMouseY(std::size_t y) {
    _mouseY = y;
}
