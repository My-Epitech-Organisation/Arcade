// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Score Manager
*/

#ifndef SRC_CORE_SCORE_SCOREMANAGER_HPP_
#define SRC_CORE_SCORE_SCOREMANAGER_HPP_

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>

namespace Arcade {

class ScoreManager {
 public:
    ScoreManager();
    ~ScoreManager() = default;

    void loadScores();

    void saveScores();

    void addScore(const std::string& gameName, int score);

    std::pair<std::string, int> getHighScore(const std::string& gameName) const;

    std::vector<std::pair<std::string, int>>
      getAllScores(const std::string& gameName) const;

    const std::string& getCurrentPlayerName() const;
    void setCurrentPlayerName(const std::string& name);

 private:
    std::unordered_map<std::string, std::vector<std::pair<std::string,
      int>>> _scores;

    std::string _currentPlayerName;

    const std::string _scoresFilePath = "scores.txt";
};

}  // namespace Arcade

#endif  // SRC_CORE_SCORE_SCOREMANAGER_HPP_
