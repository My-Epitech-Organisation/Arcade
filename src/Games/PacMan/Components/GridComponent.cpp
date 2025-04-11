// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Grid Component for Pacman implementation
*/

#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include "Games/PacMan/Components/GridComponent.hpp"

namespace Arcade {
namespace PacMan {

GridComponent::GridComponent(size_t width, size_t height)
: _width(width), _height(height), _foodCount(0), _gameOver(false),
_gameWon(false), _cellSize(0.0f) {
    _grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    _entityGrid.resize(height,
        std::vector<std::shared_ptr<Arcade::IEntity>>(width, 0));
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

void GridComponent::setEntityAtCell(size_t x, size_t y,
    std::shared_ptr<Arcade::IEntity> entity) {
    if (x >= _width || y >= _height) {
        throw std::out_of_range("Cell position out of bounds");
    }
    _entityGrid[y][x] = entity;
}

std::shared_ptr<Arcade::IEntity>
GridComponent::getEntityAtCell(size_t x, size_t y) const {
    if (x >= _width || y >= _height) {
        throw std::out_of_range("Cell position out of bounds");
    }
    return _entityGrid[y][x];
}

void GridComponent::loadDefaultMap() {
    // W = Wall, F = Food, P = Power Pill,
    // S = PacMan Spawn, G = Ghost Spawn, . = Empty
    const std::vector<std::string> map = {
        "WWWWWWWWWWWWWWWWWWWWWWWWWWW",
        "WFFFFFFFFFFFFWFFFFFFFFFFFFW",
        "WFWWWWFWWWWWFWFWWWWWFWWWWFW",
        "WPWWWWFWWWWWFWFWWWWWFWWWWPW",
        "WFWWWWFWWWWWFWFWWWWWFWWWWFW",
        "WFFFFFFFFFFFFFFFFFFFFFFFFFW",
        "WFWWWWFWWFWWWWWWWFWWFWWWWFW",
        "WFWWWWFWWFWWWWWWWFWWFWWWWFW",
        "WFFFFFFWWFFFFWFFFFWWFFFFFFW",
        "WWWWWWFWWWWW.W.WWWWWFWWWWWW",
        "WWWWWWFWWWWW.W.WWWWWFWWWWWW",
        "WWWWWWFWW.........WWFWWWWWW",
        "WWWWWWFWW.WWW.WWW.WWFWWWWWW",
        "WWWWWWFWW.WGGGGGW.WWFWWWWWW",
        "FFFFFFF....GGGGG....FFFFFFF",
        "WWWWWWFWW.WGGGGGW.WWFWWWWWW",
        "WWWWWWFWW.WWW.WWW.WWFWWWWWW",
        "WWWWWWFWW....S....WWFWWWWWW",
        "WWWWWWFWW.........WWFWWWWWW",
        "WWWWWWFWW.WWWWWWW.WWFWWWWWW",
        "WFFFFFFFFFFFFWFFFFFFFFFFFFW",
        "WFWWWWFWWWWWFWFWWWWWFWWWWFW",
        "WFWWWWFWWWWWFWFWWWWWFWWWWFW",
        "WPFFWWFFFFFFFFFFFFFFFWWFFPW",
        "WWWFWWFWWFWWWWWWWFWWFWWFWWW",
        "WWWFWWFWWFWWWWWWWFWWFWWFWWW",
        "WFFFFFFWWFFFFWFFFFWWFFFFFFW",
        "WFWWWWWWWWWWFWFWWWWWWWWWWFW",
        "WFWWWWWWWWWWFWFWWWWWWWWWWFW",
        "WFFFFFFFFFFFFFFFFFFFFFFFFFW",
        "WWWWWWWWWWWWWWWWWWWWWWWWWWW"
    };

    size_t expected_width = map[0].size();
    for (const auto& row : map) {
        if (row.size() != expected_width) {
            throw std::runtime_error("Map has inconsistent row lengths");
        }
    }

    _grid.clear();
    _entityGrid.clear();
    _foodCount = 0;
    _height = map.size();
    _width = expected_width;
    _grid.resize(_height, std::vector<CellType>(_width, CellType::EMPTY));
    _entityGrid.resize(_height,
        std::vector<std::shared_ptr<Arcade::IEntity>>(_width, 0));

    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
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
    _totalFoodCount = _foodCount;
}

}  // namespace PacMan
}  // namespace Arcade
