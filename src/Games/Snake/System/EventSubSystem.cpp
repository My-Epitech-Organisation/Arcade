// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Event Subsystem implementation for Snake game
*/

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include "Games/Snake/System/EventSubSystem.hpp"
#include "Games/Snake/Components/GridComponent.hpp"
#include "Games/Snake/System/GameLogic.hpp"
#include "Shared/EventManager/KeyEvent/KeyEvent.hpp"
#include "Shared/Models/EventType.hpp"
#include "Shared/Models/KeysType.hpp"

namespace Arcade {
namespace Snake {

EventSubSystem::EventSubSystem(
std::shared_ptr<IComponentManager> componentManager,
std::shared_ptr<IEntityManager> entityManager,
std::shared_ptr<IEventManager> eventManager,
const std::map<std::string, DrawableComponent>& assets,
std::shared_ptr<GameLogic> gameLogic)
: _componentManager(componentManager), _entityManager(entityManager),
_eventManager(eventManager), _assets(assets), _gameLogic(gameLogic) {
    subscribeToEvents();
}

EventSubSystem::~EventSubSystem() {
    std::cout << "Snake EventSubSystem: Destructor called" << std::endl;
    unsubscribeEvents();
}

void EventSubSystem::update() {
}

void EventSubSystem::unsubscribeEvents() {
    if (!_eventManager) {
        return;
    }

    try {
        _eventManager->unsubscribeAll();

        _subscribedEvents.clear();
    } catch (const std::exception& e) {
        std::cerr << "Snake EventSubSystem: Error calling unsubscribeAll(): "
                  << e.what() << std::endl;
    } catch (...) {
        std::cerr <<
        "Snake EventSubSystem: Unknown error calling unsubscribeAll()"
        << std::endl;
    }
}

void EventSubSystem::subscribeToEvents() {
    if (!_eventManager) {
        return;
    }

    _subscribedEvents.clear();

    try {
        Arcade::KeyEvent upKey(Arcade::Keys::UP,
            Arcade::EventType::KEY_PRESSED);
        _eventManager->subscribe(upKey, [this]() {
            handleKeyUpPressed();
        });
        _subscribedEvents.push_back(upKey);

        Arcade::KeyEvent zKey(Arcade::Keys::Z,
            Arcade::EventType::KEY_PRESSED);
        _eventManager->subscribe(zKey, [this]() {
            handleKeyUpPressed();
        });
        _subscribedEvents.push_back(zKey);

        Arcade::KeyEvent downKey(Arcade::Keys::DOWN,
            Arcade::EventType::KEY_PRESSED);
        _eventManager->subscribe(downKey, [this]() {
            handleKeyDownPressed();
        });
        _subscribedEvents.push_back(downKey);

        Arcade::KeyEvent sKey(Arcade::Keys::S, Arcade::EventType::KEY_PRESSED);
        _eventManager->subscribe(sKey, [this]() {
            handleKeyDownPressed();
        });
        _subscribedEvents.push_back(sKey);

        Arcade::KeyEvent leftKey(Arcade::Keys::LEFT,
            Arcade::EventType::KEY_PRESSED);
        _eventManager->subscribe(leftKey, [this]() {
            handleKeyLeftPressed();
        });
        _subscribedEvents.push_back(leftKey);

        Arcade::KeyEvent qKey(Arcade::Keys::Q, Arcade::EventType::KEY_PRESSED);
        _eventManager->subscribe(qKey, [this]() {
            handleKeyLeftPressed();
        });
        _subscribedEvents.push_back(qKey);

        Arcade::KeyEvent rightKey(Arcade::Keys::RIGHT,
            Arcade::EventType::KEY_PRESSED);
        _eventManager->subscribe(rightKey, [this]() {
            handleKeyRightPressed();
        });
        _subscribedEvents.push_back(rightKey);

        Arcade::KeyEvent dKey(Arcade::Keys::D, Arcade::EventType::KEY_PRESSED);
        _eventManager->subscribe(dKey, [this]() {
            handleKeyRightPressed();
        });
        _subscribedEvents.push_back(dKey);

        Arcade::KeyEvent rKey(Arcade::Keys::R, Arcade::EventType::KEY_PRESSED);
        _eventManager->subscribe(rKey, [this]() {
            handleRestartPressed();
        });
        _subscribedEvents.push_back(rKey);

        Arcade::KeyEvent escKey(Arcade::Keys::ESC,
            Arcade::EventType::KEY_PRESSED);
        _eventManager->subscribe(escKey, [this]() {
            handleQuitPressed();
        });
        _subscribedEvents.push_back(escKey);

    } catch (const std::exception& e) {
        std::cerr << "Snake EventSubSystem: Error subscribing events: "
        << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Snake EventSubSystem: Unknown error subscribing events"
        << std::endl;
    }
}

void EventSubSystem::handleKeyUpPressed() {
    Arcade::Entity snakeEntity = findSnakeHeadEntity();
    if (!snakeEntity)
        return;

    auto snakeComp = std::dynamic_pointer_cast<SnakeHeadComponent>(
        _componentManager->getComponentByType(snakeEntity,
            static_cast<ComponentType>(1001)));
    if (snakeComp) {
        snakeComp->setNextDirection(Direction::UP);
    }
}

void EventSubSystem::handleKeyDownPressed() {
    Arcade::Entity snakeEntity = findSnakeHeadEntity();
    if (!snakeEntity)
        return;

    auto snakeComp = std::dynamic_pointer_cast<SnakeHeadComponent>(
        _componentManager->getComponentByType(snakeEntity,
            static_cast<ComponentType>(1001)));
    if (snakeComp) {
        snakeComp->setNextDirection(Direction::DOWN);
    }
}

void EventSubSystem::handleKeyLeftPressed() {
    Arcade::Entity snakeEntity = findSnakeHeadEntity();
    if (!snakeEntity)
        return;

    auto snakeComp = std::dynamic_pointer_cast<SnakeHeadComponent>(
        _componentManager->getComponentByType(snakeEntity,
            static_cast<ComponentType>(1001)));
    if (snakeComp) {
        snakeComp->setNextDirection(Direction::LEFT);
    }
}

void EventSubSystem::handleKeyRightPressed() {
    Arcade::Entity snakeEntity = findSnakeHeadEntity();
    if (!snakeEntity)
        return;

    auto snakeComp = std::dynamic_pointer_cast<SnakeHeadComponent>(
        _componentManager->getComponentByType(snakeEntity,
            static_cast<ComponentType>(1001)));
    if (snakeComp) {
        snakeComp->setNextDirection(Direction::RIGHT);
    }
}

void EventSubSystem::handleRestartPressed() {
    if (_gameLogic) {
        _gameLogic->resetGame();
    }
}

void EventSubSystem::handleQuitPressed() {
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto gridComp = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1000)));
        if (gridComp) {
            gridComp->setGameOver(true);
            break;
        }
    }
}

Arcade::Entity EventSubSystem::findSnakeHeadEntity() const {
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto component = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1001));
        if (component)
            return entity;
    }
    return 0;
}

}  // namespace Snake
}  // namespace Arcade
