/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Render System
*/

#include "Games/Minesweeper/System/RenderSystem.hpp"
#include "Shared/Interface/ECS/IEntity.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include <iostream>


void RenderSystem::update() {
    std::cout << "Marin render la pute" << std::endl;
    auto entities = _entityManager->getEntities();
    for (const auto& entity : entities) {
        // auto board = _componentManager->getAllComponents<Arcade::Minesweeper::Board>(entity);
        // if (board) {
        //     // Render the board
        //     std::cout << "Rendering board: " << board->getName() << std::endl;
        //     // Add rendering logic here
        // }
    }
}