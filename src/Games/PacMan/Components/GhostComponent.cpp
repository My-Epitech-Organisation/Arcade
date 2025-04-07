// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Ghost Component implementation
*/

#include <cstdlib>
#include "Games/PacMan/Components/GhostComponent.hpp"

namespace Arcade {
namespace PacMan {

GhostComponent::GhostComponent(GhostType type)
    : _ghostType(type), _state(GhostState::NORMAL), 
      _currentDirection(Direction::NONE), _gridX(0), _gridY(0),
      _stateTimer(0), _scaredDuration(SCARED_DURATION),
      _canMove(true), _movementTimer(0), _movementCooldown(0.3f) {
}

void GhostComponent::updateStateTimer(float deltaTime) {
    if (_state == GhostState::SCARED) {
        _stateTimer += deltaTime;
        if (_stateTimer >= _scaredDuration) {
            _state = GhostState::NORMAL;
            _stateTimer = 0;
        }
    }
}

void GhostComponent::updateMovementTimer(float deltaTime) {
    if (!_canMove) {
        _movementTimer += deltaTime;
        if (_movementTimer >= _movementCooldown) {
            _canMove = true;
            _movementTimer = 0;
        }
    }
}

void GhostComponent::chooseNextDirection() {
    // Simple randomized movement for now
    // In a full implementation, each ghost would have its own AI behavior
    int randomDir = rand() % 4;
    switch (randomDir) {
        case 0:
            _currentDirection = Direction::UP;
            break;
        case 1:
            _currentDirection = Direction::DOWN;
            break;
        case 2:
            _currentDirection = Direction::LEFT;
            break;
        case 3:
            _currentDirection = Direction::RIGHT;
            break;
    }
}

} // namespace PacMan
} // namespace Arcade