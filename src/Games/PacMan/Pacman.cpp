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
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"

namespace Arcade {
namespace PacMan {

PacmanGame::~PacmanGame() {
    _systems.clear();
    _eventSystem.reset();

    if (_entityManager && _componentManager) {
        auto entities = _entityManager->getEntities();
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
std::shared_ptr<IEntityManager> entityManager) {
    _gameOver = false;
    _gameWon = false;
    _eventManager = eventManager;
    _componentManager = componentManager;
    _entityManager = entityManager;

    createGame();

    _eventSystem = std::make_shared<EventSubSystem>(
        _componentManager, _entityManager, _eventManager);
    _systems.push_back(std::make_shared<GameLogic>(_componentManager,
        _entityManager));
    _systems.push_back(_eventSystem);
}

void PacmanGame::createGame() {
    PacmanFactory factory(_entityManager, _componentManager);
    factory.initializeGame(16.0f);
}

void PacmanGame::update() {
    checkGameStatus();

    for (const auto& system : _systems) {
        system->update();
    }
}

void PacmanGame::checkGameStatus() {
    Arcade::Entity gridEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntities()) {
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

    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "Pacman") {
            auto pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
                _componentManager->getComponentByType(entity,
                    static_cast<ComponentType>(1001)));
            if (pacmanComp && pacmanComp->isDead()) {
                _gameOver = true;
            }
            break;
        }
    }
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

std::string PacmanGame::getSpecialCompSprite(size_t id) const {
    switch (id) {
        case static_cast<size_t>(ComponentType::SPRITE):
            return "assets/pacman/pacman.png";
        case 1001:
            return "assets/pacman/pacman.png";
        case 1002:
            return "assets/pacman/ghost_red.png";
        case 1003:
            return "assets/pacman/dot.png";
        default:
            return "";
    }
}

// Entry points for dynamic loading
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
}

}  // namespace PacMan
}  // namespace Arcade
