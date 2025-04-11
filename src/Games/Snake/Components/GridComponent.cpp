// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Grid Component for Snake implementation
*/

#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include "Games/Snake/Components/GridComponent.hpp"

namespace Arcade {

GridComponent::GridComponent(size_t width, size_t height)
: _width(width), _height(height), _gameOver(false),
_gameWon(false), _cellSize(20.0f) {
    _grid.resize(height, std::vector<SnakeCellType>(width, SnakeCellType::EMPTY));
    _entityGrid.resize(height, std::vector<Arcade::Entity>(width, 0));
}

void GridComponent::setCellType(size_t x, size_t y, SnakeCellType type) {
    if (x >= _width || y >= _height) {
        throw std::out_of_range("Cell position out of bounds");
    }
    _grid[y][x] = type;
}

SnakeCellType GridComponent::getCellType(size_t x, size_t y) const {
    if (x >= _width || y >= _height) {
        throw std::out_of_range("Cell position out of bounds");
    }
    return _grid[y][x];
}

void GridComponent::setEntityAtCell(size_t x, size_t y, Arcade::Entity entity) {
    if (x >= _width || y >= _height) {
        throw std::out_of_range("Cell position out of bounds");
    }
    _entityGrid[y][x] = entity;
}

Arcade::Entity GridComponent::getEntityAtCell(size_t x, size_t y) const {
    if (x >= _width || y >= _height) {
        throw std::out_of_range("Cell position out of bounds");
    }
    return _entityGrid[y][x];
}

void GridComponent::initializeEmptyGrid() {
    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            _grid[y][x] = SnakeCellType::EMPTY;
            _entityGrid[y][x] = 0;
        }
    }

    for (size_t x = 0; x < _width; x++) {
        _grid[0][x] = SnakeCellType::WALL;
        _grid[_height - 1][x] = SnakeCellType::WALL;
    }

    for (size_t y = 0; y < _height; y++) {
        _grid[y][0] = SnakeCellType::WALL;
        _grid[y][_width - 1] = SnakeCellType::WALL;
    }
}

bool GridComponent::isCellValid(size_t x, size_t y) const {
    if (x >= _width || y >= _height) {
        return false;
    }
    return _grid[y][x] == SnakeCellType::EMPTY;
}

void GridComponent::clearCell(size_t x, size_t y) {
    if (x >= _width || y >= _height) {
        return;
    }
    _grid[y][x] = SnakeCellType::EMPTY;
    _entityGrid[y][x] = 0;
}

}  // namespace Arcade