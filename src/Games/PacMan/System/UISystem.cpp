// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** UI System for Pacman implementation
*/

#include <string>
#include <memory>
#include <iostream>
#include "Games/PacMan/System/UISystem.hpp"
#include "Games/PacMan/Components/PacmanStats.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"

namespace Arcade {
namespace PacMan {

UISystem::UISystem(
std::shared_ptr<Arcade::IComponentManager> componentManager,
std::shared_ptr<Arcade::IEntityManager> entityManager)
: _componentManager(componentManager),
_entityManager(entityManager) {
    createUIEntities();
}

void UISystem::createUIEntities() {
    _scoreTextEntity = _entityManager->createEntity("UI_ScoreText");
    auto scoreText = std::make_shared<DrawableComponent>();
    scoreText->setAsText("SCORE: 0", "./assets/fonts/Arial.ttf", 24);
    scoreText->posX = 20;
    scoreText->posY = 20;
    scoreText->color = Color::YELLOW;
    scoreText->isVisible = true;
    _componentManager->registerComponent(_scoreTextEntity, scoreText);

    _levelTextEntity = _entityManager->createEntity("UI_LevelText");
    auto levelText = std::make_shared<DrawableComponent>();
    levelText->setAsText("LEVEL: 1", "./assets/fonts/Arial.ttf", 24);
    levelText->posX = 20;
    levelText->posY = 50;
    levelText->color = Color::YELLOW;
    levelText->isVisible = true;
    _componentManager->registerComponent(_levelTextEntity, levelText);

    _livesTextEntity = _entityManager->createEntity("UI_LivesText");
    auto livesText = std::make_shared<DrawableComponent>();
    livesText->setAsText("LIVES:", "./assets/fonts/Arial.ttf", 24);
    livesText->posX = 20;
    livesText->posY = 80;
    livesText->color = Color::YELLOW;
    livesText->isVisible = true;
    _componentManager->registerComponent(_livesTextEntity, livesText);

    float iconSpacing = 32.0f;
    float startX = 20.0f;
    float y = 110.0f;

    for (int i = 0; i < 3; i++) {
        Arcade::Entity lifeIcon = _entityManager->createEntity("UI_LifeIcon_"
            + std::to_string(i));

        auto posComp = std::make_shared<PositionComponent>(startX +
            (i * iconSpacing), y);
        _componentManager->registerComponent(lifeIcon, posComp);

        auto spriteComp = std::make_shared<DrawableComponent>();
        spriteComp->setAsTexture("assets/pacman/pacman.png", 32, 32);
        spriteComp->posX = startX + (i * iconSpacing);
        spriteComp->posY = y;
        spriteComp->isVisible = true;
        _componentManager->registerComponent(lifeIcon, spriteComp);

        _livesIconEntities.push_back(lifeIcon);
    }

    _gameOverTextEntity = _entityManager->createEntity("UI_GameOverText");
    auto gameOverText = std::make_shared<DrawableComponent>();
    gameOverText->setAsText("GAME OVER", "./assets/fonts/Arial.ttf", 32);
    gameOverText->posX = 350;
    gameOverText->posY = 300;
    gameOverText->color = Color::RED;
    gameOverText->isVisible = false;
    _componentManager->registerComponent(_gameOverTextEntity, gameOverText);

    _gameWinTextEntity = _entityManager->createEntity("UI_GameWinText");
    auto gameWinText = std::make_shared<DrawableComponent>();
    gameWinText->setAsText("YOU WIN!", "./assets/fonts/Arial.ttf", 32);
    gameWinText->posX = 350;
    gameWinText->posY = 300;
    gameWinText->color = Color::GREEN;
    gameWinText->isVisible = false;
    _componentManager->registerComponent(_gameWinTextEntity, gameWinText);

    _restartTextEntity = _entityManager->createEntity("UI_RestartText");
    auto restartText = std::make_shared<DrawableComponent>();
    restartText->setAsText("Press 'R' to restart or ESC for menu",
                          "./assets/fonts/Arial.ttf", 24);
    restartText->posX = 250;
    restartText->posY = 350;
    restartText->color = Color::WHITE;
    restartText->isVisible = false;
    _componentManager->registerComponent(_restartTextEntity, restartText);
}

void UISystem::update() {
    Arcade::Entity pacmanEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "Pacman") {
            pacmanEntity = entity;
            break;
        }
    }

