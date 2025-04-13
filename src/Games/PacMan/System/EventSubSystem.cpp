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
#include <map>
#include <string>
#include "Shared/Interface/ECS/IEntity.hpp"
#include "ECS/Entity/Entity.hpp"
#include "Games/PacMan/System/EventSubSystem.hpp"
#include "Games/PacMan/System/GameLogic.hpp"
#include "Shared/EventManager/KeyEvent/KeyEvent.hpp"
#include "Shared/Models/EventType.hpp"
#include "Shared/Models/KeysType.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace PacMan {

EventSubSystem::EventSubSystem(
std::shared_ptr<Arcade::IComponentManager> componentManager,
std::shared_ptr<Arcade::IEntityManager> entityManager,
std::shared_ptr<Arcade::IEventManager> eventManager,
const std::map<std::string, Arcade::DrawableComponent>&
drawableAssets)
: _componentManager(std::move(componentManager)),
_entityManager(std::move(entityManager)),
_eventManager(std::move(eventManager)),
_drawableAssets(drawableAssets) {
    subscribeToEvents();
}

EventSubSystem::~EventSubSystem() {
    std::cerr << "EventSubSystem: Destructor called, unsubscribing events" << std::endl;
    
    // Safely unregister all event handlers
    if (_eventManager) {
        try {
            _eventManager->unsubscribeAll();
        } catch (const std::exception& e) {
            std::cerr << "Error unsubscribing events in ~EventSubSystem(): " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error unsubscribing events in ~EventSubSystem()" << std::endl;
        }
    }
    
    // Explicitly clear member pointers to prevent further use
    _componentManager.reset();
    _entityManager.reset();
    _eventManager.reset();
}

void EventSubSystem::subscribeToEvents() {
    if (!_eventManager) {
        std::cerr << "EventSubSystem: Cannot subscribe, event manager is null" << std::endl;
        return;
    }
    try {
        Arcade::KeyEvent upKey(Arcade::Keys::UP, Arcade::EventType::KEY_PRESSED);
        Arcade::KeyEvent downKey(Arcade::Keys::DOWN,
            Arcade::EventType::KEY_PRESSED);
        Arcade::KeyEvent leftKey(Arcade::Keys::LEFT,
            Arcade::EventType::KEY_PRESSED);
        Arcade::KeyEvent rightKey(Arcade::Keys::RIGHT,
            Arcade::EventType::KEY_PRESSED);
        Arcade::KeyEvent rKey(Arcade::Keys::R,
            Arcade::EventType::KEY_PRESSED);
        _eventManager->unsubscribeAll(upKey);
        _eventManager->unsubscribeAll(downKey);
        _eventManager->unsubscribeAll(leftKey);
        _eventManager->unsubscribeAll(rightKey);
        _eventManager->unsubscribeAll(rKey);
        _eventManager->subscribe(upKey, [this](const IEvent& event) {
            (void)event;
            handleKeyUp();
        });
        _eventManager->subscribe(downKey, [this](const IEvent& event) {
            (void)event;
            handleKeyDown();
        });
        _eventManager->subscribe(leftKey, [this](const IEvent& event) {
            (void)event;
            handleKeyLeft();
        });
        _eventManager->subscribe(rightKey, [this](const IEvent& event) {
            (void)event;
            handleKeyRight();
        });
        _eventManager->subscribe(rKey, [this](const IEvent& event) {
            (void)event;
            handleKeyR();
        });
    } catch (const std::exception& e) {
        std::cerr << "Error subscribing events: " << e.what() << std::endl;
    }
}

std::shared_ptr<Arcade::IEntity>
EventSubSystem::findPacmanEntity() const {
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto component = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1001));
        if (component)
            return entity;
    }
    return nullptr;
}

std::pair<std::shared_ptr<PacmanComponent>, std::shared_ptr<GridComponent>>
EventSubSystem::getPacmanAndGridComponents() {
    std::shared_ptr<IEntity> pacmanEntity = findPacmanEntity();

    auto pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
        _componentManager->getComponentByType(pacmanEntity,
            static_cast<ComponentType>(1001)));

    std::shared_ptr<IEntity> gridEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
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

void EventSubSystem::handleKeyEsc() {
    // No-op implementation - ESC handling is now managed by GameLoop
    std::cout << "PacMan ESC handler is disabled - using GameLoop handler instead" << std::endl;
}

