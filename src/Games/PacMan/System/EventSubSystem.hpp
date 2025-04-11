// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Event Subsystem for Pacman
*/

#ifndef SRC_GAMES_PACMAN_SYSTEM_EVENTSUBSYSTEM_HPP_
#define SRC_GAMES_PACMAN_SYSTEM_EVENTSUBSYSTEM_HPP_

#include <memory>
#include <utility>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"

namespace Arcade {
namespace PacMan {

class EventSubSystem : public Arcade::ISystem {
 public:
    EventSubSystem(std::shared_ptr<Arcade::IComponentManager> componentManager,
                  std::shared_ptr<Arcade::IEntityManager> entityManager,
                  std::shared_ptr<Arcade::IEventManager> eventManager);
    ~EventSubSystem() override;
    void update() override;
    void subscribeToEvents();

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    std::shared_ptr<Arcade::IEventManager> _eventManager;
    void handleKeyUp();
    void handleKeyDown();
    void handleKeyLeft();
    void handleKeyRight();
    void handleKeyR();
    Direction validateDirection(Direction requestedDir, size_t x, size_t y,
                               std::shared_ptr<GridComponent> grid);
    Arcade::Entity findPacmanEntity() const;
    std::pair<std::shared_ptr<PacmanComponent>,
        std::shared_ptr<GridComponent>>
        getPacmanAndGridComponents();
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_SYSTEM_EVENTSUBSYSTEM_HPP_
