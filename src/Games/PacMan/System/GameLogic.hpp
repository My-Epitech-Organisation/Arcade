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
#include <vector>
#include <iostream>
#include <iomanip>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "Games/PacMan/System/PlayerLogic.hpp"
#include "Games/PacMan/System/GhostLogic.hpp"
#include "Games/PacMan/System/CollisionManager.hpp"
#include "Games/PacMan/System/GameStateManager.hpp"

namespace Arcade {
namespace PacMan {

class PerformanceTimer {
 public:
    void start() {
        _startTime = std::chrono::high_resolution_clock::now();
    }
    double stop() {
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = endTime
            - _startTime;
        return elapsed.count();
    }
 private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
};

class GameLogic : public Arcade::ISystem {
 public:
      GameLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
               std::shared_ptr<Arcade::IEntityManager> entityManager,
               const std::map<std::string, DrawableComponent>& assets);
      void update() override;
      void reloadCurrentMap();

 private:
      std::shared_ptr<Arcade::IComponentManager> _componentManager;
      std::shared_ptr<Arcade::IEntityManager> _entityManager;
      const std::map<std::string, DrawableComponent>& _assets;
      std::unique_ptr<PlayerLogic> _playerLogic;
      std::unique_ptr<GhostLogic> _ghostLogic;
      std::unique_ptr<CollisionManager> _collisionManager;
      std::unique_ptr<GameStateManager> _gameStateManager;
      std::chrono::high_resolution_clock::time_point _lastUpdateTime;
      float _currentDeltaTime = 0.0f;
      void updateDeltaTime();
      float getDeltaTime();
      std::shared_ptr<IEntity> _cachedPacmanEntity = 0;
      std::shared_ptr<IEntity> _cachedGridEntity = 0;
      std::shared_ptr<PacmanComponent> _cachedPacmanComponent = nullptr;
      std::shared_ptr<GridComponent> _cachedGridComponent = nullptr;
      void initializeEntityCache();
      std::shared_ptr<IEntity> findPacmanEntity();
      std::shared_ptr<IEntity> findGridEntity();
      PerformanceTimer _timer;
      std::map<std::string, double> _sectionTimings;
      int _frameCounter = 0;
      void recordSectionTime(const std::string& section, double time);
      void reportPerformance();
      std::vector<std::pair<std::shared_ptr<IEntity>,
        std::shared_ptr<GhostComponent>>> _cachedGhosts;
      float _ghostCacheRefreshTimer = 0.0f;
      std::vector<std::pair<std::shared_ptr<IEntity>,
        std::shared_ptr<FoodComponent>>> _cachedFoodEntities;
      float _foodCacheRefreshTimer = 0.0f;
      bool _debugMode = false;
      void updatePacmanSprite(
        std::shared_ptr<IDrawableComponent> pacmanDrawable,
        std::shared_ptr<PacmanComponent> pacman);
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_SYSTEM_GAMELOGIC_HPP_
