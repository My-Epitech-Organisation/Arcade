// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GameStats
*/
#include "Games/Minesweeper/Components/GameStats.hpp"
#include <iostream>
#include <algorithm>

namespace Arcade {
namespace Minesweeper {

GameStats::GameStats(int totalMines, int timeLimit = 600) :
_totalMines(totalMines),
_flagsPlaced(0),
_safeCellsRevealed(0),
_score(0),
_highestScore(0),
_timeLimit(timeLimit),
_timeRemaining(timeLimit),
_startTime(std::chrono::steady_clock::now()) {}

int GameStats::getRemainingMines() const {
    return _totalMines - _flagsPlaced;
}

int GameStats::getScore() const {
    return _score;
}

int GameStats::getTimeRemaining() const {
    return _timeRemaining;
}

void GameStats::addFlag() {
    _flagsPlaced++;
}

void GameStats::removeFlag() {
    if (_flagsPlaced > 0) _flagsPlaced--;
}

void GameStats::revealSafeCell() {
    _safeCellsRevealed++;
    _score += 10;
}

void GameStats::updateTime() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
        now - _startTime).count();
    _timeRemaining = _timeLimit - static_cast<int>(elapsed);
    if (_timeRemaining < 0) _timeRemaining = 0;
}

bool GameStats::isTimeUp() const {
    return _timeRemaining <= 0;
}

void GameStats::addTimeBonus(int bonus) {
    _score += bonus;
}

int GameStats::getHighestScore() const {
    return std::max(_score, _highestScore);
}

void GameStats::updateHighestScore() {
    if (_score > _highestScore)
        _highestScore = _score;
}

void GameStats::reset() {
    updateHighestScore();
    _flagsPlaced = 0;
    _safeCellsRevealed = 0;
    _score = 0;
    _timeRemaining = _timeLimit;
    _startTime = std::chrono::steady_clock::now();
}

}  // namespace Minesweeper
}  // namespace Arcade
