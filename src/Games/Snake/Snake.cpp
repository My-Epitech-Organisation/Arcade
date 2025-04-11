// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake game module implementation
*/

#include <string>
#include <memory>
#include "Games/Snake/Snake.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Shared/Interface/ECS/IEntity.hpp"
#include "Games/Snake/SnakeFactory.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Games/Snake/Components/Food.hpp"
#include "Shared/Models/ModuleInfos.hpp"

namespace Arcade {

SnakeGame::~SnakeGame() {
    stop();
}

void SnakeGame::init(std::shared_ptr<IEventManager> eventManager,
std::shared_ptr<IComponentManager> componentManager,
std::shared_ptr<IEntityManager> entityManager) {
    _eventManager = eventManager;
    _componentManager = componentManager;
    _entityManager = entityManager;

    _eventSystem = std::make_shared<EventSystem>(
        _eventManager, _componentManager, _entityManager);
    _movementSystem = std::make_shared<MovementSystem>(
        _componentManager, _entityManager);
    _collisionSystem = std::make_shared<CollisionSystem>(
        _componentManager, _entityManager);
    _renderSystem = std::make_shared<RenderSystem>(
        _componentManager, _entityManager);

    _systems.push_back(_eventSystem);
    _systems.push_back(_movementSystem);
    _systems.push_back(_collisionSystem);
    _systems.push_back(_renderSystem);

    _gameOver = false;
    _gameWon = false;
    _score = 0;
    _moveTimer = 0.0f;

    initGame();
}

void SnakeGame::initGame() {
    SnakeFactory factory(_entityManager, _componentManager);

    _snakeEntity = factory.createSnake(15, 15, Direction::RIGHT);

    spawnFood();
}

void SnakeGame::spawnFood() {
    SnakeFactory factory(_entityManager, _componentManager);

    auto snakeComponentBase =
        _componentManager->getComponentBase(_snakeEntity, "SnakeComponent");
    auto snakeComponent =
        dynamic_cast<SnakeComponent*>(snakeComponentBase.get());

    int x, y;
    bool validPosition;

    do {
        validPosition = true;
        x = rand() % 30;
        y = rand() % 30;

        for (auto& segment : snakeComponent->segments) {
            auto posCompBase =
            _componentManager->getComponentBase(segment, "PositionComponent");

            auto posComp = dynamic_cast<PositionComponent*>(posCompBase.get());
            if (posComp && posComp->x == x && posComp->y == y) {
                validPosition = false;
                break;
            }
        }
    } while (!validPosition);

    _foodEntity = factory.createFood(x, y);
}

void SnakeGame::update() {
    if (_gameOver || _gameWon)
        return;

    for (auto& system : _systems) {
        system->update();
    }

    auto snakeComponentBase =
        _componentManager->getComponentBase(_snakeEntity, "SnakeComponent");
    auto snakeComponent =
        dynamic_cast<SnakeComponent*>(snakeComponentBase.get());

    auto foodComponentBase =
        _componentManager->getComponentBase(_foodEntity, "FoodComponent");
    auto foodComponent = dynamic_cast<FoodComponent*>(foodComponentBase.get());

    if (snakeComponent && foodComponent && foodComponent->eaten) {
        _score += 10;

        _entityManager->destroyEntity(_foodEntity);
        spawnFood();

        if (_moveInterval > 0.05f) {
            _moveInterval -= 0.005f;
        }
    }
    if (_collisionSystem->hasCollision()) {
        _gameOver = true;
    }
}

bool SnakeGame::isGameOver() const {
    return _gameOver;
}

bool SnakeGame::hasWon() const {
    return _gameWon;
}

void SnakeGame::stop() {
    _systems.clear();
    _eventSystem.reset();
    _movementSystem.reset();
    _collisionSystem.reset();
    _renderSystem.reset();
}

int SnakeGame::getScore() const {
    return _score;
}

std::string SnakeGame::getSpecialCompSprite(size_t id) const {
    return "";
}

extern "C" {
    __attribute__((constructor))
    const char *init_snake(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_snake(void) {
    }

    Arcade::IArcadeModule* entryPoint(void) {
        return new Arcade::SnakeGame();
    }

    Arcade::ModuleInfos module_infos() {
        return {"Snake", "Arcade Snake Game",
            "Classic Snake game implementation",
            "./lib/arcade_snake.so", Arcade::ModuleType::GAME};
    }
}

}  // namespace Arcade
