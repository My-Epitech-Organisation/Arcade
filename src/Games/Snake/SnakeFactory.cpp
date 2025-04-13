// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Factory implementation for creating Snake game entities
*/

#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <memory>
#include <iostream>
#include "Games/Snake/SnakeFactory.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace Snake {

Arcade::Entity SnakeFactory::createGrid(size_t width, size_t height) {
    Arcade::Entity gridEntity = _entityManager->createEntity("Grid");

    float screenWidth = 800.0f;
    float screenHeight = 600.0f;

    float x = 50.0f;
    float y = 50.0f;

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(gridEntity, positionComponent);

    auto gridComponent = std::make_shared<GridComponent>(width, height);
    gridComponent->setName("SnakeGrid");
    _componentManager->registerComponent(gridEntity, gridComponent);

    return gridEntity;
}

Arcade::Entity SnakeFactory::createSnakeHead(float x, float y,
size_t gridX, size_t gridY) {
    Arcade::Entity snakeEntity = _entityManager->createEntity("SnakeHead");

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(snakeEntity, positionComponent);

    auto spriteComponent = getDrawableAsset("snake.head_right");
    if (!spriteComponent) {
        spriteComponent = std::make_shared<DrawableComponent>();
        spriteComponent->setAsTexture("assets/snake/head_right.png", 32, 32);
        spriteComponent->setAsCharacter('>');
    }
    spriteComponent->posX = x;
    spriteComponent->posY = y;
    spriteComponent->isVisible = true;
    _componentManager->registerComponent(snakeEntity, spriteComponent);

    auto snakeComponent = std::make_shared<SnakeHeadComponent>();
    snakeComponent->setName("SnakeHead");
    snakeComponent->setGridPosition(gridX, gridY);
    snakeComponent->setCurrentDirection(Direction::NONE);
    _componentManager->registerComponent(snakeEntity, snakeComponent);

    return snakeEntity;
}

Arcade::Entity SnakeFactory::createFood(float x, float y, size_t gridX,
size_t gridY, FoodType type) {
    std::string foodName = type == FoodType::BONUS ? "BonusFood" : "Food";
    std::string assetKey = type == FoodType::BONUS ? "snake.bonus_food" :
                                                    "snake.regular_food";
    foodName += "_" + std::to_string(gridX) + "_" + std::to_string(gridY);

    Arcade::Entity foodEntity = _entityManager->createEntity(foodName);

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(foodEntity, positionComponent);

    auto spriteComponent = getDrawableAsset(assetKey);
    if (!spriteComponent) {
        spriteComponent = std::make_shared<DrawableComponent>();
        std::string spritePath = type == FoodType::BONUS ?
                               "assets/snake/bonus_food.png" :
                               "assets/snake/apple.png";
        spriteComponent->setAsTexture(spritePath, 32, 32);
        spriteComponent->setAsCharacter(type == FoodType::BONUS ? 'B' : 'A');
    }
    spriteComponent->posX = x;
    spriteComponent->posY = y;
    spriteComponent->isVisible = true;
    _componentManager->registerComponent(foodEntity, spriteComponent);

    auto foodComponent = std::make_shared<FoodComponent>(type);
    foodComponent->setName(foodName);
    foodComponent->setGridPosition(gridX, gridY);
    _componentManager->registerComponent(foodEntity, foodComponent);

    return foodEntity;
}

void SnakeFactory::initializeGame(float cellSize) {
    Arcade::Entity gridEntity = createGrid(20, 15);
    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));
    if (!gridComp) {
        throw std::runtime_error(
            "Failed to create or get GridComponent in SnakeFactory");
    }
    gridComp->setCellSize(cellSize);

    auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));
    if (gridPosComp) {
        float screenWidth = 800.0f;
        float screenHeight = 600.0f;
        float boardWidth = gridComp->getWidth() * cellSize;
        float boardHeight = gridComp->getHeight() * cellSize;
        gridPosComp->x = (screenWidth - boardWidth) / 2;
        gridPosComp->y = (screenHeight - boardHeight) / 2;
    }

    float startX = gridPosComp ? gridPosComp->x : 0;
    float startY = gridPosComp ? gridPosComp->y : 0;


    size_t centerX = gridComp->getWidth() / 2;
    size_t centerY = gridComp->getHeight() / 2;
    float snakeX = startX + (centerX * cellSize);
    float snakeY = startY + (centerY * cellSize);
    Arcade::Entity snakeEntity = createSnakeHead(
        snakeX, snakeY, centerX, centerY);
    gridComp->setCellType(centerX, centerY, CellType::SNAKE_HEAD);

    auto foodPos = gridComp->placeFood();
    if (foodPos.first < gridComp->getWidth() &&
    foodPos.second < gridComp->getHeight()) {
        float foodX = startX + (foodPos.first * cellSize);
        float foodY = startY + (foodPos.second * cellSize);
        createFood(foodX, foodY,
            foodPos.first, foodPos.second, FoodType::REGULAR);
        gridComp->setCellType(foodPos.first, foodPos.second, CellType::FOOD);
        std::cout
        << "SnakeFactory::initializeGame - Created initial food at grid ("
        << foodPos.first << ", " << foodPos.second
        << ")" << std::endl;
    } else {
         std::cerr
         << "SnakeFactory::initializeGame - Failed to place initial food."
         << std::endl;
    }
}

std::shared_ptr<DrawableComponent>
SnakeFactory::getDrawableAsset(const std::string& key) const {
    auto it = _assets.find(key);
    if (it != _assets.end()) {
        auto component = std::make_shared<DrawableComponent>(it->second);
        return component;
    }
    return nullptr;
}

}  // namespace Snake
}  // namespace Arcade
