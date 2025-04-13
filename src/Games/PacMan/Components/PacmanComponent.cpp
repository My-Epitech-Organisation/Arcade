// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Pacman Component implementation
*/
#include <iostream>
#include <cmath>
#include <string>
#include "Games/PacMan/Components/PacmanComponent.hpp"

namespace Arcade {
namespace PacMan {

PacmanComponent::PacmanComponent(int lives, int score)
: _currentDirection(Direction::NONE), _nextDirection(Direction::NONE),
_lives(lives), _score(score), _gridX(0), _gridY(0),
_canMove(true), _movementTimer(0), _movementCooldown(0.2f),
_movementThreshold(0.05f), _totalGameTime(0.0f),
_visualX(0.0f), _visualY(0.0f), _targetX(0.0f), _targetY(0.0f),
_movementSpeed(24.0f), _isMoving(false),
_width(32.0f), _height(32.0f),
_animationEnabled(true), _animationTimer(0.0f),
_animationSpeed(0.1f), _animationFrame(0), _animationFrameCount(4) {
}

void PacmanComponent::updateMovementTimer(float deltaTime) {
    _movementTimer += deltaTime;
    static float stuckTimer = 0.0f;
    if (!_canMove && !_isMoving) {
        stuckTimer += deltaTime;
        if (stuckTimer > 0.5f) {
            _canMove = true;
            stuckTimer = 0.0f;
            std::cout << "Force unstuck Pacman movement" << std::endl;
        }
    } else {
        stuckTimer = 0.0f;
    }
    if (_movementTimer >= _movementThreshold) {
        _movementTimer = 0;
        _canMove = true;
    }
}

void PacmanComponent::updateVisualPosition(float deltaTime) {
    if (!_isMoving)
        return;
    float dx = _targetX - _visualX;
    float dy = _targetY - _visualY;
    float distance = std::sqrt(dx*dx + dy*dy);
    if (distance < 0.5f) {
        _visualX = _targetX;
        _visualY = _targetY;
        _isMoving = false;
        _canMove = true;
        return;
    }
    float invDistance = 1.0f / distance;
    dx *= invDistance;
    dy *= invDistance;
    _visualX += dx * _movementSpeed * deltaTime;
    _visualY += dy * _movementSpeed * deltaTime;
}

void PacmanComponent::updateAnimation(float deltaTime) {
    if (_isMoving && _animationEnabled) {
        _animationTimer += deltaTime;
        if (_animationTimer >= _animationSpeed) {
            int prevFrame = _animationFrame;
            _animationFrame = (_animationFrame + 1) % _animationFrameCount;
            _animationTimer -= _animationSpeed;
        }
    } else if (!_isMoving) {
        if (_animationFrame != 0) {
            _animationFrame = 0;
            _animationTimer = 0.0f;
        }
    }
}

std::string PacmanComponent::getDirectionalSprite() const {
    if (_animationFrame == 0 || !_isMoving) {
        if (_currentDirection != Direction::NONE) {
            switch (_currentDirection) {
                case Direction::RIGHT: return "pacman.right.2";
                case Direction::LEFT: return "pacman.left.2";
                case Direction::UP: return "pacman.up.2";
                case Direction::DOWN: return "pacman.down.2";
                default: break;
            }
        }
        return "pacman.default";
    }
    std::string dirStr;
    switch (_currentDirection) {
        case Direction::RIGHT:
            dirStr = "right";
            break;
        case Direction::LEFT:
            dirStr = "left";
            break;
        case Direction::UP:
            dirStr = "up";
            break;
        case Direction::DOWN:
            dirStr = "down";
            break;
        default:
            return "pacman.default";
    }
    return "pacman." + dirStr + "." + std::to_string(_animationFrame);
}

bool PacmanComponent::isAtTarget() const {
    float dx = _targetX - _visualX;
    float dy = _targetY - _visualY;
    return (dx*dx + dy*dy < 0.25f);
}
float PacmanComponent::getLeft() const { return _visualX; }
float PacmanComponent::getRight() const { return _visualX + _width; }
float PacmanComponent::getTop() const { return _visualY; }
float PacmanComponent::getBottom() const { return _visualY + _height; }

void PacmanComponent::setDimensions(float width, float height) {
    _width = width;
    _height = height;
}

bool PacmanComponent::collidesWith(float otherLeft, float otherTop,
float otherWidth, float otherHeight) const {
    return !(getRight() < otherLeft ||
             getLeft() > otherLeft + otherWidth ||
             getBottom() < otherTop ||
             getTop() > otherTop + otherHeight);
}

}  // namespace PacMan
}  // namespace Arcade
