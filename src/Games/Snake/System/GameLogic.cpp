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
#include <map>
#include <string>
#include <utility>
#include <algorithm>
#include <random>
#include <chrono>
#include "Games/Snake/System/GameLogic.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace Snake {

GameLogic::GameLogic(std::shared_ptr<Arcade::IComponentManager>
componentManager,
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

    snake->addGameTime(getDeltaTime());
    snake->updateMovementTimer(getDeltaTime());

    tryChangeDirection(snake, grid);

    _bonusFoodTimer += getDeltaTime();
    if (!_bonusFoodActive && _bonusFoodTimer > 15.0f) {
        createFood(grid, FoodType::BONUS);
        _bonusFoodActive = true;
        _bonusFoodTimer = 0.0f;
    }

    auto foodEntities = findFoodEntities();
    for (auto entity : foodEntities) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
        if (foodComp && foodComp->getFoodType() == FoodType::BONUS) {
            if (foodComp->updateTimeRemaining(getDeltaTime())) {
                grid->setCellType(foodComp->getGridX(), foodComp->getGridY(),
                    CellType::EMPTY);
                _entityManager->destroyEntity(entity);
                _bonusFoodActive = false;
            }
        }
    }

    _difficultyTimer += getDeltaTime();
    if (_difficultyTimer >= 10.0f) {
        increaseDifficulty(snake);
        _difficultyTimer = 0.0f;
    }

    if (snake->canMove()) {
        if (moveSnake(snake, grid)) {
            updateDrawablePositions(snake, grid);
            snake->setCanMove(false);
            snake->resetMovementTimer();

            checkFoodCollision(snake, grid);

            if (checkSnakeCollision(snake, grid)) {
                grid->setGameOver(true);
            }
        }
    }

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

    // Update snake head asset based on new direction
    Arcade::Entity snakeEntity = findSnakeHeadEntity();
    auto snakeDrawable = std::dynamic_pointer_cast<DrawableComponent>(
        _componentManager->getComponentByType(snakeEntity,
        ComponentType::DRAWABLE));
    if (snakeDrawable) {
        switch (nextDir) {
            case Direction::UP:
                snakeDrawable->setAsTexture("assets/snake/head_up.png",
                    32, 32);
                break;
            case Direction::DOWN:
                snakeDrawable->setAsTexture("assets/snake/head_down.png",
                    32, 32);
                break;
            case Direction::LEFT:
                snakeDrawable->setAsTexture("assets/snake/head_left.png",
                    32, 32);
                break;
            case Direction::RIGHT:
                snakeDrawable->setAsTexture("assets/snake/head_right.png",
                    32, 32);
                break;
            default:
                break;
        }
    }
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

    if ((newX == currentX && newY == currentY) && !grid->isCyclical()) {
        grid->setGameOver(true);
        return false;
    }

    updateSnakeBody(snake, grid, newX, newY);

    return true;
}

void GameLogic::updateSnakeBody(std::shared_ptr<SnakeHeadComponent> snake,
std::shared_ptr<GridComponent> grid,
size_t newHeadX, size_t newHeadY) {
    auto segments = snake->getSegments();

    for (const auto& segment : segments) {
        grid->setCellType(segment.first, segment.second, CellType::EMPTY);
    }

    grid->setCellType(snake->getGridX(), snake->getGridY(), CellType::EMPTY);

    std::vector<std::pair<size_t, size_t>> newSegments;
    if (!segments.empty()) {
        newSegments.push_back(std::make_pair(snake->getGridX(),
            snake->getGridY()));

        for (size_t i = 0; i < segments.size() - 1; ++i) {
            newSegments.push_back(segments[i]);
        }
    }

    snake->setGridPosition(newHeadX, newHeadY);
    grid->setCellType(newHeadX, newHeadY, CellType::SNAKE_HEAD);

    snake->clearSegments();
    for (const auto& segment : newSegments) {
        snake->addSegment(segment.first, segment.second);
        grid->setCellType(segment.first, segment.second, CellType::SNAKE_BODY);
    }
}

void GameLogic::updateDrawablePositions(std::shared_ptr<SnakeHeadComponent>
snake,
std::shared_ptr<GridComponent> grid) {
    Arcade::Entity snakeEntity = findSnakeHeadEntity();
    Arcade::Entity gridEntity = findGridEntity();

    auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));
    float startX = gridPosComp ? gridPosComp->x : 0;
    float startY = gridPosComp ? gridPosComp->y : 0;
    float cellSize = grid->getCellSize();

    auto snakePosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(snakeEntity,
            ComponentType::POSITION));
    if (snakePosComp) {
        snakePosComp->x = startX + (snake->getGridX() * cellSize);
        snakePosComp->y = startY + (snake->getGridY() * cellSize);
    }

    auto snakeDrawable = std::dynamic_pointer_cast<DrawableComponent>(
        _componentManager->getComponentByType(snakeEntity,
            ComponentType::DRAWABLE));
    if (snakeDrawable) {
        snakeDrawable->posX = startX + (snake->getGridX() * cellSize);
        snakeDrawable->posY = startY + (snake->getGridY() * cellSize);

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
            float posX = snakeDrawable->posX;
            float posY = snakeDrawable->posY;
            bool visible = snakeDrawable->isVisible;
            *snakeDrawable = *headAsset;
            snakeDrawable->posX = posX;
            snakeDrawable->posY = posY;
            snakeDrawable->isVisible = visible;
        }
    }

    auto segments = snake->getSegments();
    updateBodySegmentEntities(segments, startX, startY, cellSize);
}

