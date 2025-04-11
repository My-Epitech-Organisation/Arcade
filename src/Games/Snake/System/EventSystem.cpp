// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Event System for Snake game implementation
*/

#include "Games/Snake/System/EventSystem.hpp"
#include "Shared/EventManager/KeyEvent/KeyEvent.hpp"
#include "Shared/Models/EventType.hpp"
#include "Shared/Models/KeysType.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Games/Snake/Types.hpp"

#include <iostream>

namespace Arcade {

EventSystem::EventSystem(
    std::shared_ptr<IEventManager> eventManager,
    std::shared_ptr<IComponentManager> componentManager,
    std::shared_ptr<IEntityManager> entityManager)
    : _eventManager(eventManager),
      _componentManager(componentManager),
      _entityManager(entityManager)
{
    subscribeToEvents();
}

EventSystem::~EventSystem() {
    if (_eventManager)
        _eventManager->unsubscribeAll();
}

void EventSystem::subscribeToEvents() {
    Arcade::KeyEvent upKey(Arcade::Keys::UP, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(upKey, [this]() {
        handleKeyUp();
    });

    Arcade::KeyEvent downKey(Arcade::Keys::DOWN, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(downKey, [this]() {
        handleKeyDown();
    });

    Arcade::KeyEvent leftKey(Arcade::Keys::LEFT, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(leftKey, [this]() {
        handleKeyLeft();
    });

    Arcade::KeyEvent rightKey(Arcade::Keys::RIGHT, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(rightKey, [this]() {
        handleKeyRight();
    });

    Arcade::KeyEvent spaceKey(Arcade::Keys::SPACE, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(spaceKey, [this]() {
        handleKeySpace(true);
    });

    Arcade::KeyEvent spaceReleasedKey(Arcade::Keys::SPACE, Arcade::EventType::KEY_RELEASED);
    _eventManager->subscribe(spaceReleasedKey, [this]() {
        handleKeySpace(false);
    });

    Arcade::KeyEvent rKey(Arcade::Keys::R, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(rKey, [this]() {
        handleKeyR();
    });
}

void EventSystem::handleKeyUp() {
    std::cout << "[DEBUG] EventSystem::handleKeyUp called" << std::endl;
    for (const auto& [entity, _] : _entityManager->getEntities()) {
        auto snakeComponentBase = _componentManager->getComponentBase(entity, "SnakeComponent");
        auto snakeComponent = dynamic_cast<SnakeComponent*>(snakeComponentBase.get());
        if (snakeComponent && snakeComponent->direction != Direction::DOWN) {
            std::cout << "[DEBUG] Setting snake direction to UP" << std::endl;
            snakeComponent->direction = Direction::UP;
            break;
        }
    }
}

void EventSystem::handleKeyDown() {
    std::cout << "[DEBUG] EventSystem::handleKeyDown called" << std::endl;
    for (const auto& [entity, _] : _entityManager->getEntities()) {
        auto snakeComponentBase = _componentManager->getComponentBase(entity, "SnakeComponent");
        auto snakeComponent = dynamic_cast<SnakeComponent*>(snakeComponentBase.get());
        if (snakeComponent && snakeComponent->direction != Direction::UP) {
            std::cout << "[DEBUG] Setting snake direction to DOWN" << std::endl;
            snakeComponent->direction = Direction::DOWN;
            break;
        }
    }
}

void EventSystem::handleKeyLeft() {
    std::cout << "[DEBUG] EventSystem::handleKeyLeft called" << std::endl;
    for (const auto& [entity, _] : _entityManager->getEntities()) {
        auto snakeComponentBase = _componentManager->getComponentBase(entity, "SnakeComponent");
        auto snakeComponent = dynamic_cast<SnakeComponent*>(snakeComponentBase.get());
        if (snakeComponent && snakeComponent->direction != Direction::RIGHT) {
            std::cout << "[DEBUG] Setting snake direction to LEFT" << std::endl;
            snakeComponent->direction = Direction::LEFT;
            break;
        }
    }
}

void EventSystem::handleKeyRight() {
    std::cout << "[DEBUG] EventSystem::handleKeyRight called" << std::endl;
    for (const auto& [entity, _] : _entityManager->getEntities()) {
        auto snakeComponentBase = _componentManager->getComponentBase(entity, "SnakeComponent");
        auto snakeComponent = dynamic_cast<SnakeComponent*>(snakeComponentBase.get());
        if (snakeComponent && snakeComponent->direction != Direction::LEFT) {
            std::cout << "[DEBUG] Setting snake direction to RIGHT" << std::endl;
            snakeComponent->direction = Direction::RIGHT;
            break;
        }
    }
}

void EventSystem::handleKeySpace(bool pressed) {
    std::cout << "[DEBUG] EventSystem::handleKeySpace called with pressed=" << pressed << std::endl;
    for (const auto& [entity, _] : _entityManager->getEntities()) {
        auto snakeComponentBase = _componentManager->getComponentBase(entity, "SnakeComponent");
        auto snakeComponent = dynamic_cast<SnakeComponent*>(snakeComponentBase.get());
        if (snakeComponent) {
            std::cout << "[DEBUG] Setting snake speedMultiplier to " << (pressed ? 2.0f : 1.0f) << std::endl;
            snakeComponent->speedMultiplier = pressed ? 2.0f : 1.0f;
            break;
        }
    }
}

void EventSystem::handleKeyR() {
    std::cout << "[DEBUG] EventSystem::handleKeyR called" << std::endl;
    // Restart logic would go here
    // For now, do nothing as this is handled at a higher level
}

void EventSystem::update() {
    // The event handling is done via callbacks, so no specific update logic needed
}

}  // namespace Arcade
