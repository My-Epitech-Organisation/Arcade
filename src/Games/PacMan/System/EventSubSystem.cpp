// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Event Subsystem for Pacman
*/

#include <iostream>
#include <utility>
#include <memory>
#include "Games/PacMan/System/EventSubSystem.hpp"
#include "Shared/EventManager/KeyEvent/KeyEvent.hpp"
#include "Shared/Models/EventType.hpp"
#include "Shared/Models/KeysType.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"

namespace Arcade {
namespace PacMan {

EventSubSystem::EventSubSystem(
std::shared_ptr<Arcade::IComponentManager> componentManager,
std::shared_ptr<Arcade::IEntityManager> entityManager,
std::shared_ptr<Arcade::IEventManager> eventManager)
: _componentManager(componentManager),
_entityManager(entityManager),
_eventManager(eventManager) {
    subscribeToEvents();
}

EventSubSystem::~EventSubSystem() {
    if (_eventManager)
        _eventManager->unsubscribeAll();
}

void EventSubSystem::subscribeToEvents() {
    Arcade::KeyEvent upKey(Arcade::Keys::UP, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(upKey, [this]() {
        handleKeyUp();
    });

    Arcade::KeyEvent downKey(Arcade::Keys::DOWN,
        Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(downKey, [this]() {
        handleKeyDown();
    });

    Arcade::KeyEvent leftKey(Arcade::Keys::LEFT,
        Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(leftKey, [this]() {
        handleKeyLeft();
    });

    Arcade::KeyEvent rightKey(Arcade::Keys::RIGHT,
        Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(rightKey, [this]() {
        handleKeyRight();
    });

    Arcade::KeyEvent rKey(Arcade::Keys::R,
        Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(rKey, [this]() {
        handleKeyR();
    });
}

Arcade::Entity EventSubSystem::findPacmanEntity() const {
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto component = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1001));
        if (component)
            return entity;
    }
    return 0;
}

std::pair<std::shared_ptr<PacmanComponent>, std::shared_ptr<GridComponent>>
EventSubSystem::getPacmanAndGridComponents() {
    Arcade::Entity pacmanEntity = findPacmanEntity();

    auto pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
        _componentManager->getComponentByType(pacmanEntity,
            static_cast<ComponentType>(1001)));

    Arcade::Entity gridEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto component = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1000));
        if (component) {
            gridEntity = entity;
            break;
        }
    }

    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));

    return {pacmanComp, gridComp};
}

Direction EventSubSystem::validateDirection(Direction requestedDir, size_t x,
size_t y, std::shared_ptr<GridComponent> grid) {
    if (!grid) return Direction::NONE;

    size_t nextX = x;
    size_t nextY = y;

    switch (requestedDir) {
        case Direction::UP:
            if (y > 0) nextY--;
            break;
        case Direction::DOWN:
            if (y < grid->getHeight() - 1) nextY++;
            break;
        case Direction::LEFT:
            if (x > 0) nextX--;
            break;
        case Direction::RIGHT:
            if (x < grid->getWidth() - 1) nextX++;
            break;
        case Direction::NONE:
            return Direction::NONE;
    }

    if (grid->getCellType(nextX, nextY) == CellType::WALL)
        return Direction::NONE;

    return requestedDir;
}

void EventSubSystem::handleKeyUp() {
    auto [pacmanComp, gridComp] = getPacmanAndGridComponents();
    if (pacmanComp)
        pacmanComp->setNextDirection(Direction::UP);
}

void EventSubSystem::handleKeyDown() {
    auto [pacmanComp, gridComp] = getPacmanAndGridComponents();
    if (pacmanComp)
        pacmanComp->setNextDirection(Direction::DOWN);
}

void EventSubSystem::handleKeyLeft() {
    auto [pacmanComp, gridComp] = getPacmanAndGridComponents();
    if (pacmanComp)
        pacmanComp->setNextDirection(Direction::LEFT);
}

void EventSubSystem::handleKeyRight() {
    auto [pacmanComp, gridComp] = getPacmanAndGridComponents();
    if (pacmanComp)
        pacmanComp->setNextDirection(Direction::RIGHT);
}

void EventSubSystem::handleKeyR() {
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto component = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1000));
        if (component) {
            auto grid = std::dynamic_pointer_cast<GridComponent>(component);
            if (grid) {
                std::cout << "Game reset requested" << std::endl;
                break;
            }
        }
    }
}

void EventSubSystem::update() {
    // Nothing to do here - event handling is done via callbacks
}

}  // namespace PacMan
}  // namespace Arcade
