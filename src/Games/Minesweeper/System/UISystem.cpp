// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** UISystem
*/

#include <string>
#include <sstream>
#include <memory>
#include <iomanip>
#include "Games/Minesweeper/System/UISystem.hpp"
#include "Games/Minesweeper/Components/Board.hpp"
#include "Games/Minesweeper/Components/GameStats.hpp"
#include "ECS/Components/Text/TextComponent.hpp"

namespace Arcade {

void UISystem::createUIEntities() {
    _minesTextEntity = _entityManager->createEntity("UI_MinesText");
    auto minesText = std::make_shared<TextComponent>("Mines: 0",
        10, 10, Color::BLACK);
    _componentManager->registerComponent(_minesTextEntity, minesText);

    _timeTextEntity = _entityManager->createEntity("UI_TimeText");
    auto timeText = std::make_shared<TextComponent>("Time: 00:00",
        10, 40, Color::BLACK);
    _componentManager->registerComponent(_timeTextEntity, timeText);

    _scoreTextEntity = _entityManager->createEntity("UI_ScoreText");
    auto scoreText = std::make_shared<TextComponent>("Score: 0",
        10, 70, Color::BLACK);
    _componentManager->registerComponent(_scoreTextEntity, scoreText);

    _gameOverTextEntity = _entityManager->createEntity("UI_GameOverText");
    auto gameOverText = std::make_shared<TextComponent>("GAME OVER",
        350, 300, Color::RED);
    gameOverText->visible = false;
    _componentManager->registerComponent(_gameOverTextEntity, gameOverText);

    _gameResultTextEntity = _entityManager->createEntity("UI_GameResultText");
    auto resultText = std::make_shared<TextComponent>("",
        330, 330, Color::GREEN);
    resultText->visible = false;
    _componentManager->registerComponent(_gameResultTextEntity, resultText);

    _restartTextEntity = _entityManager->createEntity("UI_RestartText");
    auto restartText = std::make_shared<TextComponent>(
        "Press 'R' to restart or ESC for menu", 250, 360, Color::WHITE);
    restartText->visible = false;
    _componentManager->registerComponent(_restartTextEntity, restartText);
}

void UISystem::update() {
    Arcade::Entity boardEntity = 0;
    for (const auto& entity : _entityManager->getEntities()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }

    auto statsComp = _componentManager->getComponentByType(
        boardEntity, ComponentType::CUSTOM_BASE);
    auto gameStats = std::dynamic_pointer_cast
        <Arcade::Minesweeper::GameStats>(statsComp);

    auto boardComp = _componentManager->getComponentByType(
        boardEntity, ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>
        (boardComp);

    if (!gameStats || !board) return;

    auto minesTextComp = _componentManager->getComponentByType(
        _minesTextEntity, ComponentType::TEXT);
    auto minesText = std::dynamic_pointer_cast<TextComponent>(minesTextComp);
    if (minesText)
        minesText->text = "Mines: " +
            std::to_string(gameStats->getRemainingMines());

    auto timeTextComp = _componentManager->getComponentByType(
        _timeTextEntity, ComponentType::TEXT);
    auto timeText = std::dynamic_pointer_cast<TextComponent>(timeTextComp);
    if (timeText) {
        int seconds = gameStats->getTimeRemaining();
        int minutes = seconds / 60;
        seconds %= 60;

        std::stringstream timeStream;
        timeStream << "Time: " << std::setfill('0') << std::setw(2) << minutes
                   << ":" << std::setfill('0') << std::setw(2) << seconds;
        timeText->text = timeStream.str();
    }

    auto scoreTextComp = _componentManager->getComponentByType(
        _scoreTextEntity, ComponentType::TEXT);
    auto scoreText = std::dynamic_pointer_cast<TextComponent>(scoreTextComp);
    if (scoreText)
        scoreText->text = "Score: " + std::to_string(gameStats->getScore());

    auto gameOverTextComp = _componentManager->getComponentByType(
        _gameOverTextEntity, ComponentType::TEXT);
    auto gameOverText = std::dynamic_pointer_cast<TextComponent>(
        gameOverTextComp);

    auto resultTextComp = _componentManager->getComponentByType(
        _gameResultTextEntity, ComponentType::TEXT);
    auto resultText = std::dynamic_pointer_cast<TextComponent>(resultTextComp);

    auto restartTextComp = _componentManager->getComponentByType(
        _restartTextEntity, ComponentType::TEXT);
    auto restartText = std::dynamic_pointer_cast<TextComponent>(
        restartTextComp);

    if (board->isGameOver()) {
        if (gameOverText) {
            if (board->isGameWon()) {
                gameOverText->text = "YOU WIN!!!";
                gameOverText->color = Color::GREEN;
            } else {
                gameOverText->text = "GAME OVER";
                gameOverText->color = Color::RED;
            }
            gameOverText->visible = true;
        }

        if (resultText) {
            if (board->isGameWon()) {
                resultText->text = "Final Score: " +
                    std::to_string(gameStats->getScore());
                resultText->visible = true;
            } else {
                resultText->visible = false;
            }
        }

        if (restartText) {
            restartText->visible = true;
        }
    } else {
        if (gameOverText) gameOverText->visible = false;
        if (resultText) resultText->visible = false;
        if (restartText) restartText->visible = false;
    }
}

}  // namespace Arcade
