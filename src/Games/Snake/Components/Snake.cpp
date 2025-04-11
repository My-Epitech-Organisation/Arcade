// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake Component implementation
*/

#include "Games/Snake/Components/Snake.hpp"

namespace Arcade {

void SnakeComponent::updateMovementTimer(float deltaTime) {
    _movementTimer += deltaTime;
    if (_movementTimer >= _movementThreshold) {
        _movementTimer = 0;
        _canMove = true;
    }
}

}  // namespace Arcade