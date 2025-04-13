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
#include "Shared/EventManager/KeyEvent/KeyEvent.hpp"
#include "Shared/Models/EventType.hpp"
#include "Shared/Models/KeysType.hpp"

namespace Arcade {
namespace Snake {

EventSubSystem::EventSubSystem(
std::shared_ptr<IComponentManager> componentManager,
std::shared_ptr<IEntityManager> entityManager,
std::shared_ptr<IEventManager> eventManager,
const std::map<std::string, DrawableComponent>& assets)
: _componentManager(componentManager), _entityManager(entityManager),
_eventManager(eventManager), _assets(assets) {
    subscribeToEvents();
}

void EventSubSystem::update() {
    // Nothing to do here, events are handled by callbacks
}

void EventSubSystem::subscribeToEvents() {
    // Arrow keys for movement
    Arcade::KeyEvent upKey(Arcade::Keys::UP, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(upKey, [this]() {
        handleKeyUpPressed();
    });

    Arcade::KeyEvent downKey(Arcade::Keys::DOWN,
        Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(downKey, [this]() {
        handleKeyDownPressed();
    });

    Arcade::KeyEvent leftKey(Arcade::Keys::LEFT,
        Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(leftKey, [this]() {
        handleKeyLeftPressed();
    });

    Arcade::KeyEvent rightKey(Arcade::Keys::RIGHT,
        Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(rightKey, [this]() {
        handleKeyRightPressed();
    });

    Arcade::KeyEvent wKey(Arcade::Keys::W, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(wKey, [this]() {
        handleKeyUpPressed();
    });

    Arcade::KeyEvent sKey(Arcade::Keys::S, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(sKey, [this]() {
        handleKeyDownPressed();
    });

    Arcade::KeyEvent aKey(Arcade::Keys::A, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(aKey, [this]() {
        handleKeyLeftPressed();
    });

    Arcade::KeyEvent dKey(Arcade::Keys::D, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(dKey, [this]() {
        handleKeyRightPressed();
    });

    Arcade::KeyEvent rKey(Arcade::Keys::R, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(rKey, [this]() {
        handleRestartPressed();
    });

    Arcade::KeyEvent escKey(Arcade::Keys::ESC, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(escKey, [this]() {
        handleQuitPressed();
    });
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
    // Find grid entity and reset game
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto gridComp = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1000)));
        if (gridComp) {
            gridComp->resetGrid();
            break;
        }
    }

    // Reset snake
    Arcade::Entity snakeEntity = findSnakeHeadEntity();
    if (snakeEntity) {
        auto snakeComp = std::dynamic_pointer_cast<SnakeHeadComponent>(
            _componentManager->getComponentByType(snakeEntity,
                static_cast<ComponentType>(1001)));
        if (snakeComp) {
            snakeComp->setScore(0);
            // snakeComp->clearSegments();
            snakeComp->setCurrentDirection(Direction::NONE);
            snakeComp->setNextDirection(Direction::NONE);
        }
    }
}

void EventSubSystem::handleQuitPressed() {
    // Find grid entity and set game over
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
