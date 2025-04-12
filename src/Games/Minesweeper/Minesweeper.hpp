// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Minesweeper game module
*/

#ifndef SRC_GAMES_MINESWEEPER_MINESWEEPER_HPP_
#define SRC_GAMES_MINESWEEPER_MINESWEEPER_HPP_

#include <memory>
#include <vector>
#include <string>
#include <map>
#include "Shared/Interface/Game/IGameModule.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/ECS/IEntity.hpp"
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"
#include "Games/Minesweeper/System/EventSubSystem.hpp"
#include "Games/Minesweeper/Components/Board.hpp"
#include "Games/Minesweeper/MinesweeperFactory.hpp"
#include "Games/Minesweeper/System/UISystem.hpp"
#include "Games/Minesweeper/Components/GameStats.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"


namespace Arcade {
class MinesweeperGame : public IGameModule {
 public:
    MinesweeperGame() :
        _gameOver(false),
        _gameWon(false) {}
    ~MinesweeperGame() noexcept override;

    void init(std::shared_ptr<IEventManager> eventManager,
        std::shared_ptr<IComponentManager> componentManager,
        std::shared_ptr<IEntityManager> entityManager) override;
    void update() override;
    bool isGameOver() const override;
    bool hasWon() const override;
    void stop() override;
    int getScore() const override;
    void setScoreProvider(
        std::shared_ptr<IScoreProvider> scoreProvider) override {
          _scoreProvider = scoreProvider;
        }

 private:
    void createBoard();
    bool checkVictory(std::shared_ptr<IEntity> boardEntity);
    void loadDrawableAssets();
    std::shared_ptr<DrawableComponent>
        getDrawableAsset(const std::string& key) const;
    std::shared_ptr<IEventManager> _eventManager;
    std::shared_ptr<IComponentManager> _componentManager;
    std::shared_ptr<IEntityManager> _entityManager;
    std::shared_ptr<EventSubSystem> _eventSystem;
    std::vector<std::shared_ptr<ISystem>> _systems;
    std::shared_ptr<IScoreProvider> _scoreProvider;
    std::map<std::string, DrawableComponent> _drawableAssets;
    bool _gameOver;
    bool _gameWon;
};

}  // namespace Arcade

#endif  // SRC_GAMES_MINESWEEPER_MINESWEEPER_HPP_
