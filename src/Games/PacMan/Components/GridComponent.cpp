// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Grid Component for Pacman implementation
*/

#include <stdexcept>
#include "Games/PacMan/Components/GridComponent.hpp"

namespace Arcade {
namespace PacMan {

GridComponent::GridComponent(size_t width, size_t height)
    : _width(width), _height(height), _foodCount(0), _gameOver(false), _gameWon(false) {
    _grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    _entityGrid.resize(height, std::vector<Arcade::Entity>(width, 0));
}

void GridComponent::setCellType(size_t x, size_t y, CellType type) {
    if (x >= _width || y >= _height) {
        throw std::out_of_range("Cell position out of bounds");
    }
    _grid[y][x] = type;
    if (type == CellType::FOOD || type == CellType::POWER_PILL) {
        _foodCount++;
    }
}

CellType GridComponent::getCellType(size_t x, size_t y) const {
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

void GridComponent::loadDefaultMap() {
    // Default PacMan map (small for simplicity)
    // W = Wall, F = Food, P = Power Pill, S = PacMan Spawn, G = Ghost Spawn, . = Empty
    const std::vector<std::string> map = {
        "WWWWWWWWWWWWWWWWWWWW",
        "WFFFFFFFFFFFFFFFFFFW",
        "WFWWWWFWWWWWWFWWWWFW",
        "WPWWWWFWWWWWWFWWWWPW",
        "WFFFFFFFFFFFFFFFFFFW",
        "WFWWWWFWWWWWWFWWWWFW",
        "FFFFFFFFFFFFFFFFFFPF",
        "WWWWWFWWWFWWWFWWWWWW",
        "WWWWWFWFFFFFWFWWWWWW",
        "WWWWWFWGGFGGWFWWWWWW",
        "WWWWWFWWWWWWWFWWWWWW",
        "FFFFFFFFFSFFFFFFFFFF",
        "WFWWWFWWWWWWWFWWWWFW",
        "WFWWWFWWWWWWWFWWWWFW",
        "WPFFFFFFFFFFFFFFFFPW",
        "WWWWWWWWWWWWWWWWWWWW"
    };

    _height = map.size();
    _width = map[0].size();
    _grid.resize(_height, std::vector<CellType>(_width, CellType::EMPTY));
    _entityGrid.resize(_height, std::vector<Arcade::Entity>(_width, 0));
    _foodCount = 0;

    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            if (x >= map[y].size()) {
                continue;
            }
            char cell = map[y][x];
            switch (cell) {
                case 'W':
                    _grid[y][x] = CellType::WALL;
                    break;
                case 'F':
                    _grid[y][x] = CellType::FOOD;
                    _foodCount++;
                    break;
                case 'P':
                    _grid[y][x] = CellType::POWER_PILL;
                    _foodCount++;
                    break;
                case 'S':
                    _grid[y][x] = CellType::PACMAN_SPAWN;
                    break;
                case 'G':
                    _grid[y][x] = CellType::GHOST_SPAWN;
                    break;
                default:
                    _grid[y][x] = CellType::EMPTY;
                    break;
            }
        }
    }
}

} // namespace PacMan
} // namespace Arcade