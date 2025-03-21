// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Movement System
*/

#include "System/MovementSystem.hpp"

void MovementSystem::update() {
    for (auto& [entity, position] : positionManager->getAll()) {
        auto velocity = velocityManager->getComponent(entity);
        if (velocity) {
            position.x += velocity->dx;
            position.y += velocity->dy;
        }
    }
}
