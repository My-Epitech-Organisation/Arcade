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
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_COMPONENTS_PACMANCOMPONENT_HPP_
