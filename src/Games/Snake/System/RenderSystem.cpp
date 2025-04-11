// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Render system implementation for Snake game
*/

#include "Games/Snake/System/RenderSystem.hpp"
#include "Games/Snake/Components/Cell.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Games/Snake/Components/Food.hpp"
#include "ECS/Components/Position.hpp"
#include "ECS/Components/Sprite.hpp"

namespace Arcade {

RenderSystem::RenderSystem(std::shared_ptr<IComponentManager> componentManager,
                         std::shared_ptr<IEntityManager> entityManager)
    : _componentManager(componentManager),
      _entityManager(entityManager) {
}

void RenderSystem::update() {
    // All rendering is handled by the core's RenderSystem
    // This system just ensures that all entities have the correct visual representation

    // The snake's segments are rendered with the correct sprites depending on their position
    // This has been handled in the updateSnakeVisuals method in the SnakeGame class

    // Food entities are rendered with the correct sprites
    auto foodEntities = _entityManager->getEntitiesWithComponent<Food>();
    for (auto entity : foodEntities) {
        auto food = _componentManager->getComponent<Food>(entity);
        auto sprite = _componentManager->getComponent<Sprite>(entity);

        if (sprite) {
            sprite->specialCompId = (food->type == Food::APPLE) ? Food::APPLE : Food::BONUS;
        }
    }
}

}  // namespace Arcade
