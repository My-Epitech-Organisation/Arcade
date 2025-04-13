// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Player Logic for Pacman
*/

#ifndef SRC_GAMES_PACMAN_SYSTEM_PLAYERLOGIC_HPP_
#define SRC_GAMES_PACMAN_SYSTEM_PLAYERLOGIC_HPP_

#include <memory>
#include <utility>
#include <map>
#include <string>
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace PacMan {

class PlayerLogic {
 public:
    PlayerLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
               std::shared_ptr<Arcade::IEntityManager> entityManager,
               const std::map<std::string, DrawableComponent>& assets);
    void movePacman(std::shared_ptr<PacmanComponent> pacman,
                   std::shared_ptr<GridComponent> grid,
                   std::shared_ptr<IEntity> pacmanEntity,
                   std::shared_ptr<IEntity> gridEntity);
    void tryChangePacmanDirection(std::shared_ptr<PacmanComponent> pacman,
                                std::shared_ptr<GridComponent> grid);
    bool canMoveInDirection(Direction dir, size_t x, size_t y,
                          std::shared_ptr<GridComponent> grid);

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    const std::map<std::string, DrawableComponent>& _assets;
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_SYSTEM_PLAYERLOGIC_HPP_
