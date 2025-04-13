// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Pacman game module implementation
*/

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include "Games/PacMan/Pacman.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GhostComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "Games/PacMan/Components/FoodComponent.hpp"
#include "Games/PacMan/System/UISystem.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "Shared/Models/ModuleInfos.hpp"
#include "Shared/JSONParser/JSONParser.hpp"

namespace Arcade {
namespace PacMan {

// Add this new class for performance monitoring
class PerformanceMonitor {
 public:
    void start() {
        _startTime = std::chrono::high_resolution_clock::now();
    }
    double stop() {
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed
            = endTime - _startTime;
        return elapsed.count();
    }
 private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
};

// Create a static instance for reuse
static PerformanceMonitor s_perfMonitor;

PacmanGame::~PacmanGame() {
    _systems.clear();
    _eventSystem.reset();

    if (_entityManager && _componentManager) {
        auto entities = _entityManager->getEntitiesMap();
        for (const auto& entity : entities) {
            auto components = _componentManager->getEntityComponents(
                entity.first);
            for (const auto& component : components) {
                _componentManager->unregisterComponent(entity.first,
                    typeid(*component).name());
            }

            _entityManager->destroyEntity(entity.first);
        }
    }

    _componentManager.reset();
    _entityManager.reset();
    _eventManager.reset();
}

void PacmanGame::init(std::shared_ptr<IEventManager> eventManager,
std::shared_ptr<IComponentManager> componentManager,
std::shared_ptr<IEntityManager> entityManager,
std::shared_ptr<IScoreProvider> scoreProvider) {
    s_perfMonitor.start();
    _gameOver = false;
    _gameWon = false;
    _eventManager = eventManager;
    _componentManager = componentManager;
    _entityManager = entityManager;
    _scoreProvider = scoreProvider;
    double startLoadTime = s_perfMonitor.stop();
    s_perfMonitor.start();
    loadDrawableAssets();
    double assetLoadTime = s_perfMonitor.stop();
    s_perfMonitor.start();
    createGame();
    double gameCreateTime = s_perfMonitor.stop();
    s_perfMonitor.start();
    _eventSystem = std::make_shared<EventSubSystem>(
        _componentManager, _entityManager, _eventManager, _drawableAssets);
    _systems.push_back(std::make_shared<GameLogic>(_componentManager,
        _entityManager, _drawableAssets));
    _systems.push_back(std::make_shared<UISystem>(_componentManager,
        _entityManager, _drawableAssets));
    _systems.push_back(_eventSystem);
    double systemInitTime = s_perfMonitor.stop();
    const char* debugEnv = std::getenv("PACMAN_DEBUG");
    bool debugMode = (debugEnv && std::string(debugEnv) == "1");
    std::cerr << "DEBUG mode is " << (debugMode ? "ON" : "OFF") << std::endl;
    if (debugMode) {
        std::cout << "--- PacMan Initialization Times ---" << std::endl;
        std::cout << "Setup: " << startLoadTime << "ms" << std::endl;
        std::cout << "Asset Loading: " << assetLoadTime << "ms" << std::endl;
        std::cout << "Game Creation: " << gameCreateTime << "ms" << std::endl;
        std::cout << "System Init: " << systemInitTime << "ms" << std::endl;
        std::cout << "Total Init: " << (startLoadTime
            + assetLoadTime + gameCreateTime + systemInitTime)
            << "ms" << std::endl;
    }
}

void PacmanGame::createGame() {
    PacmanFactory factory(_entityManager, _componentManager, _drawableAssets);
    factory.initializeGame(16.0f);
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Pacman") {
            auto pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
                _componentManager->getComponentByType(entity,
                    static_cast<ComponentType>(1001)));
            if (pacmanComp) {
                pacmanComp->setMovementThreshold(0.04f);
                pacmanComp->setMovementSpeed(30.0f);
            }
        }
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
        if (ghostComp) {
            ghostComp->setMovementThreshold(0.06f);
            ghostComp->setMovementSpeed(28.0f);
            switch (ghostComp->getGhostType()) {
                case GhostType::RED:
                    ghostComp->setMovementSpeed(30.0f);
                    break;
                case GhostType::PINK:
                    ghostComp->setMovementSpeed(29.0f);
                    break;
                case GhostType::BLUE:
                    ghostComp->setMovementSpeed(27.0f);
                    break;
                case GhostType::ORANGE:
                    ghostComp->setMovementSpeed(25.0f);
                    break;
            }
        }
    }
}

