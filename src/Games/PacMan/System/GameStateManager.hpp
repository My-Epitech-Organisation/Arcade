// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game State Manager for Pacman
*/

#ifndef SRC_GAMES_PACMAN_SYSTEM_GAMESTATEMANAGER_HPP_
#define SRC_GAMES_PACMAN_SYSTEM_GAMESTATEMANAGER_HPP_

#include <memory>
#include <map>
#include <string>
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace PacMan {

class GameStateManager {
 public:
    GameStateManager(std::shared_ptr<Arcade::IComponentManager> componentManager,
                    std::shared_ptr<Arcade::IEntityManager> entityManager,
                    const std::map<std::string, DrawableComponent>& assets);
    
    void checkWinCondition(std::shared_ptr<GridComponent> grid);
    void reloadCurrentMap();
    void resetPositions(); // New method to reset entity positions
    void increaseGameSpeed();
    std::shared_ptr<IDrawableComponent> getDrawableAsset(const std::string& key) const;
    
 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    const std::map<std::string, DrawableComponent>& _assets;
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_SYSTEM_GAMESTATEMANAGER_HPP_
