// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game Logic System for Pacman
*/

#include <iostream>
#include <chrono>
#include <memory>
#include "Games/PacMan/System/GameLogic.hpp"
#include "Games/PacMan/System/PlayerLogic.hpp"
#include "Games/PacMan/System/GhostLogic.hpp"
#include "Games/PacMan/System/CollisionManager.hpp"
#include "Games/PacMan/System/GameStateManager.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"

namespace Arcade {
namespace PacMan {

void GameLogic::recordSectionTime(const std::string& section, double time) {
    _sectionTimings[section] += time;
}

void GameLogic::reportPerformance() {
    _frameCounter++;
    
    if (_frameCounter % 100 == 0 && _debugMode) {
        std::cout << "----- PERFORMANCE REPORT (100 frames) -----" << std::endl;
        for (const auto& [section, time] : _sectionTimings) {
            std::cout << std::fixed << std::setprecision(3)
                      << section << ": " << (time / 100) << "ms/frame" << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
        _sectionTimings.clear();
    }
}

GameLogic::GameLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
                    std::shared_ptr<Arcade::IEntityManager> entityManager,
                    const std::map<std::string, DrawableComponent>& assets)
    : _componentManager(componentManager), _entityManager(entityManager), _assets(assets) {
    
    // Initialize subsystems
    _playerLogic = std::make_unique<PlayerLogic>(componentManager, entityManager, assets);
    _ghostLogic = std::make_unique<GhostLogic>(componentManager, entityManager, assets);
    _collisionManager = std::make_unique<CollisionManager>(componentManager, entityManager, assets);
    _gameStateManager = std::make_unique<GameStateManager>(componentManager, entityManager, assets);
    
    _lastUpdateTime = std::chrono::high_resolution_clock::now();
    initializeEntityCache();
    
    // Pre-cache ghost entities
    _cachedGhosts.clear();
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
        if (ghostComp) {
            _cachedGhosts.push_back({entity, ghostComp});
        }
    }
    
    // Pre-cache food entities
    _cachedFoodEntities.clear();
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1003)));
        if (foodComp) {
            _cachedFoodEntities.push_back({entity, foodComp});
        }
    }
    
    // Enable debug mode through environment variable
    const char* debugEnv = std::getenv("PACMAN_DEBUG");
    _debugMode = (debugEnv && std::string(debugEnv) == "1");
}

void GameLogic::initializeEntityCache() {
    if (!_cachedPacmanEntity) {
        _cachedPacmanEntity = findPacmanEntity();
    }
    if (!_cachedGridEntity) {
        _cachedGridEntity = findGridEntity();
    }
    if (_cachedPacmanEntity && !_cachedPacmanComponent) {
        _cachedPacmanComponent = std::dynamic_pointer_cast<PacmanComponent>(
            _componentManager->getComponentByType(_cachedPacmanEntity,
                static_cast<ComponentType>(1001)));
    }
    if (_cachedGridEntity && !_cachedGridComponent) {
        _cachedGridComponent = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(_cachedGridEntity,
                static_cast<ComponentType>(1000)));
    }
}

std::shared_ptr<Arcade::IEntity> GameLogic::findPacmanEntity() {
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto pacmanComp = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1001));
        if (pacmanComp)
            return entity;
    }
    return nullptr;
}

std::shared_ptr<Arcade::IEntity> GameLogic::findGridEntity() {
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto gridComp = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1000));
        if (gridComp)
            return entity;
    }
    return nullptr;
}

void GameLogic::updateDeltaTime() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    _currentDeltaTime = std::chrono::duration<float>(currentTime - _lastUpdateTime).count();
    _lastUpdateTime = currentTime;
}

float GameLogic::getDeltaTime() {
    return _currentDeltaTime;
}

