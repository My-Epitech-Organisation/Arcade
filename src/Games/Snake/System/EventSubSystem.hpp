// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Event management system for Snake game
*/

#ifndef SRC_GAMES_SNAKE_SYSTEM_EVENTSUBSYSTEM_HPP_
#define SRC_GAMES_SNAKE_SYSTEM_EVENTSUBSYSTEM_HPP_

#include <functional>
#include <memory>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"

namespace Arcade {

class EventSubSystem : public ISystem {
 public:
    EventSubSystem(std::shared_ptr<IEventManager> eventManager,
                  std::shared_ptr<IComponentManager> componentManager,
                  std::shared_ptr<IEntityManager> entityManager);
    ~EventSubSystem() override;

    void update() override;

 private:
    void handleKeyPressed(const Event &event);

    std::shared_ptr<IEventManager> _eventManager;
    std::shared_ptr<IComponentManager> _componentManager;
    std::shared_ptr<IEntityManager> _entityManager;
    std::function<void(const Event&)> _keyPressHandler;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SYSTEM_EVENTSUBSYSTEM_HPP_
