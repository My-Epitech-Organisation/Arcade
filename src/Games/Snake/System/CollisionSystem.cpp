// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Collision System for Snake game implementation
*/

#include "Games/Snake/System/CollisionSystem.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Games/Snake/Components/Food.hpp"
#include "Games/Snake/Types.hpp"

namespace Arcade {

CollisionSystem::CollisionSystem(
std::shared_ptr<IComponentManager> componentManager,
std::shared_ptr<IEntityManager> entityManager)
: _componentManager(componentManager),
_entityManager(entityManager),
_collision(false) {}

void CollisionSystem::update() {
    _collision = false;

    for (const auto& [entity, _] : _entityManager->getEntities()) {
        auto snakeComponentBase = _componentManager->getComponentBase(entity, "SnakeComponent");
        auto snakeComponent = dynamic_cast<SnakeComponent*>(snakeComponentBase.get());

        if (snakeComponent && !snakeComponent->segments.empty()) {
            auto headEntity = snakeComponent->segments[0];
            auto headPosBase = _componentManager->getComponentBase(headEntity, "PositionComponent");
            auto headPos = dynamic_cast<PositionComponent*>(headPosBase.get());

            if (headPos) {
                if (checkBoundaryCollision(headPos->x, headPos->y)) {
                    _collision = true;
                    return;
                }

                if (checkSelfCollision(snakeComponent->segments)) {
                    _collision = true;
                    return;
                }
            }
        }
    }
}

bool CollisionSystem::checkBoundaryCollision(float x, float y)
{
    const float minX = 0;
    const float maxX = 29;
    const float minY = 0;
    const float maxY = 29;

    return (x < minX || x > maxX || y < minY || y > maxY);
}

bool CollisionSystem::checkSelfCollision(const std::vector<Entity>& segments) {
    if (segments.size() <= 1) {
        return false;
    }

    auto headEntity = segments[0];
    auto headPosBase = _componentManager->getComponentBase(headEntity, "PositionComponent");
    auto headPos = dynamic_cast<PositionComponent*>(headPosBase.get());

    if (!headPos) {
        return false;
    }

    for (size_t i = 3; i < segments.size(); ++i) {
        auto segmentPosBase = _componentManager->getComponentBase(segments[i], "PositionComponent");
        auto segmentPos = dynamic_cast<PositionComponent*>(segmentPosBase.get());

        if (segmentPos && headPos->x == segmentPos->x && headPos->y == segmentPos->y) {
            return true;
        }
    }

    return false;
}

}  // namespace Arcade
