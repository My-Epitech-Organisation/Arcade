// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Pacman Stats Component
*/

#ifndef SRC_GAMES_PACMAN_COMPONENTS_PACMANSTATS_HPP_
#define SRC_GAMES_PACMAN_COMPONENTS_PACMANSTATS_HPP_

#include <chrono>
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Models/ComponentType.hpp"

namespace Arcade {
namespace PacMan {

class PacmanStats : public Arcade::IComponent {
 public:
    explicit PacmanStats(int initialLives = 3);
    ~PacmanStats() = default;

    int getScore() const;
    void addScore(int points);
    int getLives() const;
    void decrementLives();
    void setLives(int lives);
    int getLevel() const;
    void incrementLevel();
    void resetGame();

    bool isPowerActive() const;
    float getPowerTimeRemaining() const;
    void activatePowerPill();
    void updatePowerTime(float deltaTime);

    int getHighScore() const;
    void updateHighScore();

    ComponentType getType() const override {
        return static_cast<ComponentType>(1005);
    }

 private:
    int _score;
    int _highScore;
    int _lives;
    int _level;
    bool _powerActive;
    float _powerDuration;
    float _powerTimeRemaining;
    std::chrono::time_point<std::chrono::steady_clock> _gameStartTime;
    static constexpr float POWER_DURATION = 10.0f;
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_COMPONENTS_PACMANSTATS_HPP_
