// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Collision System for Snake game
*/

#ifndef SRC_GAMES_SNAKE_SYSTEM_COLLISIONSYSTEM_HPP_
#define SRC_GAMES_SNAKE_SYSTEM_COLLISIONSYSTEM_HPP_

#include <memory>
#include <vector>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"

namespace Arcade {

class CollisionSystem : public ISystem {
 public:
    CollisionSystem(
        std::shared_ptr<IComponentManager> componentManager,
        std::shared_ptr<IEntityManager> entityManager);
    ~CollisionSystem() = default;

    void update() override;

    bool hasCollision() const { return _collision; }

 private:
    std::shared_ptr<IComponentManager> _componentManager;
    std::shared_ptr<IEntityManager> _entityManager;
    bool _collision;

    bool checkBoundaryCollision(float x, float y);
    bool checkSelfCollision(const std::vector<Entity>& segments);
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SYSTEM_COLLISIONSYSTEM_HPP_
