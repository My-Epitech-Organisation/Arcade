// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game Logic System for Pacman
*/

#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include "Games/PacMan/System/GameLogic.hpp"
#include "Games/PacMan/System/PlayerLogic.hpp"
#include "Games/PacMan/System/GhostLogic.hpp"
#include "Games/PacMan/System/CollisionManager.hpp"
#include "Games/PacMan/System/GameStateManager.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Utils/AnimationDebugger.hpp"

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
                      << section << ": "
                      << (time / 100) << "ms/frame" << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
        _sectionTimings.clear();
    }
}

GameLogic::GameLogic(std::shared_ptr<Arcade::IComponentManager>
componentManager,
std::shared_ptr<Arcade::IEntityManager> entityManager,
const std::map<std::string, DrawableComponent>& assets)
: _componentManager(componentManager),
_entityManager(entityManager),
_assets(assets) {
    _playerLogic = std::make_unique<PlayerLogic>
        (componentManager, entityManager, assets);
    _ghostLogic = std::make_unique<GhostLogic>
        (componentManager, entityManager, assets);
    _collisionManager = std::make_unique<CollisionManager>
        (componentManager, entityManager, assets);
    _gameStateManager = std::make_unique<GameStateManager>
        (componentManager, entityManager, assets);
    _lastUpdateTime = std::chrono::high_resolution_clock::now();
    initializeEntityCache();
    _cachedGhosts.clear();
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
        if (ghostComp) {
            _cachedGhosts.push_back({entity, ghostComp});
        }
    }
    _cachedFoodEntities.clear();
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1003)));
        if (foodComp) {
            _cachedFoodEntities.push_back({entity, foodComp});
        }
    }
    const char* debugEnv = std::getenv("PACMAN_DEBUG");
    _debugMode = (debugEnv && std::string(debugEnv) == "1");
    if (_debugMode) {
        AnimationDebugger::dumpAssetKeys(_assets);
    }
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
    _currentDeltaTime = std::chrono::duration<float>(currentTime
        - _lastUpdateTime).count();
    _lastUpdateTime = currentTime;
}

float GameLogic::getDeltaTime() {
    return _currentDeltaTime;
}

