// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake Component
*/

#ifndef SRC_GAMES_SNAKE_COMPONENTS_SNAKE_HPP_
#define SRC_GAMES_SNAKE_COMPONENTS_SNAKE_HPP_

#include <vector>
#include <memory>
#include <utility>
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Interface/ECS/IEntity.hpp"
#include "Games/Snake/Types.hpp"

namespace Arcade {

class SnakeComponent : public IComponent {
 public:
    SnakeComponent() : 
        direction(Direction::RIGHT), 
        shouldGrow(false),
        _headX(0),
        _headY(0),
        _canMove(true),
        _movementTimer(0.0f),
        _movementThreshold(0.2f),
        _lives(1),
        _score(0) {}
    ~SnakeComponent() = default;

    ComponentType getType() const override {
        return ComponentType::CUSTOM_BASE;
    }

    std::vector<Entity> segments;
    std::vector<std::pair<size_t, size_t>> segmentPositions;

    Direction direction;
    Direction nextDirection = Direction::RIGHT;

    bool shouldGrow;
    float speedMultiplier = 1.0f;

    size_t getHeadX() const { return _headX; }
    size_t getHeadY() const { return _headY; }
    void setHeadPosition(size_t x, size_t y) { _headX = x; _headY = y; }

    bool canMove() const { return _canMove; }
    void setCanMove(bool canMove) { _canMove = canMove; }
    void updateMovementTimer(float deltaTime);
    float getMovementTimer() const { return _movementTimer; }
    void setMovementThreshold(float threshold) { _movementThreshold = threshold; }
    float getMovementThreshold() const { return _movementThreshold; }

    int getLives() const { return _lives; }
    void setLives(int lives) { _lives = lives; }
    void decrementLives() { if (_lives > 0) _lives--; }
    bool isDead() const { return _lives <= 0; }
    int getScore() const { return _score; }
    void setScore(int score) { _score = score; }
    void addScore(int points) { _score += points; }

 private:
    size_t _headX;
    size_t _headY;
    bool _canMove;
    float _movementTimer;
    float _movementThreshold;
    int _lives;
    int _score;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_COMPONENTS_SNAKE_HPP_
