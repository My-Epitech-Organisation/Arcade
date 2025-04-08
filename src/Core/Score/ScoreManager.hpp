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

    // Load scores from file
    void loadScores();
    
    // Save scores to file
    void saveScores();
    
    // Add a score for the current player and game
    void addScore(const std::string& gameName, int score);
    
    // Get the highest score and player name for a specific game
    std::pair<std::string, int> getHighScore(const std::string& gameName) const;
    
    // Get all scores for a specific game
    std::vector<std::pair<std::string, int>> getAllScores(const std::string& gameName) const;
    
    // Get/Set current player name
    const std::string& getCurrentPlayerName() const;
    void setCurrentPlayerName(const std::string& name);

 private:
    // Map of game names to vectors of player name/score pairs
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> _scores;
    
    // Current player name
    std::string _currentPlayerName;
    
    // Path to scores file
    const std::string _scoresFilePath = "scores.txt";
};

}  // namespace Arcade

#endif  // SRC_CORE_SCORE_SCOREMANAGER_HPP_
