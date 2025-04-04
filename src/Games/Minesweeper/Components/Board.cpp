// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Minesweeper Board implementation
*/
#include <stdexcept>
#include <vector>
#include "Games/Minesweeper/Components/Board.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"

namespace Arcade {
namespace Minesweeper {

Board::Board(size_t width, size_t height, size_t mineCount)
: _width(width), _height(height), _mineCount(mineCount),
_gameOver(false), _gameWon(false), _gameInitialized(false) {
    _cellEntities.resize(height, std::vector<Arcade::Entity>(width, 0));
}

void Board::addCellEntity(size_t x, size_t y, Arcade::Entity cellEntity) {
    if (x >= _width || y >= _height) {
        throw std::out_of_range("Cell position out of bounds");
    }
    _cellEntities[y][x] = cellEntity;
}

Arcade::Entity Board::getCellEntity(size_t x, size_t y) const {
    if (x >= _width || y >= _height) {
        throw std::out_of_range("Cell position out of bounds");
    }
    return _cellEntities[y][x];
}

}  // namespace Minesweeper
}  // namespace Arcade
