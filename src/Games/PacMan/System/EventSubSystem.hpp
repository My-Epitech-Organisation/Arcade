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
#include <string>
#include <map>
#include <utility>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace PacMan {

class EventSubSystem : public Arcade::ISystem,
                      public std::enable_shared_from_this<EventSubSystem> {
 public:
    EventSubSystem(std::shared_ptr<Arcade::IComponentManager> componentManager,
                  std::shared_ptr<Arcade::IEntityManager> entityManager,
                  std::shared_ptr<Arcade::IEventManager> eventManager,
                  const std::map<std::string, Arcade::DrawableComponent>&
                     drawableAssets);
    ~EventSubSystem() override;
    void update() override;
    void subscribeToEvents();
    bool areEventsSubscribed() const;
    void subscribeToEscKey();

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    std::shared_ptr<Arcade::IEventManager> _eventManager;
    const std::map<std::string, Arcade::DrawableComponent>& _drawableAssets;
    void handleKeyUp();
    void handleKeyDown();
    void handleKeyLeft();
    void handleKeyRight();
    void handleKeyR();
    void handleKeyEsc();
    Direction validateDirection(Direction requestedDir, size_t x, size_t y,
                               std::shared_ptr<GridComponent> grid);
    std::shared_ptr<Arcade::IEntity> findPacmanEntity() const;
    std::pair<std::shared_ptr<PacmanComponent>,
        std::shared_ptr<GridComponent>>
        getPacmanAndGridComponents();
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_SYSTEM_EVENTSUBSYSTEM_HPP_
