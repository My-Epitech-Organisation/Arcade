// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Movement System for Snake game implementation
*/

#include "Games/Snake/System/MovementSystem.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Games/Snake/Components/Food.hpp"
#include "Games/Snake/Types.hpp"
#include "Games/Snake/Components/GridComponent.hpp"
#include <iostream>

namespace Arcade {

MovementSystem::MovementSystem(
std::shared_ptr<IComponentManager> componentManager,
std::shared_ptr<IEntityManager> entityManager)
: _componentManager(componentManager),
_entityManager(entityManager),
_lastUpdateTime(std::chrono::high_resolution_clock::now()),
_movementDelay(0.2f) {}

void MovementSystem::update() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - _lastUpdateTime).count() / 1000.0f;

    Arcade::Entity gridEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "grid") {
            gridEntity = entity;
            break;
        }
    }

    if (gridEntity == 0)
        return;

    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity, 
            static_cast<ComponentType>(1000)));

    if (!gridComp)
        return;

    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto snakeComponentBase = _componentManager->getComponentBase(entity, "SnakeComponent");
        auto snakeComponent = dynamic_cast<SnakeComponent*>(snakeComponentBase.get());

        if (snakeComponent) {
            if (snakeComponent->nextDirection != snakeComponent->direction) {
                bool validChange = false;
                switch (snakeComponent->direction) {
                    case Direction::UP:
                        validChange = snakeComponent->nextDirection != Direction::DOWN;
                        break;
                    case Direction::DOWN:
                        validChange = snakeComponent->nextDirection != Direction::UP;
                        break;
                    case Direction::LEFT:
                        validChange = snakeComponent->nextDirection != Direction::RIGHT;
                        break;
                    case Direction::RIGHT:
                        validChange = snakeComponent->nextDirection != Direction::LEFT;
                        break;
                }
                if (validChange) {
                    snakeComponent->direction = snakeComponent->nextDirection;
                }
            }

            float actualDelay = _movementDelay / snakeComponent->speedMultiplier;
            snakeComponent->updateMovementTimer(elapsedTime);

            if (snakeComponent->canMove()) {
                _lastUpdateTime = currentTime;
                snakeComponent->setCanMove(false);

                if (snakeComponent->segments.empty()) {
                    continue;
                }

                size_t oldHeadX = snakeComponent->getHeadX();
                size_t oldHeadY = snakeComponent->getHeadY();

                size_t newHeadX = oldHeadX;
                size_t newHeadY = oldHeadY;

                switch (snakeComponent->direction) {
                    case Direction::UP:
                        if (newHeadY > 0) newHeadY--;
                        break;
                    case Direction::DOWN:
                        newHeadY++;
                        break;
                    case Direction::LEFT:
                        if (newHeadX > 0) newHeadX--;
                        break;
                    case Direction::RIGHT:
                        newHeadX++;
                        break;
                }

                std::vector<std::pair<size_t, size_t>> oldPositions;
                oldPositions.clear();

                for (size_t i = 0; i < snakeComponent->segmentPositions.size(); ++i) {
                    oldPositions.push_back(snakeComponent->segmentPositions[i]);
                }

                if (gridComp->getCellType(oldHeadX, oldHeadY) == SnakeCellType::SNAKE_HEAD) {
                    gridComp->clearCell(oldHeadX, oldHeadY);
                }

                bool foundFood = false;
                if (gridComp->getCellType(newHeadX, newHeadY) == SnakeCellType::FOOD) {
                    foundFood = true;
                    snakeComponent->shouldGrow = true;

                    for (const auto& [foodEntity, foodName] : _entityManager->getEntities()) {
                        auto foodComponentBase = _componentManager->getComponentBase(foodEntity, "FoodComponent");
                        auto foodComponent = dynamic_cast<FoodComponent*>(foodComponentBase.get());

                        if (foodComponent) {
                            auto foodPosBase = _componentManager->getComponentBase(foodEntity, "PositionComponent");
                            auto foodPos = dynamic_cast<PositionComponent*>(foodPosBase.get());
                            if (foodPos && foodPos->x == newHeadX * gridComp->getCellSize() && 
                                foodPos->y == newHeadY * gridComp->getCellSize()) {
                                foodComponent->eaten = true;
                                snakeComponent->addScore(10);
                                break;
                            }
                        }
                    }
                }

                snakeComponent->setHeadPosition(newHeadX, newHeadY);

                auto headEntity = snakeComponent->segments[0];
                auto headPosBase = _componentManager->getComponentBase(headEntity, "PositionComponent");
                auto headPos = dynamic_cast<PositionComponent*>(headPosBase.get());
                auto headSpriteBase = _componentManager->getComponentBase(headEntity, "SpriteComponent");
                auto headSprite = dynamic_cast<SpriteComponent*>(headSpriteBase.get());

                if (headPos) {
                    headPos->x = newHeadX * gridComp->getCellSize();
                    headPos->y = newHeadY * gridComp->getCellSize();

                    if (headSprite) {
                        switch (snakeComponent->direction) {
                            case Direction::UP:
                                headSprite->spritePath = "assets/snake/head_up.png";
                                break;
                            case Direction::DOWN:
                                headSprite->spritePath = "assets/snake/head_down.png";
                                break;
                            case Direction::LEFT:
                                headSprite->spritePath = "assets/snake/head_left.png";
                                break;
                            case Direction::RIGHT:
                                headSprite->spritePath = "assets/snake/head_right.png";
                                break;
                        }
                    }
                }

                if (snakeComponent->segmentPositions.empty()) {
                    snakeComponent->segmentPositions.push_back(std::make_pair(newHeadX, newHeadY));
                } else {
                    snakeComponent->segmentPositions[0] = std::make_pair(newHeadX, newHeadY);
                }

                gridComp->setCellType(newHeadX, newHeadY, SnakeCellType::SNAKE_HEAD);
                gridComp->setEntityAtCell(newHeadX, newHeadY, headEntity);

                for (size_t i = 1; i < snakeComponent->segments.size(); ++i) {
                    auto segmentEntity = snakeComponent->segments[i];
                    auto segmentPosBase = _componentManager->getComponentBase(segmentEntity, "PositionComponent");
                    auto segmentPos = dynamic_cast<PositionComponent*>(segmentPosBase.get());
                    auto segmentSpriteBase = _componentManager->getComponentBase(segmentEntity, "SpriteComponent");
                    auto segmentSprite = dynamic_cast<SpriteComponent*>(segmentSpriteBase.get());

                    if (segmentPos) {
                        size_t oldX = snakeComponent->segmentPositions[i].first;
                        size_t oldY = snakeComponent->segmentPositions[i].second;
                        gridComp->clearCell(oldX, oldY);

                        size_t newX = oldPositions[i - 1].first;
                        size_t newY = oldPositions[i - 1].second;
                        snakeComponent->segmentPositions[i] = std::make_pair(newX, newY);

                        segmentPos->x = newX * gridComp->getCellSize();
                        segmentPos->y = newY * gridComp->getCellSize();

                        SnakeCellType cellType = (i == snakeComponent->segments.size() - 1) ? 
                            SnakeCellType::SNAKE_TAIL : SnakeCellType::SNAKE_BODY;
                        gridComp->setCellType(newX, newY, cellType);
                        gridComp->setEntityAtCell(newX, newY, segmentEntity);

                        if (segmentSprite) {
                            if (i == snakeComponent->segments.size() - 1) {
                                if (newX < oldPositions[i - 1].first) {
                                    segmentSprite->spritePath = "assets/snake/tail_left.png";
                                } else if (newX > oldPositions[i - 1].first) {
                                    segmentSprite->spritePath = "assets/snake/tail_right.png";
                                } else if (newY < oldPositions[i - 1].second) {
                                    segmentSprite->spritePath = "assets/snake/tail_up.png";
                                } else {
                                    segmentSprite->spritePath = "assets/snake/tail_down.png";
                                }
                            } else {
                                if (newX == oldPositions[i + 1].first) {
                                    segmentSprite->spritePath = "assets/snake/body_vertical.png";
                                } else {
                                    segmentSprite->spritePath = "assets/snake/body_horizontal.png";
                                }
                            }
                        }
                    }
                }

                if (snakeComponent->shouldGrow) {
                    size_t tailIndex = snakeComponent->segments.size() - 1;
                    size_t oldTailX = snakeComponent->segmentPositions[tailIndex].first;
                    size_t oldTailY = snakeComponent->segmentPositions[tailIndex].second;

                    auto newSegmentEntity = _entityManager->createEntity("snake_segment");
                    auto newPosComp = std::make_shared<PositionComponent>(
                        oldTailX * gridComp->getCellSize(), 
                        oldTailY * gridComp->getCellSize()
                    );
                    _componentManager->registerComponent(newSegmentEntity, newPosComp);

                    auto newSpriteComp = std::make_shared<SpriteComponent>("assets/snake/tail_down.png");
                    _componentManager->registerComponent(newSegmentEntity, newSpriteComp);

                    snakeComponent->segments.push_back(newSegmentEntity);
                    snakeComponent->segmentPositions.push_back(std::make_pair(oldTailX, oldTailY));

                    auto oldTailEntity = snakeComponent->segments[tailIndex];
                    auto oldTailSpriteBase = _componentManager->getComponentBase(oldTailEntity, "SpriteComponent");
                    auto oldTailSprite = dynamic_cast<SpriteComponent*>(oldTailSpriteBase.get());
                    if (oldTailSprite) {
                        oldTailSprite->spritePath = "assets/snake/body_vertical.png";
                    }
                    gridComp->setCellType(oldTailX, oldTailY, SnakeCellType::SNAKE_TAIL);
                    gridComp->setEntityAtCell(oldTailX, oldTailY, newSegmentEntity);

                    snakeComponent->shouldGrow = false;
                }
            }
        }
    }
}

}  // namespace Arcade
