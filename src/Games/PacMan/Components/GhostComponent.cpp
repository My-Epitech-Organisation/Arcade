// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Ghost Component implementation
*/

#include <cstdlib>
#include <vector>
#include <memory>
#include <iostream>
#include "Games/PacMan/Components/GhostComponent.hpp"

namespace Arcade {
namespace PacMan {

GhostComponent::GhostComponent(GhostType type)
: _ghostType(type), _state(GhostState::NORMAL),
_currentDirection(Direction::NONE), _gridX(0), _gridY(0),
_stateTimer(0), _scaredDuration(SCARED_DURATION),
_canMove(true), _movementTimer(0), _movementCooldown(0.3f),
_mode(GhostMode::SCATTER), _homeCornerX(0), _homeCornerY(0),
_targetX(0), _targetY(0), _modeTimer(0.0f) {
    switch (type) {
        case GhostType::RED:
            _homeCornerX = 25;
            _homeCornerY = 0;
            break;
        case GhostType::PINK:
            _homeCornerX = 2;
            _homeCornerY = 0;
            break;
        case GhostType::BLUE:
            _homeCornerX = 27;
            _homeCornerY = 30;
            break;
        case GhostType::ORANGE:
            _homeCornerX = 0;
            _homeCornerY = 30;
            break;
    }
}

Direction GhostComponent::getOppositeDirection() const {
    switch (_currentDirection) {
        case Direction::UP:
            return Direction::DOWN;
        case Direction::DOWN:
            return Direction::UP;
        case Direction::LEFT:
            return Direction::RIGHT;
        case Direction::RIGHT:
            return Direction::LEFT;
        default:
            return Direction::NONE;
    }
}

void GhostComponent::updateModeTimer(float deltaTime) {
    if (_state == GhostState::SCARED) return;

    _modeTimer += deltaTime;
    if ((_mode == GhostMode::CHASE && _modeTimer >= CHASE_DURATION) ||
        (_mode == GhostMode::SCATTER && _modeTimer >= SCATTER_DURATION)) {
        _mode = (_mode == GhostMode::CHASE) ? GhostMode::SCATTER
            : GhostMode::CHASE;
        _modeTimer = 0;

        _currentDirection = getOppositeDirection();
    }
}

void GhostComponent::updateReleaseTimer(float deltaTime) {
    _releaseTimer += deltaTime;
}

bool GhostComponent::isGhostAtPosition(size_t x, size_t y,
const std::vector<std::shared_ptr<GhostComponent>>& ghosts,
GhostComponent* self) {
    for (const auto& ghost : ghosts) {
        if (ghost.get() != self && ghost->getGridX() == x &&
        ghost->getGridY() == y)
            return true;
    }
    return false;
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
    _movementTimer += deltaTime;
    if (_movementTimer >= _movementThreshold) {
        _canMove = true;
        _movementTimer = 0;
    }
}

void GhostComponent::chooseNextDirection() {
    unsigned int seed = static_cast<unsigned int>(time(NULL));
    int randomDir = rand_r(&seed) % 4;
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

}  // namespace PacMan
}  // namespace Arcade
