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

    for (const auto& [entity, _] : _entityManager->getEntities()) {
        auto snakeComponentBase = _componentManager->getComponentBase(entity, "SnakeComponent");
        auto snakeComponent = dynamic_cast<SnakeComponent*>(snakeComponentBase.get());

        if (snakeComponent) {
            float actualDelay = _movementDelay / snakeComponent->speedMultiplier;
            if (elapsedTime >= actualDelay) {
                _lastUpdateTime = currentTime;

                if (snakeComponent->segments.empty()) {
                    continue;
                }

                auto headEntity = snakeComponent->segments[0];
                auto headPosBase = _componentManager->getComponentBase(headEntity, "PositionComponent");
                auto headPos = dynamic_cast<PositionComponent*>(headPosBase.get());

                auto headSpriteBase = _componentManager->getComponentBase(headEntity, "SpriteComponent");
                auto headSprite = dynamic_cast<SpriteComponent*>(headSpriteBase.get());

                if (!headPos) {
                    continue;
                }

                std::vector<std::pair<float, float>> oldPositions;
                for (auto& segment : snakeComponent->segments) {
                    auto posCompBase = _componentManager->getComponentBase(segment, "PositionComponent");
                    auto posComp = dynamic_cast<PositionComponent*>(posCompBase.get());
                    if (posComp) {
                        oldPositions.push_back(std::make_pair(posComp->x, posComp->y));
                    }
                }

                switch (snakeComponent->direction) {
                    case Direction::UP:
                        headPos->y -= 1;
                        if (headSprite) {
                            headSprite->spritePath =
                                "assets/snake/head_up.png";
                        }
                        break;
                    case Direction::DOWN:
                        headPos->y += 1;
                        if (headSprite) {
                            headSprite->spritePath =
                                "assets/snake/head_down.png";
                        }
                        break;
                    case Direction::LEFT:
                        headPos->x -= 1;
                        if (headSprite) {
                            headSprite->spritePath =
                                "assets/snake/head_left.png";
                        }
                        break;
                    case Direction::RIGHT:
                        headPos->x += 1;
                        if (headSprite) {
                            headSprite->spritePath =
                                "assets/snake/head_right.png";
                        }
                        break;
                }

                for (const auto& [foodEntity, _] : _entityManager->getEntities()) {
                    auto foodComponentBase = _componentManager->getComponentBase(foodEntity, "FoodComponent");
                    auto foodComponent = dynamic_cast<FoodComponent*>(foodComponentBase.get());

                    if (foodComponent) {
                        auto foodPosBase = _componentManager->getComponentBase(foodEntity, "PositionComponent");
                        auto foodPos = dynamic_cast<PositionComponent*>(foodPosBase.get());

                        if (foodPos && headPos->x == foodPos->x && headPos->y == foodPos->y) {
                            foodComponent->eaten = true;

                            snakeComponent->shouldGrow = true;
                            break;
                        }
                    }
                }

                for (size_t i = 1; i < snakeComponent->segments.size(); ++i) {
                    auto segmentEntity = snakeComponent->segments[i];
                    auto segmentPosBase = _componentManager->getComponentBase(segmentEntity, "PositionComponent");
                    auto segmentPos = dynamic_cast<PositionComponent*>(segmentPosBase.get());

                    auto segmentSpriteBase = _componentManager->getComponentBase(segmentEntity, "SpriteComponent");
                    auto segmentSprite = dynamic_cast<SpriteComponent*>(segmentSpriteBase.get());

                    if (segmentPos) {
                        segmentPos->x = oldPositions[i - 1].first;
                        segmentPos->y = oldPositions[i - 1].second;

                        if (segmentSprite) {
                            if (i == snakeComponent->segments.size() - 1) {
                                if (segmentPos->x < oldPositions[i - 1].first) {
                                    segmentSprite->spritePath = "assets/snake/tail_left.png";
                                } else if (segmentPos->x > oldPositions[i - 1].first) {
                                    segmentSprite->spritePath = "assets/snake/tail_right.png";
                                } else if (segmentPos->y < oldPositions[i - 1].second) {
                                    segmentSprite->spritePath = "assets/snake/tail_up.png";
                                } else {
                                    segmentSprite->spritePath = "assets/snake/tail_down.png";
                                }
                            } else {
                                if ((i + 1 < oldPositions.size()) &&
                                    (oldPositions[i - 1].first == oldPositions[i + 1].first ||
                                    oldPositions[i - 1].second == oldPositions[i + 1].second)) {
                                    if (oldPositions[i - 1].first == oldPositions[i + 1].first) {
                                        segmentSprite->spritePath = "assets/snake/body_vertical.png";
                                    } else {
                                        segmentSprite->spritePath = "assets/snake/body_horizontal.png";
                                    }
                                } else if (i + 1 < oldPositions.size()) {
                                    if ((oldPositions[i - 1].first < oldPositions[i].first && oldPositions[i + 1].second < oldPositions[i].second) ||
                                        (oldPositions[i - 1].second < oldPositions[i].second && oldPositions[i + 1].first < oldPositions[i].first)) {
                                        segmentSprite->spritePath = "assets/snake/body_bottomleft.png";
                                    } else if ((oldPositions[i - 1].first < oldPositions[i].first && oldPositions[i + 1].second > oldPositions[i].second) ||
                                              (oldPositions[i - 1].second > oldPositions[i].second && oldPositions[i + 1].first < oldPositions[i].first)) {
                                        segmentSprite->spritePath = "assets/snake/body_topleft.png";
                                    } else if ((oldPositions[i - 1].first > oldPositions[i].first && oldPositions[i + 1].second < oldPositions[i].second) ||
                                              (oldPositions[i - 1].second < oldPositions[i].second && oldPositions[i + 1].first > oldPositions[i].first)) {
                                        segmentSprite->spritePath = "assets/snake/body_bottomright.png";
                                    } else {
                                        segmentSprite->spritePath = "assets/snake/body_topright.png";
                                    }
                                }
                            }
                        }
                    }
                }

                if (snakeComponent->shouldGrow) {
                    auto tailIndex = snakeComponent->segments.size() - 1;
                    auto tailEntity = snakeComponent->segments[tailIndex];
                    auto tailPosBase = _componentManager->getComponentBase(tailEntity, "PositionComponent");
                    auto tailPos = dynamic_cast<PositionComponent*>(tailPosBase.get());

                    if (tailPos) {
                        auto newSegmentEntity = _entityManager->createEntity("snake_segment");

                        float x = oldPositions[tailIndex].first;
                        float y = oldPositions[tailIndex].second;
                        auto newPosComp = std::make_shared<PositionComponent>(x, y);
                        _componentManager->registerComponent(newSegmentEntity, newPosComp);

                        auto newSpriteComp = std::make_shared<SpriteComponent>("assets/snake/tail_down.png"); // Default, will be updated on next move
                        _componentManager->registerComponent(newSegmentEntity, newSpriteComp);

                        snakeComponent->segments.push_back(newSegmentEntity);

                        snakeComponent->shouldGrow = false;
                    }
                }
            }
        }
    }
}

}  // namespace Arcade
