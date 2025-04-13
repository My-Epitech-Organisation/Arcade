// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Pacman Component implementation
*/
#include <iostream>
#include <cmath>
#include "Games/PacMan/Components/PacmanComponent.hpp"

namespace Arcade {
namespace PacMan {

PacmanComponent::PacmanComponent(int lives, int score)
: _currentDirection(Direction::NONE), _nextDirection(Direction::NONE),
_lives(lives), _score(score), _gridX(0), _gridY(0),
_canMove(true), _movementTimer(0), _movementCooldown(0.2f),
_movementThreshold(0.05f), _totalGameTime(0.0f), // Much lower threshold = faster grid movement
// Initialize smooth movement variables with much higher speed
_visualX(0.0f), _visualY(0.0f), _targetX(0.0f), _targetY(0.0f),
_movementSpeed(24.0f), _isMoving(false), // Higher speed for smoother movement
_width(32.0f), _height(32.0f), // Add sprite dimensions for collision detection
_animationEnabled(true), _animationTimer(0.0f), _animationSpeed(0.1f), _animationFrame(0), _animationFrameCount(4) {
}

void PacmanComponent::updateMovementTimer(float deltaTime) {
    _movementTimer += deltaTime;
    
    // Add a safety mechanism to prevent getting stuck
    static float stuckTimer = 0.0f;
    if (!_canMove && !_isMoving) {
        stuckTimer += deltaTime;
        // If stuck for more than 0.5 seconds, force enable movement
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
        
    // Calculate the direction vector to the target
    float dx = _targetX - _visualX;
    float dy = _targetY - _visualY;
    
    // Calculate the distance to the target
    float distance = std::sqrt(dx*dx + dy*dy);
    
    // If we're close enough to the target, snap to it and enable movement again
    if (distance < 0.5f) {
        _visualX = _targetX;
        _visualY = _targetY;
        _isMoving = false;
        _canMove = true;  // Explicitly enable movement when reaching target
        return;
    }
    
    // Normalize the direction vector
    float invDistance = 1.0f / distance;
    dx *= invDistance;
    dy *= invDistance;
    
    // Update the position based on speed and delta time
    _visualX += dx * _movementSpeed * deltaTime;
    _visualY += dy * _movementSpeed * deltaTime;
}

void PacmanComponent::updateAnimation(float deltaTime) {
    // Only update animation if moving and animation is enabled
    if (_isMoving && _animationEnabled) {
        _animationTimer += deltaTime;
        
        // Update animation frame when timer exceeds animation speed
        if (_animationTimer >= _animationSpeed) {
            // Save previous frame for comparison
            int prevFrame = _animationFrame;
            
            // Update animation frame with modulo
            _animationFrame = (_animationFrame + 1) % _animationFrameCount;
            _animationTimer -= _animationSpeed;  // Reset timer but keep remainder
            
            // Debug output when frame changes (only sometimes to avoid spam)
            static int debugCounter = 0;
            if (++debugCounter % 10 == 0) {  // Log only every 10th frame change
                std::cout << "Pacman animation: frame " << prevFrame << " -> " 
                          << _animationFrame << " (dir: " 
                          << static_cast<int>(_currentDirection) << ")" << std::endl;
            }
        }
    } else if (!_isMoving) {
        // Explicitly set to frame 0 when not moving - fully open mouth
        if (_animationFrame != 0) {
            _animationFrame = 0;
            _animationTimer = 0.0f;
            
            // Debug output when stopping
            std::cout << "Pacman animation: stopped moving, reset to frame 0" << std::endl;
        }
    }
}

std::string PacmanComponent::getDirectionalSprite() const {
    // Basic closed mouth sprite (frame 0) is the same for all directions
    // But when not moving, always show frame 0 (closed mouth)
    if (_animationFrame == 0 || !_isMoving) {
        // When not moving, show direction-specific default pose
        if (_currentDirection != Direction::NONE) {
            switch (_currentDirection) {
                case Direction::RIGHT: return "pacman.right.2"; // Fully open to right
                case Direction::LEFT: return "pacman.left.2";   // Fully open to left
                case Direction::UP: return "pacman.up.2";       // Fully open up
                case Direction::DOWN: return "pacman.down.2";   // Fully open down
                default: break;
            }
        }
        return "pacman.default";  // Default is right-facing open mouth
    }
    
    // Convert direction to string for key lookup
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
    
    // Use correct frame index
    return "pacman." + dirStr + "." + std::to_string(_animationFrame);
}

bool PacmanComponent::isAtTarget() const {
    // Consider entity at target if visual position is very close to target position
    float dx = _targetX - _visualX;
    float dy = _targetY - _visualY;
    return (dx*dx + dy*dy < 0.25f);
}

// Add new helper methods for pixel-perfect collision detection
float PacmanComponent::getLeft() const { return _visualX; }
float PacmanComponent::getRight() const { return _visualX + _width; }
float PacmanComponent::getTop() const { return _visualY; }
float PacmanComponent::getBottom() const { return _visualY + _height; }

// Set sprite dimensions
void PacmanComponent::setDimensions(float width, float height) {
    _width = width;
    _height = height;
}

// Check for bounding box collision with another entity
bool PacmanComponent::collidesWith(float otherLeft, float otherTop, 
                                  float otherWidth, float otherHeight) const {
    return !(getRight() < otherLeft || 
             getLeft() > otherLeft + otherWidth ||
             getBottom() < otherTop || 
             getTop() > otherTop + otherHeight);
}

}  // namespace PacMan
}  // namespace Arcade
