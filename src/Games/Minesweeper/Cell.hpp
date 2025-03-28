// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Minesweeper Cell class
*/

#ifndef SRC_GAMES_MINESWEEPER_CELL_HPP_
#define SRC_GAMES_MINESWEEPER_CELL_HPP_

namespace Arcade {
namespace Minesweeper {

class Cell {
 public:
    Cell();
    ~Cell() = default;

    enum State {
        HIDDEN,
        REVEALED,
        FLAGGED
    };

    void setMine(bool isMine);
    bool isMine() const;
    bool isRevealed() const;
    bool isFlagged() const;
    void setAdjacentMines(int count);
    int getAdjacentMines() const;
    void reveal();
    void toggleFlag();
    State getState() const;
    char getDisplayChar() const;

 private:
    bool _isMine;
    State _state;
    int _adjacentMines;
};

}  // namespace Minesweeper
}  // namespace Arcade

#endif  // SRC_GAMES_MINESWEEPER_CELL_HPP_
