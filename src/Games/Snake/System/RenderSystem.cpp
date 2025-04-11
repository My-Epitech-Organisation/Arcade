// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Render System for Snake game implementation
*/

#include <memory>
#include "Games/Snake/System/RenderSystem.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Games/Snake/Components/Food.hpp"

namespace Arcade {

RenderSystem::RenderSystem(
std::shared_ptr<IComponentManager> componentManager,
std::shared_ptr<IEntityManager> entityManager)
: _componentManager(componentManager),
_entityManager(entityManager) {}

void RenderSystem::update() {
    auto entities = _entityManager->getEntities();
    for (const auto& entity : entities) {}
}

}  // namespace Arcade
