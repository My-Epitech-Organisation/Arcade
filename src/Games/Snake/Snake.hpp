// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake game module
*/

#ifndef SRC_GAMES_SNAKE_SNAKE_HPP_
#define SRC_GAMES_SNAKE_SNAKE_HPP_

#include <memory>
#include <vector>
#include <string>
#include "Shared/Interface/Game/IGameModule.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"
#include "Games/Snake/System/EventSystem.hpp"
#include "Games/Snake/System/MovementSystem.hpp"
#include "Games/Snake/System/CollisionSystem.hpp"
#include "Games/Snake/System/RenderSystem.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Games/Snake/Components/Food.hpp"
#include "Games/Snake/SnakeFactory.hpp"

namespace Arcade {

class SnakeGame : public IGameModule {
 public:
    SnakeGame() :
        _gameOver(false),
        _gameWon(false),
        _score(0),
        _moveTimer(0.0f),
        _moveInterval(0.2f) {}

    ~SnakeGame() override;

    void init(std::shared_ptr<IEventManager> eventManager,
        std::shared_ptr<IComponentManager> componentManager,
        std::shared_ptr<IEntityManager> entityManager) override;
    void update() override;
    bool isGameOver() const override;
    bool hasWon() const override;
    void stop() override;
    int getScore() const override;
    std::string getSpecialCompSprite(size_t id) const override;

 private:
    void initGame();
    void spawnFood();

    std::shared_ptr<IEventManager> _eventManager;
    std::shared_ptr<IComponentManager> _componentManager;
    std::shared_ptr<IEntityManager> _entityManager;
    std::shared_ptr<EventSystem> _eventSystem;
    std::shared_ptr<MovementSystem> _movementSystem;
    std::shared_ptr<CollisionSystem> _collisionSystem;
    std::shared_ptr<RenderSystem> _renderSystem;
    std::vector<std::shared_ptr<ISystem>> _systems;
    bool _gameOver;
    bool _gameWon;
    int _score;
    float _moveTimer;
    float _moveInterval;
    Arcade::Entity _snakeEntity;
    Arcade::Entity _foodEntity;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SNAKE_HPP_
