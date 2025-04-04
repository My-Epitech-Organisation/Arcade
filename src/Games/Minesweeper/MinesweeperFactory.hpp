// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Factory for creating Minesweeper game entities
*/

#ifndef SRC_GAMES_MINESWEEPER_MINESWEEPERFACTORY_HPP_
#define SRC_GAMES_MINESWEEPER_MINESWEEPERFACTORY_HPP_

#include <memory>
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Games/Minesweeper/Components/Board.hpp"
#include "Games/Minesweeper/Components/Cell.hpp"
#include "Games/Minesweeper/Components/Bomb.hpp"

namespace Arcade {
namespace Minesweeper {

class MinesweeperFactory {
 public:
    MinesweeperFactory(
        std::shared_ptr<Arcade::IEntityManager> entityManager,
        std::shared_ptr<Arcade::IComponentManager> componentManager)
        : _entityManager(entityManager), _componentManager(componentManager) {}

    Arcade::Entity createBoard(float x, float y, size_t width,
        size_t height, size_t mineCount);
    Arcade::Entity createCell(float x, float y, size_t gridX,
        size_t gridY, bool hasMine);
    void initializeGame(Arcade::Entity boardEntity, float startX,
        float startY, float cellSize);

 private:
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
};

}  // namespace Minesweeper
}  // namespace Arcade

#endif  // SRC_GAMES_MINESWEEPER_MINESWEEPERFACTORY_HPP_
