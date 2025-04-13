// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Pacman Component
*/

#ifndef SRC_GAMES_PACMAN_COMPONENTS_PACMANCOMPONENT_HPP_
#define SRC_GAMES_PACMAN_COMPONENTS_PACMANCOMPONENT_HPP_

#include <string>
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Models/ComponentType.hpp"

namespace Arcade {
namespace PacMan {

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

class PacmanComponent : public Arcade::IComponent {
 public:
    explicit PacmanComponent(int lives = 3, int score = 0);
    ~PacmanComponent() = default;

    ComponentType getType() const override {
        return static_cast<ComponentType>(1001);
    }

    Direction getCurrentDirection() const { return _currentDirection; }
    Direction getNextDirection() const { return _nextDirection; }
    void setCurrentDirection(Direction dir) { _currentDirection = dir; }
    void setNextDirection(Direction dir) { _nextDirection = dir; }
    int getLives() const { return _lives; }
    void setLives(int lives) { _lives = lives; }
    void decrementLives() { if (_lives > 0) _lives--; }
    bool isDead() const { return _lives <= 0; }
    int getScore() const { return _score; }
    void setScore(int score) { _score = score; }
    void addScore(int points) { _score += points; }
    void setName(const std::string& name) { _name = name; }
    const std::string& getName() const { return _name; }
    size_t getGridX() const { return _gridX; }
    size_t getGridY() const { return _gridY; }
    void setGridPosition(size_t x, size_t y) { _gridX = x;
        _gridY = y; }
    bool canMove() const { return _canMove; }
    void setCanMove(bool canMove) { _canMove = canMove; }
    void updateMovementTimer(float deltaTime);
    float getMovementTimer() const { return _movementTimer; }
    float getMovementThreshold() const { return _movementThreshold; }
    void resetMovementTimer() { _movementTimer = 0.0f; }
    void addGameTime(float deltaTime) { _totalGameTime += deltaTime; }
    void setMovementThreshold(float threshold) {
        _movementThreshold = threshold; }

    // Smooth movement additions
    float getVisualX() const { return _visualX; }
    float getVisualY() const { return _visualY; }
    void setVisualPosition(float x, float y) { _visualX = x; _visualY = y; }
    
    float getTargetX() const { return _targetX; }
    float getTargetY() const { return _targetY; }
    void setTargetPosition(float x, float y) { _targetX = x; _targetY = y; }
    
    bool isMoving() const { return _isMoving; }
    void setMoving(bool isMoving) { _isMoving = isMoving; }
    
    // Update visual position based on interpolation
    void updateVisualPosition(float deltaTime);
    
    // Check if movement animation is complete
    bool isAtTarget() const;
    
    // Get and set movement speed
    float getMovementSpeed() const { return _movementSpeed; }
    void setMovementSpeed(float speed) { _movementSpeed = speed; }

    // Helper methods for collision detection
    float getLeft() const;
    float getRight() const;
    float getTop() const; 
    float getBottom() const;
    
    // Set sprite dimensions for collision detection
    void setDimensions(float width, float height);
    
    // Check collision with another entity
    bool collidesWith(float otherLeft, float otherTop, 
                     float otherWidth, float otherHeight) const;

 private:
    std::string _name;
    Direction _currentDirection;
    Direction _nextDirection;
    int _lives;
    int _score;
    size_t _gridX;
    size_t _gridY;
    bool _canMove;
    float _movementCooldown;
    float _movementTimer = 0.0f;
    float _movementThreshold = 0.20f;
    float _totalGameTime = 0.0f;

    // Smooth movement variables
    float _visualX;      // Current visual x position
    float _visualY;      // Current visual y position
    float _targetX;      // Target x position for interpolation
    float _targetY;      // Target y position for interpolation
    float _movementSpeed; // Speed of movement interpolation
    bool _isMoving;      // Whether entity is currently moving between cells

    // Sprite dimensions for collision detection
    float _width;
    float _height;
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_COMPONENTS_PACMANCOMPONENT_HPP_