void PacmanGame::loadDrawableAssets() {
    try {
        JSONParser parser;
        _drawableAssets = parser.convertAssetsToGraphicalElements(
            "./config/pacman.json");
    } catch (const std::exception& e) {
        std::cerr << "Failed to load Pacman assets: "
                  << e.what() << std::endl;
    }
}

std::shared_ptr<IDrawableComponent> PacmanGame::getDrawableAsset(
const std::string& key) const {
    auto it = _drawableAssets.find(key);
    if (it != _drawableAssets.end()) {
        auto component = std::make_shared<DrawableComponent>(it->second);
        return component;
    }
    return nullptr;
}


void PacmanGame::update(float deltaTime) {
    s_perfMonitor.start();
    checkGameStatus();
    double statusCheckTime = s_perfMonitor.stop();
    double totalSystemTime = 0;

    for (const auto& system : _systems) {
        s_perfMonitor.start();
        system->update();
        totalSystemTime += s_perfMonitor.stop();
    }
    static int frameCount = 0;
    static double totalFrameTime = 0;
    static auto lastReport = std::chrono::high_resolution_clock::now();
    frameCount++;
    totalFrameTime += (statusCheckTime + totalSystemTime);
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = now - lastReport;
    if (elapsed.count() >= 5.0) {  // Report every 5 seconds
        const char* debugEnv = std::getenv("PACMAN_DEBUG");
        bool debugMode = (debugEnv && std::string(debugEnv) == "1");
        if (debugMode) {
            double avgFrameTime = totalFrameTime / frameCount;
        }
        frameCount = 0;
        totalFrameTime = 0;
        lastReport = now;
    }
}

void PacmanGame::checkGameStatus() {
    static std::shared_ptr<IEntity> gridEntity = nullptr;
    static std::shared_ptr<IEntity> pacmanEntity = nullptr;
    if (!gridEntity) {
        for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
            if (name == "Grid") {
                gridEntity = entity;
                break;
            }
        }
    }
    if (!pacmanEntity) {
        for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
            if (name == "Pacman") {
                pacmanEntity = entity;
                break;
            }
        }
    }
    static std::shared_ptr<GridComponent> gridComp = nullptr;
    if (gridEntity && !gridComp) {
        gridComp = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(gridEntity,
                static_cast<ComponentType>(1000)));
    }

    if (!gridComp)
        return;
    if (gridComp->isExitToMenu()) {
        _gameOver = true;
        stop();
        return;
    }
    _gameOver = gridComp->isGameOver();
    _gameWon = gridComp->isGameWon();

    static std::shared_ptr<PacmanComponent> pacmanComp = nullptr;
    if (pacmanEntity && !pacmanComp) {
        pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
            _componentManager->getComponentByType(pacmanEntity,
                static_cast<ComponentType>(1001)));
    }
    if (pacmanComp && pacmanComp->isDead())
        _gameOver = true;
}

int PacmanGame::getScore() const {
    std::shared_ptr<IEntity> pacmanEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Pacman") {
            pacmanEntity = entity;
            break;
        }
    }

    if (pacmanEntity) {
        auto pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
            _componentManager->getComponentByType(pacmanEntity,
                static_cast<ComponentType>(1001)));

        if (pacmanComp)
            return pacmanComp->getScore();
    }

    return 0;
}

bool PacmanGame::isGameOver() const {
    return _gameOver;
}

bool PacmanGame::hasWon() const {
    return _gameWon;
}

void PacmanGame::stop() {
    std::cout << "Pacman: Explicitly stopping game" << std::endl;
    _gameOver = true;
    _systems.clear();
    _eventSystem.reset();
    if (_componentManager && _entityManager) {
        try {
            auto entities = _entityManager->getEntitiesMap();
            for (const auto& entity : entities) {
                auto components
                    = _componentManager->getEntityComponents(entity.first);
                for (const auto& component : components) {
                    _componentManager->unregisterComponent(entity.first,
                        typeid(*component).name());
                }
                _entityManager->destroyEntity(entity.first);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error during Pacman cleanup: "
                << e.what() << std::endl;
        }
    }
}

extern "C" {
    IArcadeModule* entryPoint(void) {
        try {
            return new PacMan::PacmanGame();
        } catch (const std::exception& e) {
            std::cerr << "Error creating PacmanGame module: "
                << e.what() << std::endl;
            return nullptr;
        }
    }

    void destroy(IGameModule* instance) {
        delete instance;
    }

    Arcade::ModuleInfos module_infos() {
        return {"PacMan", "IDK",
            "IDK",
            "./lib/arcade_pacman.so", Arcade::ModuleType::GAME};
    }
}

}  // namespace PacMan
}  // namespace Arcade
