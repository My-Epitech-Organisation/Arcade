// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Event System for Snake game
*/

#ifndef SRC_GAMES_SNAKE_SYSTEM_EVENTSYSTEM_HPP_
#define SRC_GAMES_SNAKE_SYSTEM_EVENTSYSTEM_HPP_

#include <memory>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Games/Snake/Types.hpp"

namespace Arcade {

class EventSystem : public ISystem {
 public:
    EventSystem(
        std::shared_ptr<IEventManager> eventManager,
        std::shared_ptr<IComponentManager> componentManager,
        std::shared_ptr<IEntityManager> entityManager);
    ~EventSystem() override;

    void update() override;
    void subscribeToEvents();

 private:
    void handleKeyUp();
    void handleKeyDown();
    void handleKeyLeft();
    void handleKeyRight();
    void handleKeySpace(bool pressed);
    void handleKeyR();

    std::shared_ptr<IEventManager> _eventManager;
    std::shared_ptr<IComponentManager> _componentManager;
    std::shared_ptr<IEntityManager> _entityManager;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SYSTEM_EVENTSYSTEM_HPP_
