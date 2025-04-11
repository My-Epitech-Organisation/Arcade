// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game logic system for Snake game
*/

#ifndef SRC_GAMES_SNAKE_SYSTEM_GAMELOGIC_HPP_
#define SRC_GAMES_SNAKE_SYSTEM_GAMELOGIC_HPP_

#include <memory>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"

namespace Arcade {

class GameLogic : public ISystem {
 public:
    GameLogic(std::shared_ptr<IComponentManager> componentManager,
             std::shared_ptr<IEntityManager> entityManager);
    ~GameLogic() override = default;

    void update() override;

 private:
    void checkVictoryCondition();

    std::shared_ptr<IComponentManager> _componentManager;
    std::shared_ptr<IEntityManager> _entityManager;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SYSTEM_GAMELOGIC_HPP_
