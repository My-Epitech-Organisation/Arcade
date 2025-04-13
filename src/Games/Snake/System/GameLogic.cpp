// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game Logic System implementation for Snake
*/

#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <chrono>
#include "Games/Snake/System/GameLogic.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace Snake {

GameLogic::GameLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
std::shared_ptr<Arcade::IEntityManager> entityManager,
const std::map<std::string, DrawableComponent>& assets)
: _componentManager(componentManager), _entityManager(entityManager),
_assets(assets) {
    _lastUpdateTime = std::chrono::high_resolution_clock::now();
}

void GameLogic::update() {
    updateDeltaTime();
    auto [snake, grid] = getSnakeAndGridComponents();
    if (!snake || !grid)
        return;

    // Add game time to snake
    snake->addGameTime(getDeltaTime());
    snake->updateMovementTimer(getDeltaTime());

    // Try to change snake direction based on next direction
    tryChangeDirection(snake, grid);

    // Create bonus food occasionally
    _bonusFoodTimer += getDeltaTime();
    if (!_bonusFoodActive && _bonusFoodTimer > 15.0f) {
        createFood(grid, FoodType::BONUS);
        _bonusFoodActive = true;
        _bonusFoodTimer = 0.0f;
    }

    // Update bonus food timeouts
    auto foodEntities = findFoodEntities();
    for (auto entity : foodEntities) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
        if (foodComp && foodComp->getFoodType() == FoodType::BONUS) {
            if (foodComp->updateTimeRemaining(getDeltaTime())) {
                // Remove expired bonus food
                grid->setCellType(foodComp->getGridX(), foodComp->getGridY(),
                    CellType::EMPTY);
                _entityManager->destroyEntity(entity);
                _bonusFoodActive = false;
            }
        }
    }

    // Periodically increase game difficulty
    _difficultyTimer += getDeltaTime();
    if (_difficultyTimer >= 10.0f) {
        increaseDifficulty(snake);
        _difficultyTimer = 0.0f;
    }

    // Move the snake if it's time and check collisions
    if (snake->canMove()) {
        if (moveSnake(snake, grid)) {
            // If movement was successful, update drawables
            updateDrawablePositions(snake, grid);
            snake->setCanMove(false);
            snake->resetMovementTimer();

            // Check if snake ate food
            checkFoodCollision(snake, grid);

            // Check for collisions with self or walls
            if (checkSnakeCollision(snake, grid)) {
                grid->setGameOver(true);
            }
        }
    }

    // Make sure there's always food on the grid
    bool foundFood = false;
    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            if (grid->getCellType(x, y) == CellType::FOOD) {
                foundFood = true;
                break;
            }
        }
        if (foundFood)
            break;
    }

    if (!foundFood && !grid->isGameOver()) {
        createFood(grid, FoodType::REGULAR);
    }
}

void GameLogic::updateDeltaTime() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    _currentDeltaTime = std::chrono::duration<float>(
        currentTime - _lastUpdateTime).count();
    _lastUpdateTime = currentTime;
}

float GameLogic::getDeltaTime() {
    return _currentDeltaTime;
}

Arcade::Entity GameLogic::findSnakeHeadEntity() {
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto snakeComp = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1001));
        if (snakeComp)
            return entity;
    }
    return 0;
}

Arcade::Entity GameLogic::findGridEntity() {
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto gridComp = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1000));
        if (gridComp)
            return entity;
    }
    return 0;
}

std::pair<std::shared_ptr<SnakeHeadComponent>, std::shared_ptr<GridComponent>>
GameLogic::getSnakeAndGridComponents() {
    Arcade::Entity snakeEntity = findSnakeHeadEntity();
    Arcade::Entity gridEntity = findGridEntity();

    auto snakeComp = std::dynamic_pointer_cast<SnakeHeadComponent>(
        _componentManager->getComponentByType(snakeEntity,
            static_cast<ComponentType>(1001)));
    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));

    return std::make_pair(snakeComp, gridComp);
}

std::shared_ptr<DrawableComponent>
GameLogic::getDrawableAsset(const std::string& key) const {
    auto it = _assets.find(key);
    if (it != _assets.end()) {
        auto component = std::make_shared<DrawableComponent>(it->second);
        return component;
    }
    return nullptr;
}

