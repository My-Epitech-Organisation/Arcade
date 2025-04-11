// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Movement system implementation for Snake game
*/

#include "Games/Snake/System/MovementSystem.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Games/Snake/Components/Board.hpp"
#include "Games/Snake/Components/Food.hpp"
#include "Games/Snake/Components/GameStats.hpp"
#include "Games/Snake/Snake.hpp"
#include "Shared/Interface/Core/Clock.hpp"

namespace Arcade {

MovementSystem::MovementSystem(std::shared_ptr<IComponentManager> componentManager,
                             std::shared_ptr<IEntityManager> entityManager,
                             SnakeGame* gameRef)
    : _componentManager(componentManager),
      _entityManager(entityManager),
      _gameRef(gameRef),
      _timeSinceLastMove(0.0f) {
}

void MovementSystem::update() {
    _timeSinceLastMove += Clock::getDeltaTime();

    if (_timeSinceLastMove >= _moveInterval) {
        moveSnake();
        _timeSinceLastMove = 0.0f;
    }
}

void MovementSystem::moveSnake() {
    auto snakeEntities = _entityManager->getEntitiesWithComponent<Snake>();
    if (snakeEntities.empty()) return;

    Entity snakeEntity = snakeEntities[0];
    auto snake = _componentManager->getComponent<Snake>(snakeEntity);

    if (snake->segments.empty()) return;

    // Calculate new head position based on current direction
    int newHeadX = snake->segments[0].first;
    int newHeadY = snake->segments[0].second;

    switch (snake->direction) {
        case Snake::UP:
            newHeadY--;
            break;
        case Snake::DOWN:
            newHeadY++;
            break;
        case Snake::LEFT:
            newHeadX--;
            break;
        case Snake::RIGHT:
            newHeadX++;
            break;
    }

    // Check if the new head position is valid
    auto boardEntities = _entityManager->getEntitiesWithComponent<Board>();
    if (boardEntities.empty()) return;

    Entity boardEntity = boardEntities[0];
    auto board = _componentManager->getComponent<Board>(boardEntity);

    // Handle board boundaries (wrap around if cyclical is enabled)
    if (board->cyclical) {
        if (newHeadX < 0) newHeadX = board->width - 1;
        if (newHeadX >= board->width) newHeadX = 0;
        if (newHeadY < 0) newHeadY = board->height - 1;
        if (newHeadY >= board->height) newHeadY = 0;
    }

    // Check for collisions
    bool collision = checkCollision(newHeadX, newHeadY);

    // If collision with wall (and not cyclical), end the game
    if (!board->cyclical &&
        (newHeadX < 0 || newHeadX >= board->width || newHeadY < 0 || newHeadY >= board->height)) {
        updateGameState(true);
        return;
    }

    // Check for food collision
    handleFoodCollision(newHeadX, newHeadY);

    // Move snake: add new head
    snake->segments.insert(snake->segments.begin(), std::make_pair(newHeadX, newHeadY));

    // Remove tail if not growing
    if (!snake->growNextUpdate) {
        snake->segments.pop_back();
    } else {
        // Reset growth flag
        snake->growNextUpdate = false;
    }

    // Update snake visuals
    _gameRef->updateSnakeVisuals();

    // Update game state if collision
    updateGameState(collision);
}

bool MovementSystem::checkCollision(int x, int y) {
    auto snakeEntities = _entityManager->getEntitiesWithComponent<Snake>();
    if (snakeEntities.empty()) return false;

    Entity snakeEntity = snakeEntities[0];
    auto snake = _componentManager->getComponent<Snake>(snakeEntity);

    // Check collision with snake's body (not including tail, which will move)
    for (size_t i = 0; i < snake->segments.size() - (snake->growNextUpdate ? 0 : 1); ++i) {
        if (snake->segments[i].first == x && snake->segments[i].second == y) {
            return true;
        }
    }

    return false;
}

void MovementSystem::handleFoodCollision(int x, int y) {
    auto foodEntities = _entityManager->getEntitiesWithComponent<Food>();

    for (auto foodEntity : foodEntities) {
        auto food = _componentManager->getComponent<Food>(foodEntity);

        if (food->x == x && food->y == y) {
            // Snake eats food
            auto snakeEntities = _entityManager->getEntitiesWithComponent<Snake>();
            if (!snakeEntities.empty()) {
                auto snake = _componentManager->getComponent<Snake>(snakeEntities[0]);
                snake->growNextUpdate = true;
            }

            // Update score
            auto statsEntities = _entityManager->getEntitiesWithComponent<GameStats>();
            if (!statsEntities.empty()) {
                auto stats = _componentManager->getComponent<GameStats>(statsEntities[0]);
                stats->score += (food->type == Food::APPLE) ? 10 : 50;
            }

            // Remove food
            _entityManager->destroyEntity(foodEntity);

            // Spawn new food
            _gameRef->spawnFood();
            break;
        }
    }
}

void MovementSystem::updateGameState(bool collision) {
    if (collision) {
        // Game over if the snake collides with itself
        auto snakeEntities = _entityManager->getEntitiesWithComponent<Snake>();
        if (!snakeEntities.empty()) {
            _entityManager->destroyEntity(snakeEntities[0]);
        }
    }
}

}  // namespace Arcade