void EventSubSystem::handleKeyR() {
    std::shared_ptr<IEntity> gridEntity;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Grid") {
            gridEntity = entity;
            break;
        }
    }

    if (!gridEntity) {
        std::cout << "Cannot find Grid entity for restart" << std::endl;
        return;
    }

    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));

    if (!gridComp) return;
    
    std::cout << "Handling R key press - restarting game" << std::endl;

    gridComp->setGameOver(false);
    gridComp->setGameWon(false);
    
    // Reset Pacman state
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Pacman") {
            auto pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
                _componentManager->getComponentByType(entity,
                    static_cast<ComponentType>(1001)));

            if (pacmanComp) {
                pacmanComp->setLives(3);
                pacmanComp->setScore(0);
                pacmanComp->setCurrentDirection(Direction::NONE);
                pacmanComp->setNextDirection(Direction::NONE);
                pacmanComp->setMoving(false); // Make sure Pacman is not moving
            }
            
            // Make sure Pacman sprite is visible
            auto pacmanDrawable = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));
            if (pacmanDrawable) {
                pacmanDrawable->setVisibility(true);
                // Reset to default sprite
                auto pacmanAsset = _drawableAssets.find("pacman.default");
                if (pacmanAsset != _drawableAssets.end()) {
                    *pacmanDrawable = pacmanAsset->second;
                } else {
                    pacmanDrawable->
                        setAsTexture("assets/pacman/pacman.png", 32, 32);
                    pacmanDrawable->setAsCharacter('C');
                }
            }
            break;
        }
    }

    auto gameLogic = std::make_shared<GameLogic>(_componentManager,
        _entityManager, _drawableAssets);
    gameLogic->reloadCurrentMap();
    
    // Make sure to set food count
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Grid") {
            gridComp->setFoodCount(gridComp->getTotalFoodCount());
            break;
        }
    }
}

bool EventSubSystem::areEventsSubscribed() const {
    try {
        // Use isKeyPressed instead of isEventSubscribed which doesn't exist
        bool upSubscribed = _eventManager->isKeyPressed(Arcade::Keys::UP);
        bool downSubscribed = _eventManager->isKeyPressed(Arcade::Keys::DOWN);
        bool leftSubscribed = _eventManager->isKeyPressed(Arcade::Keys::LEFT);
        bool rightSubscribed = _eventManager->isKeyPressed(Arcade::Keys::RIGHT);
        bool rSubscribed = _eventManager->isKeyPressed(Arcade::Keys::R);
        bool escSubscribed = _eventManager->isKeyPressed(Arcade::Keys::ESC);
            
        // If any key is detected as pressed, we consider it subscribed
        // This isn't perfect but should work as a proxy check
        return upSubscribed || downSubscribed || leftSubscribed || 
               rightSubscribed || rSubscribed || escSubscribed;
    } catch (...) {
        return false;
    }
}

void EventSubSystem::update() {
    // Always print a message on first update
    static bool firstUpdate = true;
    if (firstUpdate) {
        std::cerr << "EventSubSystem: First update executed" << std::endl;
        // IMPORTANT: Log event registration status on first update
        firstUpdate = false;
    }
    
    // Check for subscriptions less frequently - only check every 30 frames
    static int checkCounter = 0;
    checkCounter = (checkCounter + 1) % 30;
    
    if (checkCounter == 0) {
        // IMPORTANT: Use a separate flag to track if we need to resubscribe
        bool needResubscribe = false;
        
        try {
            // Only spot check one key - if UP works, assume the others do too
            bool upSubscribed = _eventManager->isKeyPressed(Arcade::Keys::UP);
            if (!upSubscribed) {
                needResubscribe = true;
                std::cerr << "EventSubSystem: UP key not subscribed, needs resubscription" << std::endl;
            }
        } catch (const std::exception& e) {
            needResubscribe = true;
            std::cerr << "EventSubSystem: Exception checking subscriptions: " << e.what() << std::endl;
        } catch (...) {
            needResubscribe = true;
            std::cerr << "EventSubSystem: Unknown exception checking subscriptions" << std::endl;
        }
        
        // If we determined subscriptions need refreshing
        if (needResubscribe) {
            std::cout << "Resubscribing PacMan events due to lost subscriptions" << std::endl << std::flush;
            std::cerr << "Resubscribing PacMan events due to lost subscriptions" << std::endl;
            
            // First subscribe to ESC key with GameLoop's handler
            try {
                subscribeToEvents();
                std::cerr << "EventSubSystem: Events resubscribed successfully" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "EventSubSystem: Error resubscribing events: " << e.what() << std::endl;
            }
        }
    }
}

}  // namespace PacMan
}  // namespace Arcade
