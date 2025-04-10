// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Score Manager implementation
*/

#include "Core/Score/ScoreManager.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <string>
#include <fstream>

namespace Arcade {

ScoreManager::ScoreManager() : _currentPlayerName("Default") {
    loadScores();
}

void ScoreManager::loadScores() {
    std::ifstream file(_scoresFilePath);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string gameName, playerName;
        int score;

        if (std::getline(ss, gameName, ':') &&
            std::getline(ss, playerName, ':') &&
            ss >> score) {
            _scores[gameName].push_back({playerName, score});
        }
    }
    file.close();
}

void ScoreManager::saveScores() {
    std::ofstream file(_scoresFilePath);
    if (!file.is_open()) {
        std::cerr << "Could not open scores file for writing" << std::endl;
        return;
    }

    for (const auto& gamePair : _scores) {
        const std::string& gameName = gamePair.first;
        for (const auto& scorePair : gamePair.second) {
            file << gameName << ":" << scorePair.first
                << ":" << scorePair.second << std::endl;
        }
    }
    file.close();
}

void ScoreManager::addScore(const std::string& gameName, int score) {
    _scores[gameName].push_back({_currentPlayerName, score});
    saveScores();
}

std::pair<std::string, int> ScoreManager::getHighScore(
const std::string& gameName) const {
    auto it = _scores.find(gameName);
    if (it == _scores.end() || it->second.empty()) {
        return {"Default", 0};
    }

    auto highestScore = std::max_element(
        it->second.begin(), it->second.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    return *highestScore;
}

std::vector<std::pair<std::string, int>> ScoreManager::getAllScores(
const std::string& gameName) const {
    auto it = _scores.find(gameName);
    if (it == _scores.end()) {
        return {};
    }
    return it->second;
}

const std::string& ScoreManager::getCurrentPlayerName() const {
    return _currentPlayerName;
}

void ScoreManager::setCurrentPlayerName(const std::string& name) {
    _currentPlayerName = name.empty() ? "Default" : name;
}

}  // namespace Arcade
