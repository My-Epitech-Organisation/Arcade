// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** ScoreManager
*/
#include "ScoreManager/ScoreManager.hpp"
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>

void scoreManager::storeScore(std::unique_ptr<Arcade::IGameModule> game) {
    const std::string folderPath = "scores";
    const std::string filePath = folderPath + "/score.txt";

    struct stat info;
    if (stat(folderPath.c_str(), &info) != 0) {
        int result = mkdir(folderPath.c_str(), 0644);
        if (result != 0) {
            std::cerr << "Error: Could not create directory '" <<
                folderPath << "'" << std::endl;
            return;
        }
    } else if (!(info.st_mode & S_IFDIR)) {
        std::cerr << "Error: '" << folderPath <<
            "' exists but is not a directory" << std::endl;
        return;
    }
    std::ofstream scoreFile(filePath, std::ios::app);

    if (!scoreFile.is_open()) {
        std::cerr << "Error: Could not open or create file '"
            << filePath << "'" << std::endl;
        return;
    }

    std::string gameName = game->getName();
    int score = game->getScore();
    // std::string playerName = game->getPlayerName();

    scoreFile << gameName << "," /*<< playerName*/ << "," <<
        score << std::endl;

    scoreFile.close();
    std::cout << "Score successfully saved to " << filePath << std::endl;
}

std::size_t scoreManager::getScore(std::unique_ptr<Arcade::IGameModule> game) {
    std::ifstream scoreFile("scores/score.txt");
    if (!scoreFile.is_open()) {
        std::cerr <<
            "Error: Could not open or create file 'scores/score.txt'"
            << std::endl;
        return;
    }
    std::string gameName = game->getName();
    std::size_t highestScore = 0;
    std::string line;

    while (std::getline(scoreFile, line)) {
        if (line.empty())
            continue;

        std::size_t firstComma = line.find(',');
        if (firstComma == std::string::npos)
            continue;

        std::string currentGameName = line.substr(0, firstComma);

        if (currentGameName == gameName) {
            std::size_t lastComma = line.find_last_of(',');
            if (lastComma != std::string::npos && lastComma + 1
                    < line.length()) {
                try {
                    std::size_t currentScore =
                        std::stoul(line.substr(lastComma + 1));
                    if (currentScore > highestScore)
                        highestScore = currentScore;
                } catch (const std::exception& e) {
                    continue;
                }
            }
        }
    }
    scoreFile.close();
    return highestScore;
}

std::vector<std::pair<std::string, std::size_t>> scoreManager::getAllScore() {
    const std::string filePath = "scores/score.txt";
    std::vector<std::pair<std::string, std::size_t>> allScores;
    std::map<std::string, std::size_t> highestScores;

    std::ifstream scoreFile(filePath);
    if (!scoreFile.is_open()) {
        std::cerr << "Error: Could not open file '" <<
            filePath << "'" << std::endl;
        return allScores;
    }

    std::string line;
    while (std::getline(scoreFile, line)) {
        if (line.empty())
            continue;

        std::size_t firstComma = line.find(',');
        if (firstComma == std::string::npos)
            continue;

        std::string gameName = line.substr(0, firstComma);

        std::size_t lastComma = line.find_last_of(',');
        if (lastComma != std::string::npos && lastComma + 1 < line.length()) {
            try {
                std::size_t score = std::stoul(line.substr(lastComma + 1));
                if (highestScores.find(gameName) == highestScores.end() ||
                    score > highestScores[gameName]) {
                    highestScores[gameName] = score;
                }
            } catch (const std::exception& e) {
                continue;
            }
        }
    }
    scoreFile.close();

    for (const auto& [game, score] : highestScores) {
        allScores.emplace_back(game, score);
    }
    return allScores;
}
