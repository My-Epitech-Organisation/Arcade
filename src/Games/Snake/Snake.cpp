// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake game module implementation
*/

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "Games/Snake/Snake.hpp"
#include "Games/Snake/Components/SnakeHeadComponent.hpp"
#include "Games/Snake/Components/GridComponent.hpp"
#include "Games/Snake/Components/FoodComponent.hpp"
#include "Games/Snake/System/UISystem.hpp"
#include "Games/Snake/System/GameLogic.hpp"
#include "Games/Snake/System/EventSubSystem.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "Shared/Models/ModuleInfos.hpp"
#include "Shared/JSONParser/JSONParser.hpp"

namespace Arcade {
namespace Snake {

SnakeGame::~SnakeGame() {
    _systems.clear();
    _eventSystem.reset();
    _gameLogicSystem.reset();

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

void SnakeGame::init(std::shared_ptr<IEventManager> eventManager,
std::shared_ptr<IComponentManager> componentManager,
std::shared_ptr<IEntityManager> entityManager) {
    _gameOver = false;
    _gameWon = false;
    _eventManager = eventManager;
    _componentManager = componentManager;
    _entityManager = entityManager;
    loadDrawableAssets();
    createGame();

    _gameLogicSystem = std::make_shared<GameLogic>(_componentManager,
        _entityManager, _drawableAssets);
    _systems.push_back(_gameLogicSystem);

    _systems.push_back(std::make_shared<UISystem>(_componentManager,
        _entityManager, _drawableAssets));

    _eventSystem = std::make_shared<EventSubSystem>(
        _componentManager, _entityManager,
        _eventManager, _drawableAssets, _gameLogicSystem);
    _systems.push_back(_eventSystem);
}

void SnakeGame::createGame() {
    SnakeFactory factory(_entityManager, _componentManager, _drawableAssets);
    factory.initializeGame(32.0f);
}

void SnakeGame::loadDrawableAssets() {
    JSONParser parser;
    _drawableAssets = parser.convertAssetsToGraphicalElements(
        "./config/snake.json");
}

std::shared_ptr<DrawableComponent> SnakeGame::getDrawableAsset(
const std::string& key) const {
    auto it = _drawableAssets.find(key);
    if (it != _drawableAssets.end()) {
        auto drawable = std::make_shared<DrawableComponent>();
        *drawable = it->second;
        return drawable;
    }
    return nullptr;
}

void SnakeGame::update() {
    checkGameStatus();

    for (const auto& system : _systems) {
        system->update();
    }
}

void SnakeGame::checkGameStatus() {
    for (const auto& entity : _entityManager->getEntities()) {
        auto gridComp = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(entity.first,
                static_cast<ComponentType>(1000)));

        if (gridComp && gridComp->isGameOver()) {
            _gameOver = true;
            break;
        }
    }

    _gameWon = false;
}

int SnakeGame::getScore() const {
    for (const auto& entity : _entityManager->getEntities()) {
        auto snakeComp = std::dynamic_pointer_cast<SnakeHeadComponent>(
            _componentManager->getComponentByType(entity.first,
                static_cast<ComponentType>(1001)));
        if (snakeComp) {
            return snakeComp->getScore();
        }
    }

    return 0;
}

bool SnakeGame::isGameOver() const {
    return _gameOver;
}

bool SnakeGame::hasWon() const {
    return _gameWon;
}

void SnakeGame::stop() {
    _gameOver = true;

    if (_eventSystem) {
        try {
            _eventSystem->unsubscribeEvents();
        } catch (const std::exception& e) {
            std::cerr   << "Error unsubscribing Snake events: "
                        << e.what() << std::endl;
        } catch (...) {
            std::cerr   << "Unknown error unsubscribing Snake events"
                        << std::endl;
        }
    }

    _systems.clear();
    _gameLogicSystem.reset();
    _eventSystem.reset();

    if (_entityManager && _componentManager) {
        try {
            auto entities = _entityManager->getEntities();
            for (const auto& entity : entities) {
                auto components =
                _componentManager->getEntityComponents(entity.first);
                for (const auto& component : components) {
                    _componentManager->unregisterComponent(entity.first,
                        typeid(*component).name());
                }
                _entityManager->destroyEntity(entity.first);
            }
        } catch (const std::exception& e) {
            std::cerr   << "Error during Snake cleanup: "
                        << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error during Snake cleanup" << std::endl;
        }
    }
    _componentManager.reset();
    _entityManager.reset();
    _eventManager.reset();
}

extern "C" {
    Arcade::IArcadeModule* entryPoint(void) {
        try {
            return new Snake::SnakeGame();
        } catch (const std::exception& e) {
            std::cerr << "Error creating SnakeGame module: "
                << e.what() << std::endl;
            return nullptr;
        }
    }

    void destroy(IGameModule* instance) {
        delete instance;
    }

    Arcade::ModuleInfos module_infos() {
        return {"Snake", "A classic Snake game",
            "assets/snake/snake.png",
            "./lib/arcade_snake.so", Arcade::ModuleType::GAME};
    }
}

}  // namespace Snake
}  // namespace Arcade