void GameLogic::tryChangeDirection(std::shared_ptr<SnakeHeadComponent> snake,
                                  std::shared_ptr<GridComponent> grid) {
    Direction nextDir = snake->getNextDirection();
    Direction currentDir = snake->getCurrentDirection();

    // If no next direction set, do nothing
    if (nextDir == Direction::NONE)
        return;

    // Check that the next direction is valid (not reverse of current)
    if ((currentDir == Direction::UP && nextDir == Direction::DOWN) ||
        (currentDir == Direction::DOWN && nextDir == Direction::UP) ||
        (currentDir == Direction::LEFT && nextDir == Direction::RIGHT) ||
        (currentDir == Direction::RIGHT && nextDir == Direction::LEFT)) {
        return;
    }

    // Accept the direction change
    snake->setCurrentDirection(nextDir);
    snake->setNextDirection(Direction::NONE);
}

bool GameLogic::moveSnake(std::shared_ptr<SnakeHeadComponent> snake,
                         std::shared_ptr<GridComponent> grid) {
    Direction currentDir = snake->getCurrentDirection();
    if (currentDir == Direction::NONE)
        return false;

    size_t currentX = snake->getGridX();
    size_t currentY = snake->getGridY();
    size_t newX = currentX;
    size_t newY = currentY;

    // Calculate new position based on direction
    switch (currentDir) {
        case Direction::UP:
            newY = (currentY > 0) ? currentY - 1 : (grid->isCyclical() ?
                grid->getHeight() - 1 : currentY);
            break;
        case Direction::DOWN:
            newY = (currentY < grid->getHeight() - 1) ? currentY + 1 :
                (grid->isCyclical() ? 0 : currentY);
            break;
        case Direction::LEFT:
            newX = (currentX > 0) ? currentX - 1 : (grid->isCyclical() ?
                grid->getWidth() - 1 : currentX);
            break;
        case Direction::RIGHT:
            newX = (currentX < grid->getWidth() - 1) ? currentX + 1 :
                (grid->isCyclical() ? 0 : currentX);
            break;
        case Direction::NONE:
            return false;
    }

    // If we hit a wall and grid is not cyclical, it's game over
    if ((newX == currentX && newY == currentY) && !grid->isCyclical()) {
        grid->setGameOver(true);
        return false;
    }

    // Update the snake body segments with the new head position
    updateSnakeBody(snake, grid, newX, newY);

    return true;
}

void GameLogic::updateSnakeBody(std::shared_ptr<SnakeHeadComponent> snake,
                               std::shared_ptr<GridComponent> grid,
                               size_t newHeadX, size_t newHeadY) {
    // Get current segments
    auto segments = snake->getSegments();

    // Clear current body cells
    for (const auto& segment : segments) {
        grid->setCellType(segment.first, segment.second, CellType::EMPTY);
    }

    // Clear the current head cell
    grid->setCellType(snake->getGridX(), snake->getGridY(), CellType::EMPTY);

    // Move each segment to the position of the previous one
    std::vector<std::pair<size_t, size_t>> newSegments;
    if (!segments.empty()) {
        // First segment takes the position of the head
        newSegments.push_back(std::make_pair(snake->getGridX(),
            snake->getGridY()));

        // Rest of the segments follow
        for (size_t i = 0; i < segments.size() - 1; ++i) {
            newSegments.push_back(segments[i]);
        }
    }

    // Update snake head position
    snake->setGridPosition(newHeadX, newHeadY);
    grid->setCellType(newHeadX, newHeadY, CellType::SNAKE_HEAD);

    // Update segments in snake and grid
    snake->clearSegments();
    for (const auto& segment : newSegments) {
        snake->addSegment(segment.first, segment.second);
        grid->setCellType(segment.first, segment.second, CellType::SNAKE_BODY);
    }
}

