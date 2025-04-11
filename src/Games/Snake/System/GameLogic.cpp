// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game logic system implementation for Snake game
*/

#include "Games/Snake/System/GameLogic.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Games/Snake/Components/Board.hpp"
#include "Games/Snake/Components/GameStats.hpp"

namespace Arcade {

GameLogic::GameLogic(std::shared_ptr<IComponentManager> componentManager,
                   std::shared_ptr<IEntityManager> entityManager)
    : _componentManager(componentManager),
      _entityManager(entityManager) {
}

void GameLogic::update() {
    checkVictoryCondition();
}

void GameLogic::checkVictoryCondition() {
    // In Snake, the game doesn't technically have a victory condition
    // But we could consider reaching a certain length or score as a win

    auto statsEntities = _entityManager->getEntitiesWithComponent<GameStats>();
    if (statsEntities.empty()) return;

    auto stats = _componentManager->getComponent<GameStats>(statsEntities[0]);

    // Check if score reached win threshold (e.g., 500 points)
    // This is optional as Snake traditionally doesn't have a "win" condition
    // const int WIN_SCORE = 500;
    // if (stats->score >= WIN_SCORE) {
    //     // Set win flag in the game stats
    //     stats->won = true;
    // }

    // Alternatively, check snake length
    auto snakeEntities = _entityManager->getEntitiesWithComponent<Snake>();
    if (snakeEntities.empty()) return;

    auto snake = _componentManager->getComponent<Snake>(snakeEntities[0]);

    // Get board dimensions
    auto boardEntities = _entityManager->getEntitiesWithComponent<Board>();
    if (boardEntities.empty()) return;

    auto board = _componentManager->getComponent<Board>(boardEntities[0]);

    // If snake fills almost the entire board (e.g., 90%), consider it a win
    const float WIN_THRESHOLD = 0.9f;
    int boardSize = board->width * board->height;
    float fillRatio = static_cast<float>(snake->segments.size()) / boardSize;

    if (fillRatio >= WIN_THRESHOLD) {
        // Set win flag in the stats
        // For now we don't have a win flag in GameStats, could add it if needed
    }
}

}  // namespace Arcade
