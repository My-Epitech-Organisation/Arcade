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
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {

void UISystem::createUIEntities() {
    _minesTextEntity = _entityManager->createEntity("UI_MinesText");
    auto minesText = std::make_shared<DrawableComponent>();
    minesText->setAsText("Mines: 0", "./assets/fonts/Arial.ttf", 16.0f);
    minesText->posX = 10;
    minesText->posY = 10;
    minesText->color = Color::BLACK;
    _componentManager->registerComponent(_minesTextEntity, minesText);

    _timeTextEntity = _entityManager->createEntity("UI_TimeText");
    auto timeText = std::make_shared<DrawableComponent>();
    timeText->setAsText("Time: 00:00", "./assets/fonts/Arial.ttf", 16.0f);
    timeText->posX = 10;
    timeText->posY = 40;
    timeText->color = Color::BLACK;
    _componentManager->registerComponent(_timeTextEntity, timeText);

    _scoreTextEntity = _entityManager->createEntity("UI_ScoreText");
    auto scoreText = std::make_shared<DrawableComponent>();
    scoreText->setAsText("Score: 0", "./assets/fonts/Arial.ttf", 16.0f);
    scoreText->posX = 10;
    scoreText->posY = 70;
    scoreText->color = Color::BLACK;
    _componentManager->registerComponent(_scoreTextEntity, scoreText);

    _gameOverTextEntity = _entityManager->createEntity("UI_GameOverText");
    auto gameOverText = std::make_shared<DrawableComponent>();
    gameOverText->setAsText("GAME OVER", "./assets/fonts/Arial.ttf", 24.0f);
    gameOverText->posX = 350;
    gameOverText->posY = 300;
    gameOverText->color = Color::RED;
    gameOverText->isVisible = false;
    _componentManager->registerComponent(_gameOverTextEntity, gameOverText);

    _gameResultTextEntity = _entityManager->createEntity("UI_GameResultText");
    auto resultText = std::make_shared<DrawableComponent>();
    resultText->setAsText("", "./assets/fonts/Arial.ttf", 18.0f);
    resultText->posX = 330;
    resultText->posY = 330;
    resultText->color = Color::GREEN;
    resultText->isVisible = false;
    _componentManager->registerComponent(_gameResultTextEntity, resultText);

    _restartTextEntity = _entityManager->createEntity("UI_RestartText");
    auto restartText = std::make_shared<DrawableComponent>();
    restartText->setAsText("Press 'R' to restart or ESC for menu",
        "./assets/fonts/Arial.ttf", 16.0f);
    restartText->posX = 250;
    restartText->posY = 360;
    restartText->color = Color::WHITE;
    restartText->isVisible = false;
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
        _minesTextEntity, ComponentType::DRAWABLE);
    auto minesText
        = std::dynamic_pointer_cast<DrawableComponent>(minesTextComp);
    if (minesText)
        minesText->setAsText("Mines: "
            + std::to_string(gameStats->getRemainingMines()),
            minesText->font, minesText->scale);

    auto timeTextComp = _componentManager->getComponentByType(
        _timeTextEntity, ComponentType::DRAWABLE);
    auto timeText = std::dynamic_pointer_cast<DrawableComponent>(timeTextComp);
    if (timeText) {
        int seconds = gameStats->getTimeRemaining();
        int minutes = seconds / 60;
        seconds %= 60;

        std::stringstream timeStream;
        timeStream << "Time: " << std::setfill('0') << std::setw(2) << minutes
                   << ":" << std::setfill('0') << std::setw(2) << seconds;
        timeText->setAsText(timeStream.str(), timeText->font, timeText->scale);
    }

    auto scoreTextComp = _componentManager->getComponentByType(
        _scoreTextEntity, ComponentType::DRAWABLE);
    auto scoreText
        = std::dynamic_pointer_cast<DrawableComponent>(scoreTextComp);
    if (scoreText)
        scoreText->setAsText("Score: " + std::to_string(gameStats->getScore()),
            scoreText->font, scoreText->scale);

    auto gameOverTextComp = _componentManager->getComponentByType(
        _gameOverTextEntity, ComponentType::DRAWABLE);
    auto gameOverText = std::dynamic_pointer_cast<DrawableComponent>(
        gameOverTextComp);

    auto resultTextComp = _componentManager->getComponentByType(
        _gameResultTextEntity, ComponentType::DRAWABLE);
    auto resultText
        = std::dynamic_pointer_cast<DrawableComponent>(resultTextComp);

    auto restartTextComp = _componentManager->getComponentByType(
        _restartTextEntity, ComponentType::DRAWABLE);
    auto restartText = std::dynamic_pointer_cast<DrawableComponent>(
        restartTextComp);

    if (board->isGameOver()) {
        if (gameOverText) {
            if (board->isGameWon()) {
                gameOverText->setAsText("YOU WIN!!!",
                    gameOverText->font, gameOverText->scale);
                gameOverText->color = Color::GREEN;
            } else {
                gameOverText->setAsText("GAME OVER",
                    gameOverText->font, gameOverText->scale);
                gameOverText->color = Color::RED;
            }
            gameOverText->isVisible = true;
        }

        if (resultText) {
            if (board->isGameWon()) {
                resultText->setAsText("Final Score: "
                    + std::to_string(gameStats->getScore()),
                    resultText->font, resultText->scale);
                resultText->isVisible = true;
            } else {
                resultText->isVisible = false;
            }
        }

        if (restartText) {
            restartText->isVisible = true;
        }
    } else {
        if (gameOverText) gameOverText->isVisible = false;
        if (resultText) resultText->isVisible = false;
        if (restartText) restartText->isVisible = false;
    }
}

}  // namespace Arcade