void GameLogic::update() {
    _timer.start();
    updateDeltaTime();
    float deltaTime = _currentDeltaTime;
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
    if (pacman->isDead()) {
        grid->setGameOver(true);
    }
    bool gameOver = grid->isGameOver();
    _timer.start();
    if (pacman->isMoving()) {
        pacman->updateAnimation(deltaTime * 1.2f);
    } else {
        pacman->updateAnimation(deltaTime);
    }
    if (pacman->isMoving()) {
        pacman->updateVisualPosition(deltaTime);
        auto pacmanDrawable = std::dynamic_pointer_cast<IDrawableComponent>(
            _componentManager->getComponentByType(_cachedPacmanEntity,
                ComponentType::DRAWABLE));
        if (pacmanDrawable) {
            pacmanDrawable->setPosition(pacman->getVisualX(),
                pacman->getVisualY());
            updatePacmanSprite(pacmanDrawable, pacman);
            pacman->setDimensions(pacmanDrawable->getWidth(),
                pacmanDrawable->getHeight());
        }
    } else {
        static int staticUpdateCounter = 0;
        if (++staticUpdateCounter % 10 == 0) {
            auto pacmanDrawable = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(_cachedPacmanEntity,
                    ComponentType::DRAWABLE));
            if (pacmanDrawable) {
                updatePacmanSprite(pacmanDrawable, pacman);
            }
        }
    }
    for (auto& [entity, ghostComp] : _cachedGhosts) {
        if (ghostComp->isMoving()) {
            ghostComp->updateVisualPosition(deltaTime);
            auto ghostDrawable = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));
            if (ghostDrawable) {
                ghostDrawable->setPosition(ghostComp->getVisualX(),
                    ghostComp->getVisualY());
                ghostComp->setDimensions(ghostDrawable->getWidth(),
                    ghostDrawable->getHeight());
            }
        }
    }
    recordSectionTime("Visual Position Updates", _timer.stop());
    if (gameOver) {
        recordSectionTime("Total Frame Time", _timer.stop());
        reportPerformance();
        return;
    }
    _timer.start();
    pacman->updateMovementTimer(deltaTime);
    pacman->addGameTime(deltaTime);
    recordSectionTime("Pacman Timer", _timer.stop());
    if (pacman->canMove() && !pacman->isMoving()) {
        _timer.start();
        _playerLogic->movePacman(pacman, grid,
            _cachedPacmanEntity, _cachedGridEntity);
        recordSectionTime("Pacman Movement", _timer.stop());
    }
    static float ghostUpdateTimer = 0;
    ghostUpdateTimer += deltaTime;
    if (ghostUpdateTimer >= 0.1f) {
        _timer.start();
        _ghostLogic->updateGhostStates(0.1f);
        ghostUpdateTimer = 0;
        recordSectionTime("Ghost State Update", _timer.stop());
    }
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
    _timer.start();
    for (auto& [entity, ghostComp] : _cachedGhosts) {
        if (!ghostComp->canLeaveBox()) {
            ghostComp->updateReleaseTimer(deltaTime);
            continue;
        }
        ghostComp->updateMovementTimer(deltaTime);
        if (ghostComp->canMove() && !ghostComp->isMoving()) {
            if (ghostComp->getCurrentDirection() == Direction::NONE) {
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
    _timer.start();
    _playerLogic->tryChangePacmanDirection(pacman, grid);
    recordSectionTime("Pacman Direction", _timer.stop());
    _timer.start();
    _collisionManager->checkCollisions(pacman, grid,
        _cachedPacmanEntity, _cachedGridEntity);
    recordSectionTime("Collision Detection", _timer.stop());
    _timer.start();
    _gameStateManager->checkWinCondition(grid);
    recordSectionTime("Win Condition", _timer.stop());
    recordSectionTime("Total Frame Time", _timer.stop());
    reportPerformance();
}

void GameLogic::updatePacmanSprite(std::shared_ptr<IDrawableComponent>
pacmanDrawable,
std::shared_ptr<PacmanComponent> pacman) {
    if (!pacmanDrawable || !pacman)
        return;
    std::string assetKey = pacman->getDirectionalSprite();
    static std::string lastKey = "";
    static int missingKeyCounter = 0;
    bool keyMissing = false;
    auto asset = _assets.find(assetKey);
    if (asset == _assets.end()) {
        keyMissing = true;
        size_t firstDot = assetKey.find('.');
        size_t secondDot = assetKey.find('.', firstDot + 1);
        if (firstDot != std::string::npos && secondDot != std::string::npos) {
            std::string direction = assetKey.substr(firstDot + 1,
                secondDot - firstDot - 1);
            std::string frameStr = assetKey.substr(secondDot + 1);
            int frame = std::stoi(frameStr);
            std::vector<std::string> keyFormats = {
                "pacman." + direction + "." + std::to_string(frame),
                "pacman." + direction + "_" + std::to_string(frame),
                "textures.pacman." + direction + "." + std::to_string(frame-1),
                "pacman." + direction + "[" + std::to_string(frame-1) + "]"
            };
            for (const auto& format : keyFormats) {
                asset = _assets.find(format);
                if (asset != _assets.end()) {
                    if (_debugMode) {
                        std::cout
                            << "Found sprite with alternative key format: "
                            << format << std::endl;
                    }
                    assetKey = format;
                    keyMissing = false;
                    break;
                }
            }
        }
    }
    if (keyMissing && lastKey != assetKey) {
        lastKey = assetKey;
        missingKeyCounter++;
        if (_debugMode || missingKeyCounter % 10 == 1) {
            std::cout << "Sprite key not found: " << assetKey << std::endl;
            std::cout << "Available pacman keys:" << std::endl;
            for (const auto& [key, _] : _assets) {
                if (key.find("pacman") != std::string::npos) {
                    std::cout << " - " << key << std::endl;
                }
            }
        }
       asset = _assets.find("pacman.default");
    }
    if (asset != _assets.end()) {
        float x = pacmanDrawable->getPositionX();
        float y = pacmanDrawable->getPositionY();
        *pacmanDrawable = asset->second;
        pacmanDrawable->setPosition(x, y);
    }
}

void GameLogic::reloadCurrentMap() {
    std::cout << "Reloading PacMan game map (full reset)..." << std::endl;
    _gameStateManager->resetEntireGame();
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
