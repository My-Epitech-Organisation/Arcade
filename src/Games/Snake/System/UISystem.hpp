// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** UI system for Snake game
*/

#ifndef SRC_GAMES_SNAKE_SYSTEM_UISYSTEM_HPP_
#define SRC_GAMES_SNAKE_SYSTEM_UISYSTEM_HPP_

#include <memory>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"

namespace Arcade {

class UISystem : public ISystem {
 public:
    UISystem(std::shared_ptr<IComponentManager> componentManager,
            std::shared_ptr<IEntityManager> entityManager);
    ~UISystem() override = default;

    void update() override;

 private:
    void updateScoreDisplay();
    void updateTimeDisplay();

    std::shared_ptr<IComponentManager> _componentManager;
    std::shared_ptr<IEntityManager> _entityManager;
    Entity _scoreTextEntity;
    Entity _timeTextEntity;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SYSTEM_UISYSTEM_HPP_
