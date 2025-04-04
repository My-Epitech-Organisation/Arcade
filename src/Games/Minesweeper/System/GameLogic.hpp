// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GameLogic
*/

#ifndef SRC_GAMES_MINESWEEPER_SYSTEM_GAMELOGIC_HPP_
    #define SRC_GAMES_MINESWEEPER_SYSTEM_GAMELOGIC_HPP_
    #include <memory>
    #include "Shared/Interface/ECS/IComponentManager.hpp"
    #include "Shared/Interface/ECS/IEntityManager.hpp"
    #include "Shared/Interface/ECS/ISystem.hpp"

class GameLogic : public Arcade::ISystem {
 public:
    GameLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
        std::shared_ptr<Arcade::IEntityManager> entityManager)
        : _componentManager(componentManager), _entityManager(entityManager) {}
    void update() override;

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
};

#endif  // SRC_GAMES_MINESWEEPER_SYSTEM_GAMELOGIC_HPP_
