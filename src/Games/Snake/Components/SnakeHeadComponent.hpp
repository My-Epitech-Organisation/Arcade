// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake Head Component header
*/

#ifndef SRC_GAMES_SNAKE_COMPONENTS_SNAKEHEADCOMPONENT_HPP_
#define SRC_GAMES_SNAKE_COMPONENTS_SNAKEHEADCOMPONENT_HPP_

#include <string>
#include <vector>
#include <utility>
#include <chrono>
#include "Shared/Interface/ECS/IComponent.hpp"

namespace Arcade {
namespace Snake {

/**
 * @enum Direction
 * @brief Movement directions for the snake
 */
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

/**
 * @class SnakeHeadComponent
 * @brief Component representing the snake's head
 */
class SnakeHeadComponent : public Arcade::IComponent {
 public:
    /**
     * @brief Constructor
     * @param score Initial score
     * @param lives Initial lives
     */
    explicit SnakeHeadComponent(int score = 0, int lives = 1);

    /**
     * @brief Destructor
     */
    ~SnakeHeadComponent() = default;

    /**
     * @brief Gets component type
     * @return Component type
     */
    ComponentType getType() const override {
        return static_cast<ComponentType>(1001);  // Custom component type
    }

    /**
     * @brief Gets current direction
     * @return Current direction
     */
    Direction getCurrentDirection() const { return _currentDirection; }

    /**
     * @brief Gets next direction
     * @return Next direction
     */
    Direction getNextDirection() const { return _nextDirection; }

    /**
     * @brief Sets current direction
     * @param dir Direction
     */
    void setCurrentDirection(Direction dir) { _currentDirection = dir; }

    /**
     * @brief Sets next direction
     * @param dir Direction
     */
    void setNextDirection(Direction dir) { _nextDirection = dir; }

    /**
     * @brief Gets remaining lives
     * @return Lives
     */
    int getLives() const { return _lives; }

    /**
     * @brief Sets lives
     * @param lives Lives
     */
    void setLives(int lives) { _lives = lives; }

    /**
     * @brief Decrements lives
     */
    void decrementLives() { if (_lives > 0) _lives--; }

    /**
     * @brief Checks if snake is dead
     * @return True if dead
     */
    bool isDead() const { return _lives <= 0; }

    /**
     * @brief Gets current score
     * @return Score
     */
    int getScore() const { return _score; }

    /**
     * @brief Sets score
     * @param score Score
     */
    void setScore(int score) { _score = score; }

    /**
     * @brief Add points to score
     * @param points Points to add
     */
    void addScore(int points) { _score += points; }

    /**
     * @brief Sets name
     * @param name Name
     */
    void setName(const std::string& name) { _name = name; }

    /**
     * @brief Gets name
     * @return Name
     */
    const std::string& getName() const { return _name; }

    /**
     * @brief Gets X position in grid
     * @return X position
     */
    size_t getGridX() const { return _gridX; }

    /**
     * @brief Gets Y position in grid
     * @return Y position
     */
    size_t getGridY() const { return _gridY; }

    /**
     * @brief Sets grid position
     * @param x X position
     * @param y Y position
     */
    void setGridPosition(size_t x, size_t y) { _gridX = x; _gridY = y; }

    /**
     * @brief Checks if snake can move
     * @return True if can move
     */
    bool canMove() const { return _canMove; }

    /**
     * @brief Sets can move flag
     * @param canMove Can move flag
     */
    void setCanMove(bool canMove) { _canMove = canMove; }

    /**
     * @brief Updates movement timer
     * @param deltaTime Time since last update
     */
    void updateMovementTimer(float deltaTime);

    /**
     * @brief Gets movement timer
     * @return Movement timer
     */
    float getMovementTimer() const { return _movementTimer; }

    /**
     * @brief Gets movement threshold
     * @return Movement threshold
     */
    float getMovementThreshold() const { return _movementThreshold; }

    /**
     * @brief Resets movement timer
     */
    void resetMovementTimer() { _movementTimer = 0.0f; }

    /**
     * @brief Adds game time
     * @param deltaTime Time to add
     */
    void addGameTime(float deltaTime) { _totalGameTime += deltaTime; }

    /**
     * @brief Sets movement threshold
     * @param threshold Threshold
     */
    void setMovementThreshold(float threshold) { _movementThreshold = threshold; }

    /**
     * @brief Adds a segment position to the snake
     * @param x X position
     * @param y Y position
     */
    void addSegment(size_t x, size_t y) { _segments.push_back({x, y}); }

    /**
     * @brief Gets segments
     * @return Segments
     */
    const std::vector<std::pair<size_t, size_t>>& getSegments() const { return _segments; }

    /**
     * @brief Sets segments
     * @param segments Segments
     */
    void setSegments(const std::vector<std::pair<size_t, size_t>>& segments) { _segments = segments; }

    /**
     * @brief Clear all segments from the snake
     */
    void clearSegments() { _segments.clear(); }

    /**
     * @brief Gets segments count
     * @return Segments count
     */
    size_t getSegmentsCount() const { return _segments.size(); }

    /**
     * @brief Updates segments positions based on head movement
     */
    void updateSegments();

 private:
    std::string _name;
    Direction _currentDirection;
    Direction _nextDirection;
    int _lives;
    int _score;
    size_t _gridX;
    size_t _gridY;
    bool _canMove;
    float _movementTimer;
    float _movementThreshold;
    float _totalGameTime;
    std::vector<std::pair<size_t, size_t>> _segments;  // Snake body segments positions
};

}  // namespace Snake
}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_COMPONENTS_SNAKEHEADCOMPONENT_HPP_