void GameLogic::updateDrawablePositions(std::shared_ptr<SnakeHeadComponent> snake,
                                      std::shared_ptr<GridComponent> grid) {
    Arcade::Entity snakeEntity = findSnakeHeadEntity();
    Arcade::Entity gridEntity = findGridEntity();

    // Get positions
    auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));
    float startX = gridPosComp ? gridPosComp->x : 0;
    float startY = gridPosComp ? gridPosComp->y : 0;
    float cellSize = grid->getCellSize();

    // Update head position
    auto snakePosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(snakeEntity,
            ComponentType::POSITION));
    if (snakePosComp) {
        snakePosComp->x = startX + (snake->getGridX() * cellSize);
        snakePosComp->y = startY + (snake->getGridY() * cellSize);
    }

    // Update drawable for the head
    auto snakeDrawable = std::dynamic_pointer_cast<DrawableComponent>(
        _componentManager->getComponentByType(snakeEntity,
            ComponentType::DRAWABLE));
    if (snakeDrawable) {
        snakeDrawable->posX = startX + (snake->getGridX() * cellSize);
        snakeDrawable->posY = startY + (snake->getGridY() * cellSize);

        // Update head texture based on direction
        Direction dir = snake->getCurrentDirection();
        std::string directionTexture;
        switch (dir) {
            case Direction::UP:
                directionTexture = "snake.head_up";
                break;
            case Direction::DOWN:
                directionTexture = "snake.head_down";
                break;
            case Direction::LEFT:
                directionTexture = "snake.head_left";
                break;
            case Direction::RIGHT:
                directionTexture = "snake.head_right";
                break;
            case Direction::NONE:
                directionTexture = "snake.head_right";
                break;
        }

        auto headAsset = getDrawableAsset(directionTexture);
        if (headAsset) {
            // Keep position and visibility
            float posX = snakeDrawable->posX;
            float posY = snakeDrawable->posY;
            bool visible = snakeDrawable->isVisible;
            *snakeDrawable = *headAsset;
            snakeDrawable->posX = posX;
            snakeDrawable->posY = posY;
            snakeDrawable->isVisible = visible;
        }
    }
}

void GameLogic::checkFoodCollision(std::shared_ptr<SnakeHeadComponent> snake,
                                 std::shared_ptr<GridComponent> grid) {
    size_t headX = snake->getGridX();
    size_t headY = snake->getGridY();

    auto foodEntities = findFoodEntities();
    for (auto foodEntity : foodEntities) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(foodEntity,
                static_cast<ComponentType>(1002)));

        if (foodComp && foodComp->getGridX() == headX &&
            foodComp->getGridY() == headY) {
            // Add to score
            snake->addScore(foodComp->getPointValue());

            // Add a new segment
            auto segments = snake->getSegments();
            size_t tailX, tailY;
  
            if (segments.empty()) {
                // If no segments yet, add one behind the head
                Direction dir = snake->getCurrentDirection();
                tailX = headX;
                tailY = headY;
                switch (dir) {
                    case Direction::UP:
                        tailY = (tailY < grid->getHeight() - 1) ? tailY + 1 : 0;
                        break;
                    case Direction::DOWN:
                        tailY = (tailY > 0) ? tailY - 1 : grid->getHeight() - 1;
                        break;
                    case Direction::LEFT:
                        tailX = (tailX < grid->getWidth() - 1) ? tailX + 1 : 0;
                        break;
                    case Direction::RIGHT:
                        tailX = (tailX > 0) ? tailX - 1 : grid->getWidth() - 1;
                        break;
                    case Direction::NONE:
                        break;
                }
            } else {
                // Otherwise add one after the last segment
                auto lastSegment = segments.back();
                tailX = lastSegment.first;
                tailY = lastSegment.second;
            }
  
            snake->addSegment(tailX, tailY);
            grid->setCellType(tailX, tailY, CellType::SNAKE_BODY);

            // Remove food entity
            _entityManager->destroyEntity(foodEntity);
            grid->setCellType(headX, headY, CellType::SNAKE_HEAD);

            // If it was a bonus food, reset the timer
            if (foodComp->getFoodType() == FoodType::BONUS) {
                _bonusFoodActive = false;
            }
  
            // Create new food
            createFood(grid, FoodType::REGULAR);
            break;
        }
    }
}

