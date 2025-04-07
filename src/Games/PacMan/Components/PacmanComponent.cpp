// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Pacman Component implementation
*/

#include "Games/PacMan/Components/PacmanComponent.hpp"

namespace Arcade {
namespace PacMan {

PacmanComponent::PacmanComponent(int lives, int score)
    : _currentDirection(Direction::NONE), _nextDirection(Direction::NONE),
      _lives(lives), _score(score), _gridX(0), _gridY(0),
      _canMove(true), _movementTimer(0), _movementCooldown(0.2f) {
}

void PacmanComponent::updateMovementTimer(float deltaTime) {
    if (!_canMove) {
        _movementTimer += deltaTime;
        if (_movementTimer >= _movementCooldown) {
            _canMove = true;
            _movementTimer = 0;
        }
    }
}

} // namespace PacMan
} // namespace Arcade