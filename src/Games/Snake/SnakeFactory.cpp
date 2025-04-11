// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Factory for creating Snake game entities implementation
*/

#include <memory>
#include <string>
#include "Games/Snake/SnakeFactory.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Games/Snake/Components/Food.hpp"
#include "Games/Snake/Types.hpp"
#include "Games/Snake/Components/GridComponent.hpp"

namespace Arcade {

Arcade::Entity SnakeFactory::createSnake(float x, float y, Direction initialDirection) {
    Arcade::Entity gridEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "grid") {
            gridEntity = entity;
            break;
        }
    }

    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity, 
            static_cast<ComponentType>(1000)));

    float cellSize = 20.0f;
    if (gridComp) {
        cellSize = gridComp->getCellSize();
    }

    size_t gridX = static_cast<size_t>(x / cellSize);
    size_t gridY = static_cast<size_t>(y / cellSize);

    Arcade::Entity snakeEntity = _entityManager->createEntity("snake");

    auto snakeComponent = std::make_shared<SnakeComponent>();
    snakeComponent->direction = initialDirection;
    snakeComponent->nextDirection = initialDirection;
    snakeComponent->setHeadPosition(gridX, gridY);

    snakeComponent->segmentPositions.clear();

    Arcade::Entity head = createSnakeSegment(gridX, gridY, true, false, initialDirection);
    snakeComponent->segments.push_back(head);
    snakeComponent->segmentPositions.push_back(std::make_pair(gridX, gridY));

    if (gridComp) {
        gridComp->setCellType(gridX, gridY, SnakeCellType::SNAKE_HEAD);
        gridComp->setEntityAtCell(gridX, gridY, head);
    }

    size_t bodyX = gridX;
    size_t bodyY = gridY;

    switch (initialDirection) {
        case Direction::UP:
            bodyY += 1;
            break;
        case Direction::DOWN:
            bodyY -= 1;
            break;
        case Direction::LEFT:
            bodyX += 1;
            break;
        case Direction::RIGHT:
            bodyX -= 1;
            break;
    }

    Arcade::Entity body1 = createSnakeSegment(bodyX, bodyY, false, false, initialDirection);
    snakeComponent->segments.push_back(body1);
    snakeComponent->segmentPositions.push_back(std::make_pair(bodyX, bodyY));

    if (gridComp) {
        gridComp->setCellType(bodyX, bodyY, SnakeCellType::SNAKE_BODY);
        gridComp->setEntityAtCell(bodyX, bodyY, body1);
    }

    switch (initialDirection) {
        case Direction::UP:
            bodyY += 1;
            break;
        case Direction::DOWN:
            bodyY -= 1;
            break;
        case Direction::LEFT:
            bodyX += 1;
            break;
        case Direction::RIGHT:
            bodyX -= 1;
            break;
    }

    Arcade::Entity tail = createSnakeSegment(bodyX, bodyY, false, true, initialDirection);
    snakeComponent->segments.push_back(tail);
    snakeComponent->segmentPositions.push_back(std::make_pair(bodyX, bodyY));

    if (gridComp) {
        gridComp->setCellType(bodyX, bodyY, SnakeCellType::SNAKE_TAIL);
        gridComp->setEntityAtCell(bodyX, bodyY, tail);
    }

    _componentManager->registerComponent(snakeEntity, snakeComponent);

    return snakeEntity;
}

Arcade::Entity SnakeFactory::createSnakeSegment(float gridX, float gridY,
bool isHead, bool isTail, Direction dir) {
    Arcade::Entity entity = _entityManager->createEntity("snake_segment");

    float cellSize = 20.0f;
    Arcade::Entity gridEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "grid") {
            gridEntity = entity;
            break;
        }
    }
    if (gridEntity) {
        auto gridComp = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(gridEntity, 
                static_cast<ComponentType>(1000)));
        if (gridComp) {
            cellSize = gridComp->getCellSize();
        }
    }

    auto positionComponent = std::make_shared<PositionComponent>(
        gridX * cellSize,
        gridY * cellSize
    );

    _componentManager->registerComponent(entity, positionComponent);

    std::string spritePath = getSnakeSegmentSprite(isHead, isTail, dir);
    auto spriteComponent = std::make_shared<SpriteComponent>(spritePath);
    _componentManager->registerComponent(entity, spriteComponent);

    return entity;
}

Arcade::Entity SnakeFactory::createFood(float x, float y) {
    Arcade::Entity entity = _entityManager->createEntity("food");

    Arcade::Entity gridEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "grid") {
            gridEntity = entity;
            break;
        }
    }

    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity, 
            static_cast<ComponentType>(1000)));

    float cellSize = 20.0f;
    size_t gridX = static_cast<size_t>(x);
    size_t gridY = static_cast<size_t>(y);

    if (gridComp) {
        cellSize = gridComp->getCellSize();
        gridX = static_cast<size_t>(x / cellSize);
        gridY = static_cast<size_t>(y / cellSize);

        gridComp->setCellType(gridX, gridY, SnakeCellType::FOOD);
        gridComp->setEntityAtCell(gridX, gridY, entity);
    }

    auto positionComponent = std::make_shared<PositionComponent>(
        gridX * cellSize,
        gridY * cellSize
    );
    _componentManager->registerComponent(entity, positionComponent);

    auto spriteComponent = std::make_shared<SpriteComponent>(
        "assets/snake/apple.png");
    _componentManager->registerComponent(entity, spriteComponent);

    auto foodComponent = std::make_shared<FoodComponent>();
    _componentManager->registerComponent(entity, foodComponent);

    return entity;
}

Arcade::Entity SnakeFactory::createGrid(size_t width, size_t height, float cellSize) {
    Arcade::Entity entity = _entityManager->createEntity("grid");
    auto gridComponent = std::make_shared<GridComponent>(width, height);
    gridComponent->setCellSize(cellSize);
    gridComponent->initializeEmptyGrid();

    _componentManager->registerComponent(entity, gridComponent);
    return entity;
}

std::string SnakeFactory::getSnakeSegmentSprite(bool isHead,
bool isTail, Direction dir) {
    if (isHead) {
        switch (dir) {
            case Direction::UP:
                return "assets/snake/head_up.png";
            case Direction::DOWN:
                return "assets/snake/head_down.png";
            case Direction::LEFT:
                return "assets/snake/head_left.png";
            case Direction::RIGHT:
                return "assets/snake/head_right.png";
        }
    } else if (isTail) {
        switch (dir) {
            case Direction::UP:
                return "assets/snake/tail_up.png";
            case Direction::DOWN:
                return "assets/snake/tail_down.png";
            case Direction::LEFT:
                return "assets/snake/tail_left.png";
            case Direction::RIGHT:
                return "assets/snake/tail_right.png";
        }
    } else {
        switch (dir) {
            case Direction::UP:
            case Direction::DOWN:
                return "assets/snake/body_vertical.png";
            case Direction::LEFT:
            case Direction::RIGHT:
                return "assets/snake/body_horizontal.png";
        }
    }
    return "assets/snake/body_horizontal.png";
}

}  // namespace Arcade
