// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Render System
*/

#include <iostream>
#include "Games/Minesweeper/System/RenderSystem.hpp"
#include "Shared/Interface/ECS/IEntity.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"


void RenderSystem::update() {
    std::cout << "Marin render la pute" << std::endl;
    auto entities = _entityManager->getEntities();
    for (const auto& entity : entities) {}
}
