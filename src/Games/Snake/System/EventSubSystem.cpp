// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Event management system implementation for Snake game
*/

#include "Games/Snake/System/EventSubSystem.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Shared/Interface/Core/Event.hpp"

namespace Arcade {

EventSubSystem::EventSubSystem(std::shared_ptr<IEventManager> eventManager,
                             std::shared_ptr<IComponentManager> componentManager,
                             std::shared_ptr<IEntityManager> entityManager)
    : _eventManager(eventManager),
      _componentManager(componentManager),
      _entityManager(entityManager) {
    _keyPressHandler = std::bind(&EventSubSystem::handleKeyPressed, this, std::placeholders::_1);
    _eventManager->subscribe(Event::Type::KEY_PRESSED, _keyPressHandler);
}

EventSubSystem::~EventSubSystem() {
    _eventManager->unsubscribe(Event::Type::KEY_PRESSED, _keyPressHandler);
}

void EventSubSystem::update() {
    // No need for update as events are handled by callbacks
}

void EventSubSystem::handleKeyPressed(const Event &event) {
    auto entities = _entityManager->getEntitiesWithComponent<Snake>();
    if (entities.empty()) {
        return;
    }

    Entity snakeEntity = entities[0];
    auto snake = _componentManager->getComponent<Snake>(snakeEntity);

    // Check key code and change direction
    // Prevent 180-degree turns (e.g., can't go RIGHT if currently going LEFT)
    switch (event.key.keyCode) {
        case Event::Key::ARROW_UP:
            if (snake->direction != Snake::DOWN) {
                snake->direction = Snake::UP;
            }
            break;
        case Event::Key::ARROW_DOWN:
            if (snake->direction != Snake::UP) {
                snake->direction = Snake::DOWN;
            }
            break;
        case Event::Key::ARROW_LEFT:
            if (snake->direction != Snake::RIGHT) {
                snake->direction = Snake::LEFT;
            }
            break;
        case Event::Key::ARROW_RIGHT:
            if (snake->direction != Snake::LEFT) {
                snake->direction = Snake::RIGHT;
            }
            break;
        default:
            break;
    }
}

}  // namespace Arcade
