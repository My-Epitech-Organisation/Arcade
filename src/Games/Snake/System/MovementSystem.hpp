// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Movement System for Snake game
*/

#ifndef SRC_GAMES_SNAKE_SYSTEM_MOVEMENTSYSTEM_HPP_
#define SRC_GAMES_SNAKE_SYSTEM_MOVEMENTSYSTEM_HPP_

#include <memory>
#include <chrono>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"

namespace Arcade {

class MovementSystem : public ISystem {
 public:
    MovementSystem(
        std::shared_ptr<IComponentManager> componentManager,
        std::shared_ptr<IEntityManager> entityManager);
    ~MovementSystem() = default;

    void update() override;

 private:
    std::shared_ptr<IComponentManager> _componentManager;
    std::shared_ptr<IEntityManager> _entityManager;

    std::chrono::time_point<std::chrono::high_resolution_clock> _lastUpdateTime;
    float _movementDelay = 0.2f;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SYSTEM_MOVEMENTSYSTEM_HPP_
