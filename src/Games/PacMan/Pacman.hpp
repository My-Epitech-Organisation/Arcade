// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Pacman game module
*/

#ifndef SRC_GAMES_PACMAN_PACMAN_HPP_
#define SRC_GAMES_PACMAN_PACMAN_HPP_

#include <memory>
#include <vector>
#include <string>
#include "Shared/Interface/Game/IGameModule.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"
#include "Games/PacMan/System/EventSubSystem.hpp"
#include "Games/PacMan/System/GameLogic.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/PacmanFactory.hpp"

namespace Arcade {
namespace PacMan {

class PacmanGame : public IGameModule {
public:
    PacmanGame() : _gameOver(false), _gameWon(false) {}
    ~PacmanGame() override;

    void init(std::shared_ptr<IEventManager> eventManager,
             std::shared_ptr<IComponentManager> componentManager,
             std::shared_ptr<IEntityManager> entityManager) override;
    void update() override;
    bool isGameOver() const override;
    bool hasWon() const override;
    void stop() override;
    std::string getSpecialCompSprite(size_t id) const override;

private:
    void createGame();
    void checkGameStatus();

    std::shared_ptr<IEventManager> _eventManager;
    std::shared_ptr<IComponentManager> _componentManager;
    std::shared_ptr<IEntityManager> _entityManager;
    std::shared_ptr<EventSubSystem> _eventSystem;
    std::vector<std::shared_ptr<ISystem>> _systems;
    bool _gameOver;
    bool _gameWon;
};

} // namespace PacMan
} // namespace Arcade

#endif // SRC_GAMES_PACMAN_PACMAN_HPP_