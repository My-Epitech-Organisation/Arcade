// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GameStats
*/

#ifndef SRC_GAMES_MINESWEEPER_COMPONENTS_GAMESTATS_HPP_
    #define SRC_GAMES_MINESWEEPER_COMPONENTS_GAMESTATS_HPP_
    #include <chrono>
    #include "Shared/Interface/ECS/IComponent.hpp"
    #include "Shared/Models/ComponentType.hpp"

namespace Arcade {
namespace Minesweeper {

class GameStats : public Arcade::IComponent {
 public:
    GameStats(int totalMines, int timeLimit);

    int getRemainingMines() const;
    int getScore() const;
    int getTimeRemaining() const;

    void addFlag();
    void removeFlag();
    void revealSafeCell();

    void updateTime();

    bool isTimeUp() const;

    void addTimeBonus(int bonus);

    void reset();

    ComponentType getType() const override {
        return ComponentType::CUSTOM_BASE;
    }

 private:
    int _totalMines;
    int _flagsPlaced;
    int _safeCellsRevealed;
    int _score;
    int _timeLimit;
    int _timeRemaining;
    std::chrono::time_point<std::chrono::steady_clock> _startTime;
};

}  // namespace Minesweeper
}  // namespace Arcade

#endif  // SRC_GAMES_MINESWEEPER_COMPONENTS_GAMESTATS_HPP_
