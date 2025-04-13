// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Ghost Logic for Pacman
*/

#ifndef SRC_GAMES_PACMAN_SYSTEM_GHOSTLOGIC_HPP_
#define SRC_GAMES_PACMAN_SYSTEM_GHOSTLOGIC_HPP_

#include <memory>
#include <map>
#include <string>
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Games/PacMan/Components/GhostComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace PacMan {

class GhostLogic {
 public:
    GhostLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
              std::shared_ptr<Arcade::IEntityManager> entityManager,
              const std::map<std::string, DrawableComponent>& assets);
    
    void moveGhost(std::shared_ptr<GhostComponent> ghostComp,
                  std::shared_ptr<IEntity> entity,
                  std::shared_ptr<GridComponent> grid,
                  std::shared_ptr<PacmanComponent> pacman);
                  
    void updateGhostStates(float deltaTime);
    
    std::shared_ptr<IDrawableComponent> getDrawableAsset(const std::string& key) const;
    
 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    const std::map<std::string, DrawableComponent>& _assets;
    
    // Add helper method to reset ghost appearance
    void resetGhostAppearance(std::shared_ptr<IEntity> entity, 
                             std::shared_ptr<GhostComponent> ghostComp);
    
    // Add emergency fallback method
    void emergencyGhostReset(std::shared_ptr<IEntity> entity, 
                           std::shared_ptr<GhostComponent> ghostComp);
                             
    // Add helper method to set ghost to eyes appearance
    void setGhostToEyesAppearance(std::shared_ptr<IEntity> entity,
                                 std::shared_ptr<GhostComponent> ghostComp);
                             
    // Add helper to find grid entity
    std::shared_ptr<IEntity> findGridEntity();
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_SYSTEM_GHOSTLOGIC_HPP_