    if (!pacmanEntity) return;

    auto pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
        _componentManager->getComponentByType(pacmanEntity,
            static_cast<ComponentType>(1001)));

    if (!pacmanComp) return;

    updateUITexts();
    updateLivesIcons();
    updateGameOverState();
}

void UISystem::updateUITexts() {
    Arcade::Entity pacmanEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "Pacman") {
            pacmanEntity = entity;
            break;
        }
    }

    if (!pacmanEntity) return;

    auto pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
        _componentManager->getComponentByType(pacmanEntity,
            static_cast<ComponentType>(1001)));

    if (!pacmanComp) return;

    auto scoreTextComp = std::dynamic_pointer_cast<DrawableComponent>(
        _componentManager->getComponentByType(_scoreTextEntity,
            ComponentType::DRAWABLE));
    if (scoreTextComp)
        scoreTextComp->text = "SCORE: " + std::to_string(
            pacmanComp->getScore());

    auto levelTextComp = std::dynamic_pointer_cast<DrawableComponent>(
        _componentManager->getComponentByType(_levelTextEntity,
            ComponentType::DRAWABLE));
    if (levelTextComp) {
        Arcade::Entity gridEntity = 0;
        for (const auto& [entity, name] : _entityManager->getEntities()) {
            if (name == "Grid") {
                gridEntity = entity;
                break;
            }
        }
        auto gridComp = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(gridEntity,
                static_cast<ComponentType>(1000)));
        int level = (gridComp) ? gridComp->getLevel() : 1;
        levelTextComp->text = "LEVEL: " + std::to_string(level);
    }
}

void UISystem::updateLivesIcons() {
    Arcade::Entity pacmanEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "Pacman") {
            pacmanEntity = entity;
            break;
        }
    }

    if (!pacmanEntity) return;

    auto pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
        _componentManager->getComponentByType(pacmanEntity,
            static_cast<ComponentType>(1001)));

    if (!pacmanComp) return;

    int lives = pacmanComp->getLives();

    for (size_t i = 0; i < _livesIconEntities.size(); i++) {
        auto spriteComp = std::dynamic_pointer_cast<DrawableComponent>(
            _componentManager->getComponentByType(_livesIconEntities[i],
                ComponentType::DRAWABLE));

        if (i < static_cast<size_t>(lives)) {
            if (!spriteComp) {
                auto newDrawableComp = std::make_shared<DrawableComponent>();
                newDrawableComp->setAsTexture
                    ("assets/pacman/pacman.png", 32, 32);
                _componentManager->registerComponent(_livesIconEntities[i],
                    newDrawableComp);
            } else {
                spriteComp->isVisible = true;
            }
        } else if (spriteComp) {
            spriteComp->isVisible = false;
        }
    }
}

void UISystem::updateGameOverState() {
    Arcade::Entity gridEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name == "Grid") {
            gridEntity = entity;
            break;
        }
    }

    if (gridEntity == 0)
        return;

    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));

    if (!gridComp)
        return;

    auto gameOverTextComp = std::dynamic_pointer_cast<DrawableComponent>(
        _componentManager->getComponentByType(_gameOverTextEntity,
            ComponentType::DRAWABLE));

    auto gameWinTextComp = std::dynamic_pointer_cast<DrawableComponent>(
        _componentManager->getComponentByType(_gameWinTextEntity,
            ComponentType::DRAWABLE));

    auto restartTextComp = std::dynamic_pointer_cast<DrawableComponent>(
        _componentManager->getComponentByType(_restartTextEntity,
            ComponentType::DRAWABLE));

    if (gridComp->isGameOver()) {
        if (gameOverTextComp)
            gameOverTextComp->isVisible = !gridComp->isGameWon();

        if (gameWinTextComp)
            gameWinTextComp->isVisible = gridComp->isGameWon();

        if (restartTextComp)
            restartTextComp->isVisible = true;
    } else {
        if (gameOverTextComp)
            gameOverTextComp->isVisible = false;

        if (gameWinTextComp)
            gameWinTextComp->isVisible = false;

        if (restartTextComp)
            restartTextComp->isVisible = false;
    }
}

}  // namespace PacMan
}  // namespace Arcade
