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
#include "ECS/Components/Text/TextComponent.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"

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
    auto scoreText = std::make_shared<TextComponent>("SCORE: 0", 20, 20,
        Color::YELLOW);
    scoreText->visible = true;
    _componentManager->registerComponent(_scoreTextEntity, scoreText);

    _levelTextEntity = _entityManager->createEntity("UI_LevelText");
    auto levelText = std::make_shared<TextComponent>("LEVEL: 1", 20, 50,
        Color::YELLOW);
    levelText->visible = true;
    _componentManager->registerComponent(_levelTextEntity, levelText);

    _livesTextEntity = _entityManager->createEntity("UI_LivesText");
    auto livesText = std::make_shared<TextComponent>("LIVES:", 20, 80,
        Color::YELLOW);
    livesText->visible = true;
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

        auto spriteComp = std::make_shared<SpriteComponent>
            ("assets/pacman/pacman.png");
        _componentManager->registerComponent(lifeIcon, spriteComp);

        _livesIconEntities.push_back(lifeIcon);
    }

    _gameOverTextEntity = _entityManager->createEntity("UI_GameOverText");
    auto gameOverText = std::make_shared<TextComponent>("GAME OVER", 350, 300,
        Color::RED);
    gameOverText->visible = false;
    _componentManager->registerComponent(_gameOverTextEntity, gameOverText);

    _gameWinTextEntity = _entityManager->createEntity("UI_GameWinText");
    auto gameWinText = std::make_shared<TextComponent>("YOU WIN!", 350, 300,
        Color::GREEN);
    gameWinText->visible = false;
    _componentManager->registerComponent(_gameWinTextEntity, gameWinText);

    _restartTextEntity = _entityManager->createEntity("UI_RestartText");
    auto restartText = std::make_shared<TextComponent>(
        "Press 'R' to restart or ESC for menu", 250, 350, Color::WHITE);
    restartText->visible = false;
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

    auto scoreTextComp = std::dynamic_pointer_cast<TextComponent>(
        _componentManager->getComponentByType(_scoreTextEntity,
            ComponentType::TEXT));
    if (scoreTextComp)
        scoreTextComp->text = "SCORE: " + std::to_string(
            pacmanComp->getScore());

    auto levelTextComp = std::dynamic_pointer_cast<TextComponent>(
        _componentManager->getComponentByType(_levelTextEntity,
            ComponentType::TEXT));
    if (levelTextComp)
        levelTextComp->text = "LEVEL: " + std::to_string(1);
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
        auto posComp = std::dynamic_pointer_cast<PositionComponent>(
            _componentManager->getComponentByType(_livesIconEntities[i],
                ComponentType::POSITION));

        if (posComp) {
            auto spriteComp = std::dynamic_pointer_cast<SpriteComponent>(
                _componentManager->getComponentByType(_livesIconEntities[i],
                    ComponentType::SPRITE));

            if (i < static_cast<size_t>(lives)) {
                if (!spriteComp) {
                    auto newSpriteComp = std::make_shared<SpriteComponent>
                        ("assets/pacman/pacman.png");
                    _componentManager->registerComponent(_livesIconEntities[i],
                        newSpriteComp);
                }
            } else {
                if (spriteComp) {
                    _componentManager->unregisterComponent(
                        _livesIconEntities[i], typeid(*spriteComp).name());
                }
            }
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

    if (gridEntity != 0)
        return;

    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));

    if (!gridComp)
        return;

    auto gameOverTextComp = std::dynamic_pointer_cast<TextComponent>(
        _componentManager->getComponentByType(_gameOverTextEntity,
            ComponentType::TEXT));

    auto gameWinTextComp = std::dynamic_pointer_cast<TextComponent>(
        _componentManager->getComponentByType(_gameWinTextEntity,
            ComponentType::TEXT));

    auto restartTextComp = std::dynamic_pointer_cast<TextComponent>(
        _componentManager->getComponentByType(_restartTextEntity,
            ComponentType::TEXT));

    if (gridComp->isGameOver()) {
        if (gameOverTextComp)
            gameOverTextComp->visible = !gridComp->isGameWon();

        if (gameWinTextComp)
            gameWinTextComp->visible = gridComp->isGameWon();

        if (restartTextComp)
            restartTextComp->visible = true;
    } else {
        if (gameOverTextComp)
            gameOverTextComp->visible = false;

        if (gameWinTextComp)
            gameWinTextComp->visible = false;

        if (restartTextComp)
            restartTextComp->visible = false;
    }
}

}  // namespace PacMan
}  // namespace Arcade
