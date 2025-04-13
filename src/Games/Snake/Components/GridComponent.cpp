// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Grid Component implementation for Snake game
*/

#include <iostream>
#include <random>
#include <chrono>
#include "Games/Snake/Components/GridComponent.hpp"

namespace Arcade {
namespace Snake {

GridComponent::GridComponent(size_t width, size_t height)
: _width(width), _height(height), _cellSize(0),
  _gameOver(false), _gameWon(false) {
    // Initialize the grid with empty cells
    _grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    _entities.resize(height, std::vector<Arcade::Entity>(width, 0));

    // Seed the random number generator
    unsigned seed = static_cast<unsigned>(
        std::chrono::system_clock::now().time_since_epoch().count());
    _rng = std::mt19937(seed);
}

CellType GridComponent::getCellType(size_t x, size_t y) const {
    if (x >= _width || y >= _height)
        return CellType::WALL;  // Out of bounds is treated as a wall
    return _grid[y][x];
}

void GridComponent::setCellType(size_t x, size_t y, CellType type) {
    if (x >= _width || y >= _height)
        return;  // Ignore if out of bounds
    _grid[y][x] = type;
}

Arcade::Entity GridComponent::getEntityAtCell(size_t x, size_t y) const {
    if (x >= _width || y >= _height)
        return 0;  // Return 0 (invalid entity) if out of bounds
    return _entities[y][x];
}

void GridComponent::setEntityAtCell(size_t x, size_t y, Arcade::Entity entity) {
    if (x >= _width || y >= _height)
        return;  // Ignore if out of bounds
    _entities[y][x] = entity;
}

std::pair<size_t, size_t> GridComponent::placeFood() {
    // Count empty cells
    int emptyCells = 0;
    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            if (_grid[y][x] == CellType::EMPTY)
                emptyCells++;
        }
    }

    if (emptyCells == 0) {
        // No empty cells available, return an invalid position
        return {_width, _height};
    }

    // Randomly select an empty cell
    std::uniform_int_distribution<int> dist(0, emptyCells - 1);
    int selectedCell = dist(_rng);

    // Find the selected empty cell
    int currentEmptyIndex = 0;
    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            if (_grid[y][x] == CellType::EMPTY) {
                if (currentEmptyIndex == selectedCell) {
                    // Found the selected cell, mark it as food
                    _grid[y][x] = CellType::FOOD;
                    return {x, y};
                }
                currentEmptyIndex++;
            }
        }
    }

    // Shouldn't reach here if there are empty cells
    return {_width, _height};
}

void GridComponent::resetGrid() {
    // Reset all cells to empty
    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            _grid[y][x] = CellType::EMPTY;
            _entities[y][x] = 0;
        }
    }

    // Reset game state
    _gameOver = false;
    _gameWon = false;
}

}  // namespace Snake
}  // namespace Arcade