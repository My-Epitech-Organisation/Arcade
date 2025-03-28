// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Minesweeper Cell implementation
*/

#include "Games/Minesweeper/Cell.hpp"

namespace Arcade {
namespace Minesweeper {

Cell::Cell() : _isMine(false), _state(HIDDEN), _adjacentMines(0) {}

void Cell::setMine(bool isMine) {
    _isMine = isMine;
}

bool Cell::isMine() const {
    return _isMine;
}

bool Cell::isRevealed() const {
    return _state == REVEALED;
}

bool Cell::isFlagged() const {
    return _state == FLAGGED;
}

void Cell::setAdjacentMines(int count) {
    _adjacentMines = count;
}

int Cell::getAdjacentMines() const {
    return _adjacentMines;
}

void Cell::reveal() {
    if (_state != FLAGGED) {
        _state = REVEALED;
    }
}

void Cell::toggleFlag() {
    if (_state == HIDDEN) {
        _state = FLAGGED;
    } else if (_state == FLAGGED) {
        _state = HIDDEN;
    }
}

Cell::State Cell::getState() const {
    return _state;
}

char Cell::getDisplayChar() const {
    if (_state == FLAGGED) {
        return 'F';
    } else if (_state == HIDDEN) {
        return '#';
    } else if (_isMine) {
        return '*';
    } else if (_adjacentMines == 0) {
        return ' ';
    } else {
        return '0' + _adjacentMines;
    }
}

}  // namespace Minesweeper
}  // namespace Arcade
