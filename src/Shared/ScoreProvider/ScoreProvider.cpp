// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Score provider implementation
*/

#include "ScoreProvider/ScoreProvider.hpp"
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

namespace Arcade {

ScoreProvider::ScoreProvider() : _score(0), _playerName("Player") {}

ScoreProvider::ScoreProvider(const std::string& name) :
_score(0), _playerName(name) {}

int ScoreProvider::getScore() const {
    return _score;
}

void ScoreProvider::resetScore() {
    _score = 0;
}

void ScoreProvider::setScore(int score) {
    _score = score;
}

std::string ScoreProvider::getFormattedScore() const {
    std::ostringstream formatted;
    formatted << std::setw(8) << std::setfill('0') << _score;
    return formatted.str();
}

void ScoreProvider::addToScore(int add) {
    _score += add;
}

void ScoreProvider::setName(const std::string& name) {
    _playerName = name;
}

std::string ScoreProvider::getName() const {
    return _playerName;
}

}  // namespace Arcade
