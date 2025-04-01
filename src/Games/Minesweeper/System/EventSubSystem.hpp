// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Event Subsystem for Minesweeper
*/

#ifndef SRC_GAMES_MINESWEEPER_SYSTEM_EVENTSUBSYSTEM_HPP_
    #define SRC_GAMES_MINESWEEPER_SYSTEM_EVENTSUBSYSTEM_HPP_
#include <memory>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"

namespace Arcade {
class EventSubSystem : public Arcade::ISystem {
 public:
    EventSubSystem(std::shared_ptr<Arcade::IComponentManager> componentManager,
        std::shared_ptr<Arcade::IEntityManager> entityManager,
        std::shared_ptr<Arcade::IEventManager> eventManager);
    ~EventSubSystem() = default;
    void update() override;
    void subscribeToEvents();

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    std::shared_ptr<Arcade::IEventManager> _eventManager;
    void handleLeftClick();
    void handleRightClick();
    void handleKeyR();
};
}  // namespace Arcade

#endif  // SRC_GAMES_MINESWEEPER_SYSTEM_EVENTSUBSYSTEM_HPP_
