// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake Head Component implementation
*/
#include <iostream>
#include "Games/Snake/Components/SnakeHeadComponent.hpp"

namespace Arcade {
namespace Snake {

SnakeHeadComponent::SnakeHeadComponent(int score, int lives)
: _currentDirection(Direction::NONE), _nextDirection(Direction::NONE),
_lives(lives), _score(score), _gridX(0), _gridY(0),
_canMove(true), _movementTimer(0), _movementThreshold(0.2f), _totalGameTime(0) {
}

void SnakeHeadComponent::updateMovementTimer(float deltaTime) {
    _movementTimer += deltaTime;
    if (_movementTimer >= _movementThreshold) {
        _movementTimer = 0;
        _canMove = true;
    }
}

}  // namespace Snake
}  // namespace Arcade