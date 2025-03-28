// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Minesweeper Board class
*/

#ifndef SRC_GAMES_MINESWEEPER_BOARD_HPP_
#define SRC_GAMES_MINESWEEPER_BOARD_HPP_

#include <vector>
#include "Games/Minesweeper/Cell.hpp"

namespace Arcade {
namespace Minesweeper {

class Board {
 public:
    Board(size_t width, size_t height, size_t mineCount);
    ~Board() = default;

    void initialize();
    bool revealCell(size_t x, size_t y);
    void flagCell(size_t x, size_t y);
    bool isGameOver() const;
    bool hasWon() const;
    size_t getWidth() const;
    size_t getHeight() const;
    const Cell& getCell(size_t x, size_t y) const;
    Cell& getCellMutable(size_t x, size_t y);

 private:
    void placeMines();
    void calculateAdjacentMines();
    void revealEmptyCells(size_t x, size_t y);
    bool isValidPosition(size_t x, size_t y) const;
    int countAdjacentMines(size_t x, size_t y) const;
    bool checkWinCondition() const;
    std::vector<std::vector<Cell>> _cells;
    size_t _width;
    size_t _height;
    size_t _mineCount;
    bool _gameOver;
    bool _gameWon;
};

}  // namespace Minesweeper
}  // namespace Arcade

#endif  // SRC_GAMES_MINESWEEPER_BOARD_HPP_
