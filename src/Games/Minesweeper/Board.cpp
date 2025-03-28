// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Minesweeper Board implementation
*/

#include "Games/Minesweeper/Board.hpp"
#include <random>
#include <algorithm>

namespace Arcade {
namespace Minesweeper {

Board::Board(size_t width, size_t height, size_t mineCount)
    : _width(width), _height(height), _mineCount(mineCount),
      _gameOver(false), _gameWon(false) {
    _cells.resize(_height);
    for (auto& row : _cells) {
        row.resize(_width);
    }
}

void Board::initialize() {
    _gameOver = false;
    _gameWon = false;

    // Reset cells
    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            _cells[y][x] = Cell();
        }
    }

    placeMines();
    calculateAdjacentMines();
}

bool Board::revealCell(size_t x, size_t y) {
    if (!isValidPosition(x, y) || _gameOver || _cells[y][x].isRevealed() ||
        _cells[y][x].isFlagged()) {
        return false;
    }

    _cells[y][x].reveal();

    if (_cells[y][x].isMine()) {
        _gameOver = true;
        // Reveal all mines
        for (size_t cy = 0; cy < _height; ++cy) {
            for (size_t cx = 0; cx < _width; ++cx) {
                if (_cells[cy][cx].isMine()) {
                    _cells[cy][cx].reveal();
                }
            }
        }
        return true;
    }

    if (_cells[y][x].getAdjacentMines() == 0) {
        revealEmptyCells(x, y);
    }

    if (checkWinCondition()) {
        _gameWon = true;
    }

    return true;
}

void Board::flagCell(size_t x, size_t y) {
    if (!isValidPosition(x, y) || _gameOver || _cells[y][x].isRevealed()) {
        return;
    }

    _cells[y][x].toggleFlag();
    
    if (checkWinCondition()) {
        _gameWon = true;
    }
}

bool Board::isGameOver() const {
    return _gameOver;
}

bool Board::hasWon() const {
    return _gameWon;
}

size_t Board::getWidth() const {
    return _width;
}

size_t Board::getHeight() const {
    return _height;
}

const Cell& Board::getCell(size_t x, size_t y) const {
    return _cells[y][x];
}

Cell& Board::getCellMutable(size_t x, size_t y) {
    return _cells[y][x];
}

void Board::placeMines() {
    std::vector<std::pair<size_t, size_t>> positions;
    
    // Create a vector of all positions
    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            positions.emplace_back(x, y);
        }
    }
    
    // Shuffle positions
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(positions.begin(), positions.end(), g);
    
    // Place mines
    for (size_t i = 0; i < _mineCount && i < positions.size(); ++i) {
        const auto& [x, y] = positions[i];
        _cells[y][x].setMine(true);
    }
}

void Board::calculateAdjacentMines() {
    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            if (!_cells[y][x].isMine()) {
                _cells[y][x].setAdjacentMines(countAdjacentMines(x, y));
            }
        }
    }
}

void Board::revealEmptyCells(size_t x, size_t y) {
    if (!isValidPosition(x, y) || _cells[y][x].isRevealed() || _cells[y][x].isMine() ||
        _cells[y][x].isFlagged()) {
        return;
    }

    _cells[y][x].reveal();

    if (_cells[y][x].getAdjacentMines() == 0) {
        // Recursively reveal adjacent cells
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;
                int nx = static_cast<int>(x) + dx;
                int ny = static_cast<int>(y) + dy;
                if (nx >= 0 && ny >= 0 && 
                    static_cast<size_t>(nx) < _width && 
                    static_cast<size_t>(ny) < _height) {
                    revealEmptyCells(nx, ny);
                }
            }
        }
    }
}

bool Board::isValidPosition(size_t x, size_t y) const {
    return x < _width && y < _height;
}

int Board::countAdjacentMines(size_t x, size_t y) const {
    int count = 0;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;

            int nx = static_cast<int>(x) + dx;
            int ny = static_cast<int>(y) + dy;

            if (nx >= 0 && ny >= 0 && 
                static_cast<size_t>(nx) < _width && 
                static_cast<size_t>(ny) < _height && 
                _cells[ny][nx].isMine()) {
                count++;
            }
        }
    }

    return count;
}

bool Board::checkWinCondition() const {
    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            const Cell& cell = _cells[y][x];
            
            // If there's a non-mine cell that's not revealed, the game is not won
            if (!cell.isMine() && !cell.isRevealed()) {
                return false;
            }
            
            // If there's a mine that's not correctly flagged, the game is not won
            if (cell.isMine() && !cell.isFlagged() && !cell.isRevealed()) {
                return false;
            }
        }
    }
    
    return true;
}

}  // namespace Minesweeper
}  // namespace Arcade