bool GameLogic::checkSnakeCollision(std::shared_ptr<SnakeHeadComponent> snake,
                                   std::shared_ptr<GridComponent> grid) {
    size_t headX = snake->getGridX();
    size_t headY = snake->getGridY();

    // Check for collision with snake's own body
    auto segments = snake->getSegments();
    for (const auto& segment : segments) {
        if (segment.first == headX && segment.second == headY) {
            return true;
        }
    }

    return false;
}

void GameLogic::createFood(std::shared_ptr<GridComponent> grid, FoodType type) {
    auto pos = grid->placeFood();
    if (pos.first >= grid->getWidth() || pos.second >= grid->getHeight()) {
        return;  // No empty cell for food
    }

    float cellSize = grid->getCellSize();

    // Find grid entity to get position
    Arcade::Entity gridEntity = findGridEntity();
    auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));

    float startX = gridPosComp ? gridPosComp->x : 0;
    float startY = gridPosComp ? gridPosComp->y : 0;

    float foodX = startX + (pos.first * cellSize);
    float foodY = startY + (pos.second * cellSize);

    // Create food entity
    std::string foodName = "Food_" + std::to_string(pos.first) + "_" +
        std::to_string(pos.second);
    Arcade::Entity foodEntity = _entityManager->createEntity(foodName);

    // Add position component
    auto positionComponent = std::make_shared<PositionComponent>(foodX, foodY);
    _componentManager->registerComponent(foodEntity, positionComponent);

    // Add drawable component
    std::string assetKey = type == FoodType::BONUS ? "snake.bonus_food" :
        "snake.regular_food";
    auto spriteComponent = getDrawableAsset(assetKey);
    if (!spriteComponent) {
        spriteComponent = std::make_shared<DrawableComponent>();
        std::string spritePath = type == FoodType::BONUS ?
            "assets/snake/bonus_food.png" : "assets/snake/apple.png";
        spriteComponent->setAsTexture(spritePath, 32, 32);
        spriteComponent->setAsCharacter(type == FoodType::BONUS ? 'B' : 'A');
    }
    spriteComponent->posX = foodX;
    spriteComponent->posY = foodY;
    spriteComponent->isVisible = true;
    _componentManager->registerComponent(foodEntity, spriteComponent);

    // Add food component
    auto foodComponent = std::make_shared<FoodComponent>(type);
    foodComponent->setName(foodName);
    foodComponent->setGridPosition(pos.first, pos.second);
    _componentManager->registerComponent(foodEntity, foodComponent);
}

std::vector<Arcade::Entity> GameLogic::findFoodEntities() {
    std::vector<Arcade::Entity> foodEntities;
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
        if (foodComp) {
            foodEntities.push_back(entity);
        }
    }
    return foodEntities;
}

void GameLogic::resetGame() {
    auto [snake, grid] = getSnakeAndGridComponents();
    if (!snake || !grid)
        return;

    // Clear all food entities
    auto foodEntities = findFoodEntities();
    for (auto entity : foodEntities) {
        _entityManager->destroyEntity(entity);
    }

    // Reset grid
    grid->resetGrid();

    // Reset snake
    snake->setScore(0);
    snake->clearSegments();
    snake->setCurrentDirection(Direction::NONE);
    snake->setNextDirection(Direction::NONE);

    // Center the snake
    size_t centerX = grid->getWidth() / 2;
    size_t centerY = grid->getHeight() / 2;
    snake->setGridPosition(centerX, centerY);
    grid->setCellType(centerX, centerY, CellType::SNAKE_HEAD);

    // Add initial food
    createFood(grid, FoodType::REGULAR);

    // Reset timers
    _bonusFoodActive = false;
    _bonusFoodTimer = 0.0f;
    _difficultyTimer = 0.0f;
    _lastUpdateTime = std::chrono::high_resolution_clock::now();
}

void GameLogic::increaseDifficulty(std::shared_ptr<SnakeHeadComponent> snake) {
    // Increase snake speed by reducing movement threshold
    float currentThreshold = snake->getMovementThreshold();
    // Don't let it get too fast
    if (currentThreshold > 0.05f) {
        snake->setMovementThreshold(currentThreshold * 0.9f);
    }
}

}  // namespace Snake
}  // namespace Arcade