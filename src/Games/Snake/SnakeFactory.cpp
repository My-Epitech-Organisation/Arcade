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

namespace Arcade {

Arcade::Entity SnakeFactory::createSnake(float x, float y,
Direction initialDirection) {
    Arcade::Entity snakeEntity = _entityManager->createEntity("snake");

    auto snakeComponent = std::make_shared<SnakeComponent>();
    snakeComponent->direction = initialDirection;

    Arcade::Entity head = createSnakeSegment(x, y,
        true, false, initialDirection);
    snakeComponent->segments.push_back(head);

    float bodyX = x;
    float bodyY = y;

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

    Arcade::Entity body1 = createSnakeSegment(bodyX, bodyY,
        false, false, initialDirection);
    snakeComponent->segments.push_back(body1);

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

    Arcade::Entity tail = createSnakeSegment(bodyX, bodyY,
        false, true, initialDirection);
    snakeComponent->segments.push_back(tail);

    _componentManager->registerComponent(snakeEntity, snakeComponent);

    return snakeEntity;
}

Arcade::Entity SnakeFactory::createSnakeSegment(float x, float y,
bool isHead, bool isTail, Direction dir) {
    Arcade::Entity entity = _entityManager->createEntity("snake_segment");

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(entity, positionComponent);

    std::string spritePath = getSnakeSegmentSprite(isHead, isTail, dir);
    auto spriteComponent = std::make_shared<SpriteComponent>(spritePath);
    _componentManager->registerComponent(entity, spriteComponent);

    return entity;
}

Arcade::Entity SnakeFactory::createFood(float x, float y) {
    Arcade::Entity entity = _entityManager->createEntity("food");

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(entity, positionComponent);

    auto spriteComponent = std::make_shared<SpriteComponent>(
        "assets/snake/apple.png");
    _componentManager->registerComponent(entity, spriteComponent);

    auto foodComponent = std::make_shared<FoodComponent>();
    _componentManager->registerComponent(entity, foodComponent);

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
