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
#include "Games/Snake/System/EventSubSystem.hpp"
#include "Games/Snake/Components/Board.hpp"
#include "Games/Snake/SnakeFactory.hpp"
#include "Games/Snake/System/UISystem.hpp"
#include "Games/Snake/Components/GameStats.hpp"
#include "Games/Snake/System/MovementSystem.hpp"

namespace Arcade {

class SnakeGame : public IGameModule {
 public:
    SnakeGame() :
        _gameOver(false),
        _gameWon(false),
        _score(0) {}

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
    void createBoard();
    void createSnake();
    void spawnFood();
    void updateSnakeVisuals();

    std::shared_ptr<IEventManager> _eventManager;
    std::shared_ptr<IComponentManager> _componentManager;
    std::shared_ptr<IEntityManager> _entityManager;
    std::shared_ptr<EventSubSystem> _eventSystem;
    std::vector<std::shared_ptr<ISystem>> _systems;
    bool _gameOver;
    bool _gameWon;
    int _score;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SNAKE_HPP_
