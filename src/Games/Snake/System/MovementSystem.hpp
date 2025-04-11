// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Movement system for Snake game
*/

#ifndef SRC_GAMES_SNAKE_SYSTEM_MOVEMENTSYSTEM_HPP_
#define SRC_GAMES_SNAKE_SYSTEM_MOVEMENTSYSTEM_HPP_

#include <memory>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"

namespace Arcade {

class SnakeGame;  // Forward declaration

class MovementSystem : public ISystem {
 public:
    MovementSystem(std::shared_ptr<IComponentManager> componentManager,
                  std::shared_ptr<IEntityManager> entityManager,
                  SnakeGame* gameRef);
    ~MovementSystem() override = default;

    void update() override;

 private:
    void moveSnake();
    bool checkCollision(int x, int y);
    void handleFoodCollision(int x, int y);
    void updateGameState(bool collision);

    std::shared_ptr<IComponentManager> _componentManager;
    std::shared_ptr<IEntityManager> _entityManager;
    SnakeGame* _gameRef;
    float _timeSinceLastMove;
    const float _moveInterval = 0.2f;  // Time between moves in seconds
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SYSTEM_MOVEMENTSYSTEM_HPP_