void GameLogic::updateBodySegmentEntities(
const std::vector<std::pair<size_t, size_t>>& segments,
float startX, float startY, float cellSize) {
std::map<std::string, Arcade::Entity> existingSegmentEntitiesByName;
    std::vector<Arcade::Entity> allSegmentEntities;

    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name.find("SnakeSegment_") == 0) {
            existingSegmentEntitiesByName[name] = entity;
            allSegmentEntities.push_back(entity);
        }
    }

    for (size_t i = 0; i < segments.size(); ++i) {
        Arcade::Entity segmentEntity;
        std::string segmentName = "SnakeSegment_" + std::to_string(i);

        auto it = existingSegmentEntitiesByName.find(segmentName);
        if (it != existingSegmentEntitiesByName.end()) {
            segmentEntity = it->second;
            existingSegmentEntitiesByName.erase(it);
        } else {
            segmentEntity = _entityManager->createEntity(segmentName);
            auto positionComponent = std::make_shared<PositionComponent>(0, 0);
            _componentManager->registerComponent(segmentEntity,
            positionComponent);

            auto drawableComponent = std::make_shared<DrawableComponent>();
            auto bodyAsset = getDrawableAsset("snake.body");
            if (bodyAsset) {
                *drawableComponent = *bodyAsset;
            } else {
                drawableComponent->setAsTexture("assets/snake/body.png",
                    32, 32);
                drawableComponent->setAsCharacter('o');
            }
            drawableComponent->isVisible = true;
            _componentManager->registerComponent(segmentEntity,
                drawableComponent);
        }

        auto posComp = std::dynamic_pointer_cast<PositionComponent>(
            _componentManager->getComponentByType(segmentEntity,
                ComponentType::POSITION));
        if (posComp) {
            posComp->x = startX + (segments[i].first * cellSize);
            posComp->y = startY + (segments[i].second * cellSize);
        }

        auto drawComp = std::dynamic_pointer_cast<DrawableComponent>(
            _componentManager->getComponentByType(segmentEntity,
                ComponentType::DRAWABLE));
        if (drawComp) {
            drawComp->posX = startX + (segments[i].first * cellSize);
            drawComp->posY = startY + (segments[i].second * cellSize);
            drawComp->isVisible = true;
        }
    }
    for (const auto& [name, entity] : existingSegmentEntitiesByName) {
        _entityManager->destroyEntity(entity);
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
            snake->addScore(foodComp->getPointValue());

            auto segments = snake->getSegments();
            size_t tailX, tailY;

            if (segments.empty()) {
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
                auto lastSegment = segments.back();
                tailX = lastSegment.first;
                tailY = lastSegment.second;
            }

            snake->addSegment(tailX, tailY);
            grid->setCellType(tailX, tailY, CellType::SNAKE_BODY);

            auto foodDrawable = std::dynamic_pointer_cast<DrawableComponent>(
                _componentManager->getComponentByType(foodEntity,
                ComponentType::DRAWABLE));
            if (foodDrawable) {
                foodDrawable->isVisible = false;
            }

            _entityManager->destroyEntity(foodEntity);
            grid->setCellType(headX, headY, CellType::SNAKE_HEAD);

            if (foodComp->getFoodType() == FoodType::BONUS) {
                _bonusFoodActive = false;
            }

            createFood(grid, FoodType::REGULAR);
            break;
        }
    }
}

bool GameLogic::checkSnakeCollision(std::shared_ptr<SnakeHeadComponent> snake,
std::shared_ptr<GridComponent> grid) {
    size_t headX = snake->getGridX();
    size_t headY = snake->getGridY();

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
        return;
    }

    float cellSize = grid->getCellSize();

    Arcade::Entity gridEntity = findGridEntity();
    auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));

    float startX = gridPosComp ? gridPosComp->x : 0;
    float startY = gridPosComp ? gridPosComp->y : 0;

    float foodX = startX + (pos.first * cellSize);
    float foodY = startY + (pos.second * cellSize);

    std::string foodName = "Food_" + std::to_string(pos.first) + "_" +
        std::to_string(pos.second);
    Arcade::Entity foodEntity = _entityManager->createEntity(foodName);

    auto positionComponent = std::make_shared<PositionComponent>(foodX, foodY);
    _componentManager->registerComponent(foodEntity, positionComponent);

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

    auto foodEntities = findFoodEntities();
    for (auto entity : foodEntities) {
        _entityManager->destroyEntity(entity);
    }

    grid->resetGrid();

    snake->setScore(0);
    snake->clearSegments();
    snake->setCurrentDirection(Direction::NONE);
    snake->setNextDirection(Direction::NONE);

    size_t centerX = grid->getWidth() / 2;
    size_t centerY = grid->getHeight() / 2;
    snake->setGridPosition(centerX, centerY);
    grid->setCellType(centerX, centerY, CellType::SNAKE_HEAD);

    // createFood(grid, FoodType::REGULAR);

    _bonusFoodActive = false;
    _bonusFoodTimer = 0.0f;
    _difficultyTimer = 0.0f;
    _lastUpdateTime = std::chrono::high_resolution_clock::now();
}

void GameLogic::increaseDifficulty(std::shared_ptr<SnakeHeadComponent> snake) {
    float currentThreshold = snake->getMovementThreshold();
    if (currentThreshold > 0.05f) {
        snake->setMovementThreshold(currentThreshold * 0.9f);
    }
}

}  // namespace Snake
}  // namespace Arcade
