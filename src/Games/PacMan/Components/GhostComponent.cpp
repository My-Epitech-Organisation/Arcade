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
#include <cmath>
#include "Games/PacMan/Components/GhostComponent.hpp"

namespace Arcade {
namespace PacMan {

GhostComponent::GhostComponent(GhostType type)
: _ghostType(type), _state(GhostState::NORMAL),
_currentDirection(Direction::NONE), _gridX(0), _gridY(0),
_stateTimer(0), _scaredDuration(SCARED_DURATION),
_canMove(true), _movementTimer(0), _movementCooldown(0.3f),
_mode(GhostMode::SCATTER), _homeCornerX(0), _homeCornerY(0),
_targetX(0), _targetY(0), _modeTimer(0.0f), _movementThreshold(0.08f),
_releaseTimer(0.0f),
_visualX(0.0f), _visualY(0.0f), _targetVisualX(0.0f), _targetVisualY(0.0f),
_movementSpeed(22.0f), _isMoving(false),
_width(32.0f), _height(32.0f) {
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
    static float stuckTimer = 0.0f;
    if (!_canMove && !_isMoving) {
        stuckTimer += deltaTime;
        if (stuckTimer > 0.5f) {
            _canMove = true;
            stuckTimer = 0.0f;
            std::cout << "Force unstuck Ghost movement" << std::endl;
        }
    } else {
        stuckTimer = 0.0f;
    }
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

void GhostComponent::updateVisualPosition(float deltaTime) {
    if (!_isMoving)
        return;
    float dx = _targetVisualX - _visualX;
    float dy = _targetVisualY - _visualY;
    float distance = std::sqrt(dx*dx + dy*dy);
    if (distance < 0.5f) {
        _visualX = _targetVisualX;
        _visualY = _targetVisualY;
        _isMoving = false;
        _canMove = true;
    }
    float invDistance = 1.0f / distance;
    dx *= invDistance;
    dy *= invDistance;
    _visualX += dx * _movementSpeed * deltaTime;
    _visualY += dy * _movementSpeed * deltaTime;
}

bool GhostComponent::isAtTarget() const {
    float dx = _targetVisualX - _visualX;
    float dy = _targetVisualY - _visualY;
    return (dx*dx + dy*dy < 0.25f);
}

// Add new helper methods for pixel-perfect collision detection
float GhostComponent::getLeft() const { return _visualX; }
float GhostComponent::getRight() const { return _visualX + _width; }
float GhostComponent::getTop() const { return _visualY; }
float GhostComponent::getBottom() const { return _visualY + _height; }

// Set sprite dimensions
void GhostComponent::setDimensions(float width, float height) {
    _width = width;
    _height = height;
}

// Check for bounding box collision with another entity
bool GhostComponent::collidesWith(float otherLeft, float otherTop,
float otherWidth, float otherHeight) const {
    return !(getRight() < otherLeft ||
            getLeft() > otherLeft + otherWidth ||
            getBottom() < otherTop ||
            getTop() > otherTop + otherHeight);
}

}  // namespace PacMan
}  // namespace Arcade
