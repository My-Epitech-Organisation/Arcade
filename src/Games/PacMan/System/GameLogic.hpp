// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game Logic System for Pacman
*/

#ifndef SRC_GAMES_PACMAN_SYSTEM_GAMELOGIC_HPP_
#define SRC_GAMES_PACMAN_SYSTEM_GAMELOGIC_HPP_

#include <memory>
#include <map>
#include <chrono>
#include <string>
#include <utility>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GhostComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace PacMan {

class GameLogic : public Arcade::ISystem {
 public:
      GameLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
               std::shared_ptr<Arcade::IEntityManager> entityManager,
               const std::map<std::string, DrawableComponent>& assets);
      void update() override;
      void reloadCurrentMap();

 private:
      std::shared_ptr<IEntity> findPacmanEntity();
      std::shared_ptr<IEntity> findGridEntity();
      std::pair<std::shared_ptr<PacmanComponent>,
         std::shared_ptr<GridComponent>>
      getPacmanAndGridComponents();
      void checkFoodCollision(std::shared_ptr<PacmanComponent> pacman,
         std::shared_ptr<GridComponent> grid);
      std::shared_ptr<Arcade::IComponentManager> _componentManager;
      std::shared_ptr<Arcade::IEntityManager> _entityManager;
      std::chrono::high_resolution_clock::time_point _lastUpdateTime;
      const std::map<std::string, DrawableComponent>& _assets;
      std::shared_ptr<DrawableComponent>
         getDrawableAsset(const std::string& key) const;

      void movePacman();
      void checkCollisions();
      void increaseGameSpeed();
      void checkCollisionsWithGhosts(std::shared_ptr<PacmanComponent> pacman,
                                    std::shared_ptr<GridComponent> grid);
      void checkCollisionsWithFood(std::shared_ptr<PacmanComponent> pacman,
                                 std::shared_ptr<GridComponent> grid);
      void checkWinCondition(std::shared_ptr<GridComponent> grid);
      float getDeltaTime();
      void tryChangePacmanDirection(std::shared_ptr<PacmanComponent> pacman,
                                 std::shared_ptr<GridComponent> grid);
      bool canMoveInDirection(Direction dir, size_t x, size_t y,
                           std::shared_ptr<GridComponent> grid);
      float _currentDeltaTime = 0.0f;
      void updateDeltaTime();
      void moveGhost(std::shared_ptr<GhostComponent> ghostComp,
         std::shared_ptr<IEntity> entity,
         std::shared_ptr<GridComponent> grid,
         std::shared_ptr<PacmanComponent> pacman);

      std::shared_ptr<IEntity> _cachedPacmanEntity = 0;
      std::shared_ptr<IEntity> _cachedGridEntity = 0;
      std::shared_ptr<PacmanComponent> _cachedPacmanComponent = nullptr;
      std::shared_ptr<GridComponent> _cachedGridComponent = nullptr;
      // Function to initialize the cached entities
      void initializeEntityCache();
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_SYSTEM_GAMELOGIC_HPP_
