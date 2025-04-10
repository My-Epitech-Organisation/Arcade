// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Pacman Stats Component implementation
*/

#include <algorithm>
#include "Games/PacMan/Components/PacmanStats.hpp"

namespace Arcade {
namespace PacMan {

PacmanStats::PacmanStats(int initialLives)
: _score(0),
_highScore(0),
_lives(initialLives),
_level(1),
_powerActive(false),
_powerDuration(POWER_DURATION),
_powerTimeRemaining(0.0f),
_gameStartTime(std::chrono::steady_clock::now()) {
}

int PacmanStats::getScore() const {
    return _score;
}

void PacmanStats::addScore(int points) {
    _score += points;
    updateHighScore();
}

int PacmanStats::getLives() const {
    return _lives;
}

void PacmanStats::decrementLives() {
    if (_lives > 0) _lives--;
}

void PacmanStats::setLives(int lives) {
    _lives = lives;
}

int PacmanStats::getLevel() const {
    return _level;
}

void PacmanStats::incrementLevel() {
    _level++;
}

void PacmanStats::resetGame() {
    updateHighScore();
    _score = 0;
    _lives = 3;
    _level = 1;
    _powerActive = false;
    _powerTimeRemaining = 0.0f;
    _gameStartTime = std::chrono::steady_clock::now();
}

bool PacmanStats::isPowerActive() const {
    return _powerActive;
}

float PacmanStats::getPowerTimeRemaining() const {
    return _powerTimeRemaining;
}

void PacmanStats::activatePowerPill() {
    _powerActive = true;
    _powerTimeRemaining = _powerDuration;
}

void PacmanStats::updatePowerTime(float deltaTime) {
    if (_powerActive) {
        _powerTimeRemaining -= deltaTime;
        if (_powerTimeRemaining <= 0) {
            _powerActive = false;
            _powerTimeRemaining = 0;
        }
    }
}

int PacmanStats::getHighScore() const {
    return _highScore;
}

void PacmanStats::updateHighScore() {
    if (_score > _highScore) {
        _highScore = _score;
    }
}

}  // namespace PacMan
}  // namespace Arcade
