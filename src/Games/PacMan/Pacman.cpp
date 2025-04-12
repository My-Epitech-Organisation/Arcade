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
    _gameOver = false;
    _gameWon = false;
    _eventManager = eventManager;
    _componentManager = componentManager;
    _entityManager = entityManager;
    _scoreProvider = scoreProvider;
    loadDrawableAssets();
    createGame();

    _eventSystem = std::make_shared<EventSubSystem>(
        _componentManager, _entityManager, _eventManager, _drawableAssets);
    _systems.push_back(std::make_shared<GameLogic>(_componentManager,
        _entityManager, _drawableAssets));
    _systems.push_back(std::make_shared<UISystem>(_componentManager,
        _entityManager, _drawableAssets));
    _systems.push_back(_eventSystem);
}

void PacmanGame::createGame() {
    PacmanFactory factory(_entityManager, _componentManager, _drawableAssets);
    factory.initializeGame(16.0f);
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
    (void)deltaTime;
    checkGameStatus();

    for (const auto& system : _systems) {
        system->update();
    }
}

void PacmanGame::checkGameStatus() {
    std::shared_ptr<IEntity> gridEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Grid") {
            gridEntity = entity;
            break;
        }
    }

    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));

    if (!gridComp)
        return;

    _gameOver = gridComp->isGameOver();
    _gameWon = gridComp->isGameWon();

    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Pacman") {
            auto pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
                _componentManager->getComponentByType(entity,
                    static_cast<ComponentType>(1001)));
            if (pacmanComp && pacmanComp->isDead())
                _gameOver = true;
            break;
        }
    }
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
    _gameOver = true;
    _componentManager.reset();
    _entityManager.reset();
    _systems.clear();
    _eventSystem.reset();
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