void GameLogic::update() {
    // Start overall timing
    _timer.start();
    
    // Update delta time only once per frame
    updateDeltaTime();
    float deltaTime = _currentDeltaTime;
    
    // Cache initialization (if needed)
    if (!_cachedPacmanComponent || !_cachedGridComponent) {
        _timer.start();
        initializeEntityCache();
        recordSectionTime("Cache Initialization", _timer.stop());
    }

    if (!_cachedPacmanEntity || !_cachedGridEntity) {
        return;
    }
    
    auto pacman = _cachedPacmanComponent;
    auto grid = _cachedGridComponent;

    if (!pacman || !grid)
        return;

    // Check for game over conditions
    if (pacman->isDead()) {
        grid->setGameOver(true);
    }
    
    // If game is over, we still render but don't update game state
    bool gameOver = grid->isGameOver();
    
    // Update visual positions even when game is over for smooth transitions
    _timer.start();
    
    // Optimize snap distance for faster movement
    const float snapDistance = 1.0f;
    
    // Update Pacman's visual position
    if (pacman->isMoving()) {
        pacman->updateVisualPosition(deltaTime);
        
        // Update the DrawableComponent position to match visual position
        auto pacmanDrawable = std::dynamic_pointer_cast<IDrawableComponent>(
            _componentManager->getComponentByType(_cachedPacmanEntity,
                ComponentType::DRAWABLE));
        if (pacmanDrawable) {
            pacmanDrawable->setPosition(pacman->getVisualX(), pacman->getVisualY());
            
            // Also update dimensions for collision detection
            pacman->setDimensions(pacmanDrawable->getWidth(), pacmanDrawable->getHeight());
        }
    }
    
    // Update Ghost's visual positions
    for (auto& [entity, ghostComp] : _cachedGhosts) {
        if (ghostComp->isMoving()) {
            ghostComp->updateVisualPosition(deltaTime);
            
            // Update the DrawableComponent position to match visual position
            auto ghostDrawable = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));
            if (ghostDrawable) {
                ghostDrawable->setPosition(ghostComp->getVisualX(), ghostComp->getVisualY());
                
                // Also update dimensions for collision detection
                ghostComp->setDimensions(ghostDrawable->getWidth(), ghostDrawable->getHeight());
            }
        }
    }
    
    recordSectionTime("Visual Position Updates", _timer.stop());
    
    // Stop updating game logic if game is over
    if (gameOver) {
        // Report performance statistics
        recordSectionTime("Total Frame Time", _timer.stop());
        reportPerformance();
        return;
    }
    
    // Update Pacman movement timer (this is lightweight)
    _timer.start();
    pacman->updateMovementTimer(deltaTime);
    pacman->addGameTime(deltaTime);
    recordSectionTime("Pacman Timer", _timer.stop());

    // Only process movement if pacman can actually move and is not currently in transit
    if (pacman->canMove() && !pacman->isMoving()) {
        _timer.start();
        _playerLogic->movePacman(pacman, grid, _cachedPacmanEntity, _cachedGridEntity);
        recordSectionTime("Pacman Movement", _timer.stop());
    }

    // Update ghost states only when needed
    static float ghostUpdateTimer = 0;
    ghostUpdateTimer += deltaTime;
    if (ghostUpdateTimer >= 0.1f) { // Update ghost states less frequently
        _timer.start();
        _ghostLogic->updateGhostStates(deltaTime * 10); // Compensate for the reduced frequency
        ghostUpdateTimer = 0;
        recordSectionTime("Ghost State Update", _timer.stop());
    }
    
    // Refresh ghost cache periodically
    _ghostCacheRefreshTimer += deltaTime;
    if (_ghostCacheRefreshTimer >= 2.0f || _cachedGhosts.empty()) {
        _timer.start();
        _cachedGhosts.clear();
        for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
            auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
                _componentManager->getComponentByType(entity,
                    static_cast<ComponentType>(1002)));
                    
            if (ghostComp) {
                _cachedGhosts.push_back({entity, ghostComp});
            }
        }
        _ghostCacheRefreshTimer = 0;
        recordSectionTime("Ghost Cache Refresh", _timer.stop());
    }
    
    // Process ghost movements with cached entities
    _timer.start();
    for (auto& [entity, ghostComp] : _cachedGhosts) {
        // Skip ghosts that can't leave their box yet
        if (!ghostComp->canLeaveBox()) {
            ghostComp->updateReleaseTimer(deltaTime);
            continue;
        }
        
        // Update ghost movement timers
        ghostComp->updateMovementTimer(deltaTime);
        
        // Move ghosts that are ready and not currently in transit
        if (ghostComp->canMove() && !ghostComp->isMoving()) {
            if (ghostComp->getCurrentDirection() == Direction::NONE) {
                // Choose random initial direction if needed
                unsigned int seed = static_cast<unsigned int>(
                    std::time(nullptr) + 
                    reinterpret_cast<std::uintptr_t>(entity.get()));
                Direction dirs[] = {Direction::UP, Direction::DOWN,
                    Direction::LEFT, Direction::RIGHT};
                ghostComp->setCurrentDirection(dirs[rand_r(&seed) % 4]);
            }
            _ghostLogic->moveGhost(ghostComp, entity, grid, pacman);
        }
    }
    recordSectionTime("Ghost Movement", _timer.stop());

    // Handle direction changes for Pacman
    _timer.start();
    _playerLogic->tryChangePacmanDirection(pacman, grid);
    recordSectionTime("Pacman Direction", _timer.stop());
    
    // Check for collisions
    _timer.start();
    _collisionManager->checkCollisions(pacman, grid, _cachedPacmanEntity, _cachedGridEntity);
    recordSectionTime("Collision Detection", _timer.stop());
    
    // Check win condition
    _timer.start();
    _gameStateManager->checkWinCondition(grid);
    recordSectionTime("Win Condition", _timer.stop());
    
    // Report performance statistics
    recordSectionTime("Total Frame Time", _timer.stop());
    reportPerformance();
}

void GameLogic::reloadCurrentMap() {
    _gameStateManager->reloadCurrentMap();
    
    // After reloading, clear all caches
    _cachedPacmanEntity = nullptr;
    _cachedGridEntity = nullptr;
    _cachedPacmanComponent = nullptr;
    _cachedGridComponent = nullptr;
    _cachedGhosts.clear();
    _cachedFoodEntities.clear();
    _ghostCacheRefreshTimer = 0.0f;
    _foodCacheRefreshTimer = 0.0f;
    
    initializeEntityCache();
}

}  // namespace PacMan
}  // namespace Arcade
