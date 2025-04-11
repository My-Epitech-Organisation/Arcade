// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake game module implementation
*/

#include <string>
#include <memory>
#include <iostream>
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

    std::shared_ptr<SnakeComponent> snakeComponent = nullptr;

    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "snake" && entity == _snakeEntity) {
            auto components = _componentManager->getEntityComponents(entity);
            for (const auto& component : components) {
                auto castComponent = std::dynamic_pointer_cast<SnakeComponent>(component);
                if (castComponent) {
                    snakeComponent = castComponent;
                    break;
                }
            }
            break;
        }
    }

    if (!snakeComponent) {
        _foodEntity = factory.createFood(rand() % 30, rand() % 30);
        return;
    }

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

    std::shared_ptr<SnakeComponent> snakeComponent = nullptr;
    std::shared_ptr<FoodComponent> foodComponent = nullptr;

    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "snake" && entity == _snakeEntity) {
            auto components = _componentManager->getEntityComponents(entity);
            for (const auto& component : components) {
                auto castComponent = std::dynamic_pointer_cast<SnakeComponent>(component);
                if (castComponent) {
                    snakeComponent = castComponent;
                    break;
                }
            }
            break;
        }
    }

    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "food" && entity == _foodEntity) {
            auto components = _componentManager->getEntityComponents(entity);
            for (const auto& component : components) {
                auto castComponent = std::dynamic_pointer_cast<FoodComponent>(component);
                if (castComponent) {
                    foodComponent = castComponent;
                    break;
                }
            }
            break;
        }
    }

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
    auto snakeCompBase = _componentManager->getComponentBase(id, "SnakeComponent");
    auto snakeComp = dynamic_cast<SnakeComponent*>(snakeCompBase.get());
    if (snakeComp) {
        switch (snakeComp->direction) {
            case Direction::UP:
                return "assets/snake/head_up.png";
            case Direction::DOWN:
                return "assets/snake/head_down.png";
            case Direction::LEFT:
                return "assets/snake/head_left.png";
            case Direction::RIGHT:
                return "assets/snake/head_right.png";
        }
    }

    auto foodCompBase = _componentManager->getComponentBase(id, "FoodComponent");
    auto foodComp = dynamic_cast<FoodComponent*>(foodCompBase.get());
    if (foodComp) {
        return "assets/snake/apple.png";
    }

    for (const auto& [entity, _] : _entityManager->getEntities()) {
        auto snakeComponentBase = _componentManager->getComponentBase(entity, "SnakeComponent");
        auto snakeComponent = dynamic_cast<SnakeComponent*>(snakeComponentBase.get());
        if (snakeComponent) {
            for (size_t i = 0; i < snakeComponent->segments.size(); ++i) {
                if (snakeComponent->segments[i] == id) {
                    if (i == 0) {
                        switch (snakeComponent->direction) {
                            case Direction::UP:
                                return "assets/snake/head_up.png";
                            case Direction::DOWN:
                                return "assets/snake/head_down.png";
                            case Direction::LEFT:
                                return "assets/snake/head_left.png";
                            case Direction::RIGHT:
                                return "assets/snake/head_right.png";
                        }
                    } else if (i == snakeComponent->segments.size() - 1) {
                        auto tailPosBase = _componentManager->getComponentBase(id, "PositionComponent");
                        auto tailPos = dynamic_cast<PositionComponent*>(tailPosBase.get());

                        auto prevSegmentPosBase = _componentManager->getComponentBase(
                            snakeComponent->segments[i - 1], "PositionComponent");
                        auto prevPos = dynamic_cast<PositionComponent*>(prevSegmentPosBase.get());

                        if (tailPos && prevPos) {
                            if (tailPos->x < prevPos->x) {
                                return "assets/snake/tail_left.png";
                            } else if (tailPos->x > prevPos->x) {
                                return "assets/snake/tail_right.png";
                            } else if (tailPos->y < prevPos->y) {
                                return "assets/snake/tail_up.png";
                            } else {
                                return "assets/snake/tail_down.png";
                            }
                        }
                    } else {
                        return "assets/snake/body_horizontal.png";
                    }
                    break;
                }
            }
        }
    }

    return "";
}

extern "C" {
    Arcade::IArcadeModule* entryPoint(void) {
        try {
            return new Arcade::SnakeGame();
        } catch (const std::exception& e) {
            std::cerr << "Error creating Snake instance: "
                << e.what() << std::endl;
            return nullptr;
        }
    }

    void destroy(IGameModule* instance) {
        delete instance;
    }

    Arcade::ModuleInfos module_infos() {
        return {"Snake", "Arcade Snake Game",
            "Classic Snake game implementation",
            "./lib/arcade_snake.so", Arcade::ModuleType::GAME};
    }
}

}  // namespace Arcade
