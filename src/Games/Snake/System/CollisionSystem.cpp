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
#include "Games/Snake/Components/GridComponent.hpp"
#include <iostream>

namespace Arcade {

CollisionSystem::CollisionSystem(
std::shared_ptr<IComponentManager> componentManager,
std::shared_ptr<IEntityManager> entityManager)
: _componentManager(componentManager),
_entityManager(entityManager),
_collision(false) {}

void CollisionSystem::update() {
    _collision = false;

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

    for (const auto& [entity, _] : _entityManager->getEntities()) {
        auto snakeComponentBase = _componentManager->getComponentBase(entity, "SnakeComponent");
        auto snakeComponent = dynamic_cast<SnakeComponent*>(snakeComponentBase.get());

        if (snakeComponent) {
            size_t headX = snakeComponent->getHeadX();
            size_t headY = snakeComponent->getHeadY();

            if (headX >= gridComp->getWidth() || headY >= gridComp->getHeight() ||
                gridComp->getCellType(headX, headY) == SnakeCellType::WALL) {
                _collision = true;
                gridComp->setGameOver(true);
                std::cout << "Wall collision detected!" << std::endl;
                return;
            }

            if (checkSelfCollision(snakeComponent)) {
                _collision = true;
                gridComp->setGameOver(true);
                std::cout << "Self collision detected!" << std::endl;
                return;
            }
        }
    }
}

bool CollisionSystem::checkBoundaryCollision(float x, float y)
{
    return false;
}

bool CollisionSystem::checkSelfCollision(SnakeComponent* snakeComponent) {
    if (snakeComponent->segmentPositions.size() <= 1) {
        return false;
    }

    size_t headX = snakeComponent->getHeadX();
    size_t headY = snakeComponent->getHeadY();

    for (size_t i = 3; i < snakeComponent->segmentPositions.size(); ++i) {
        size_t segX = snakeComponent->segmentPositions[i].first;
        size_t segY = snakeComponent->segmentPositions[i].second;

        if (headX == segX && headY == segY) {
            std::cout << "Snake collided with itself at position (" << headX << ", " << headY << ")" << std::endl;
            return true;
        }
    }
    return false;
}

}  // namespace Arcade
