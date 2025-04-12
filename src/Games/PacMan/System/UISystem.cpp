// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** UI System for Pacman implementation
*/

#include <string>
#include <memory>
#include <map>
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
std::shared_ptr<Arcade::IEntityManager> entityManager,
const std::map<std::string, DrawableComponent>& assets)
: _componentManager(componentManager),
_entityManager(entityManager),
_assets(assets) {
    createUIEntities();
}

void UISystem::createUIEntities() {
    std::string fontPath = "./assets/fonts/Arial.ttf";
    auto defaultFontIt = _assets.find("fonts.default");
    if (defaultFontIt != _assets.end()) {
        fontPath = defaultFontIt->second.getFont();
    }
    _scoreTextEntity = _entityManager->createEntity("UI_ScoreText");
    auto scoreText = std::make_shared<DrawableComponent>();
    auto scoreAsset = _assets.find("ui.score");
    if (scoreAsset != _assets.end()) {
        // Copy properties from the asset
        scoreText->setPath(scoreAsset->second.getPath());
        scoreText->setFont(scoreAsset->second.getFont());
        scoreText->setColor(scoreAsset->second.getColor());
        scoreText->setScale(scoreAsset->second.getScale());
        scoreText->setPosition(scoreAsset->second.getPositionX(),
            scoreAsset->second.getPositionY());
        scoreText->setRotation(scoreAsset->second.getRotation());
        scoreText->setVisibility(scoreAsset->second.isRenderable());
        scoreText->setDimensions(scoreAsset->second.getWidth(),
            scoreAsset->second.getHeight());
        scoreText->setText("SCORE: 0");
    } else {
        scoreText->setAsText("SCORE: 0", fontPath, 24);
    }
    scoreText->setPosition(20, 20);
    scoreText->setColor(Color::YELLOW);
    scoreText->setVisibility(true);
    _componentManager->registerComponent(_scoreTextEntity, scoreText);

    _levelTextEntity = _entityManager->createEntity("UI_LevelText");
    auto levelText = std::make_shared<DrawableComponent>();
    levelText->setAsText("LEVEL: 1", fontPath, 24);
    levelText->setPosition(20, 50);
    levelText->setColor(Color::YELLOW);
    levelText->setVisibility(true);
    _componentManager->registerComponent(_levelTextEntity, levelText);

    _livesTextEntity = _entityManager->createEntity("UI_LivesText");
    auto livesText = std::make_shared<DrawableComponent>();
    auto livesAsset = _assets.find("ui.lives");
    if (livesAsset != _assets.end()) {
        // Copy properties from the asset
        livesText->setPath(livesAsset->second.getPath());
        livesText->setFont(livesAsset->second.getFont());
        livesText->setColor(livesAsset->second.getColor());
        livesText->setScale(livesAsset->second.getScale());
        livesText->setPosition(livesAsset->second.getPositionX(),
            livesAsset->second.getPositionY());
        livesText->setRotation(livesAsset->second.getRotation());
        livesText->setVisibility(livesAsset->second.isRenderable());
        livesText->setDimensions(livesAsset->second.getWidth(),
            livesAsset->second.getHeight());
        livesText->setText("LIVES:");
    } else {
        livesText->setAsText("LIVES:", fontPath, 24);
    }
    livesText->setPosition(20, 80);
    livesText->setColor(Color::YELLOW);
    livesText->setVisibility(true);
    _componentManager->registerComponent(_livesTextEntity, livesText);

    float iconSpacing = 32.0f;
    float startX = 20.0f;
    float y = 110.0f;

    for (int i = 0; i < 3; i++) {
        std::shared_ptr<Arcade::IEntity> lifeIcon
            = _entityManager->createEntity("UI_LifeIcon_"
            + std::to_string(i));

        auto posComp = std::make_shared<PositionComponent>(startX +
            (i * iconSpacing), y);
        _componentManager->registerComponent(lifeIcon, posComp);

        auto spriteComp = std::make_shared<DrawableComponent>();
        auto pacmanAsset = _assets.find("pacman.default");
        if (pacmanAsset != _assets.end()) {
            // Copy properties from the asset
            spriteComp->setPath(pacmanAsset->second.getPath());
            spriteComp->setFont(pacmanAsset->second.getFont());
            spriteComp->setColor(pacmanAsset->second.getColor());
            spriteComp->setScale(pacmanAsset->second.getScale());
            spriteComp->setRotation(pacmanAsset->second.getRotation());
            spriteComp->setDimensions(pacmanAsset->second.getWidth(),
                pacmanAsset->second.getHeight());
        } else {
            spriteComp->setAsTexture("assets/pacman/pacman.png", 32, 32);
        }
        spriteComp->setPosition(startX + (i * iconSpacing), y);
        spriteComp->setVisibility(true);
        _componentManager->registerComponent(lifeIcon, spriteComp);

        _livesIconEntities.push_back(lifeIcon);
    }

    _gameOverTextEntity = _entityManager->createEntity("UI_GameOverText");
    auto gameOverText = std::make_shared<DrawableComponent>();
    auto gameOverAsset = _assets.find("ui.game_over");
    if (gameOverAsset != _assets.end()) {
        // Copy properties from the asset
        gameOverText->setPath(gameOverAsset->second.getPath());
        gameOverText->setFont(gameOverAsset->second.getFont());
        gameOverText->setText(gameOverAsset->second.getText());
        gameOverText->setColor(gameOverAsset->second.getColor());
        gameOverText->setScale(gameOverAsset->second.getScale());
        gameOverText->setRotation(gameOverAsset->second.getRotation());
        gameOverText->setDimensions(gameOverAsset->second.getWidth(),
            gameOverAsset->second.getHeight());
    } else {
        gameOverText->setAsText("GAME OVER", fontPath, 32);
    }
    gameOverText->setPosition(350, 300);
    gameOverText->setColor(Color::RED);
    gameOverText->setVisibility(false);
    _componentManager->registerComponent(_gameOverTextEntity, gameOverText);

    _gameWinTextEntity = _entityManager->createEntity("UI_GameWinText");
    auto gameWinText = std::make_shared<DrawableComponent>();
    auto winAsset = _assets.find("ui.win");
    if (winAsset != _assets.end()) {
        // Copy properties from the asset
        gameWinText->setPath(winAsset->second.getPath());
        gameWinText->setFont(winAsset->second.getFont());
        gameWinText->setText(winAsset->second.getText());
        gameWinText->setColor(winAsset->second.getColor());
        gameWinText->setScale(winAsset->second.getScale());
        gameWinText->setRotation(winAsset->second.getRotation());
        gameWinText->setDimensions(winAsset->second.getWidth(),
            winAsset->second.getHeight());
    } else {
        gameWinText->setAsText("YOU WIN!", fontPath, 32);
    }
    gameWinText->setPosition(350, 300);
    gameWinText->setColor(Color::GREEN);
    gameWinText->setVisibility(false);
    _componentManager->registerComponent(_gameWinTextEntity, gameWinText);

    _restartTextEntity = _entityManager->createEntity("UI_RestartText");
    auto restartText = std::make_shared<DrawableComponent>();
    restartText->setAsText("Press 'R' to restart or ESC for menu",
        fontPath, 24);
    restartText->setPosition(250, 350);
    restartText->setColor(Color::WHITE);
    restartText->setVisibility(false);
    _componentManager->registerComponent(_restartTextEntity, restartText);
}

void UISystem::updateUITexts() {
    std::shared_ptr<Arcade::IEntity> pacmanEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
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

    auto scoreTextComp = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(_scoreTextEntity,
            ComponentType::DRAWABLE));
    if (scoreTextComp)
        scoreTextComp->setText("SCORE: " + std::to_string(
            pacmanComp->getScore()));

    auto levelTextComp = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(_levelTextEntity,
            ComponentType::DRAWABLE));
    if (levelTextComp) {
        std::shared_ptr<Arcade::IEntity> gridEntity = 0;
        for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
            if (name == "Grid") {
                gridEntity = entity;
                break;
            }
        }
        auto gridComp = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(gridEntity,
                static_cast<ComponentType>(1000)));
        int level = (gridComp) ? gridComp->getLevel() : 1;
        levelTextComp->setText("LEVEL: " + std::to_string(level));
    }
}

void UISystem::updateLivesIcons() {
    std::shared_ptr<Arcade::IEntity> pacmanEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
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
        auto spriteComp = std::dynamic_pointer_cast<IDrawableComponent>(
            _componentManager->getComponentByType(_livesIconEntities[i],
                ComponentType::DRAWABLE));

        if (i < static_cast<size_t>(lives)) {
            if (!spriteComp) {
                auto newDrawableComp = std::make_shared<DrawableComponent>();
                auto pacmanAsset = _assets.find("pacman.default");
                if (pacmanAsset != _assets.end()) {
                    // Copy properties from the asset
                    newDrawableComp->setPath(pacmanAsset->second.getPath());
                    newDrawableComp->setFont(pacmanAsset->second.getFont());
                    newDrawableComp->setColor(pacmanAsset->second.getColor());
                    newDrawableComp->setScale(pacmanAsset->second.getScale());
                    newDrawableComp->setRotation(pacmanAsset->
                        second.getRotation());
                    newDrawableComp->setDimensions(pacmanAsset->
                        second.getWidth(),
                        pacmanAsset->second.getHeight());
                } else {
                    newDrawableComp->setAsTexture("assets/pacman/pacman.png",
                        32, 32);
                }
                _componentManager->registerComponent(_livesIconEntities[i],
                    newDrawableComp);
            } else {
                spriteComp->setVisibility(true);
            }
        } else if (spriteComp) {
            spriteComp->setVisibility(false);
        }
    }
}

void UISystem::update() {
    std::shared_ptr<Arcade::IEntity> pacmanEntity = nullptr;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
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

void UISystem::updateGameOverState() {
    std::shared_ptr<Arcade::IEntity> gridEntity = 0;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
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

    auto gameOverTextComp = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(_gameOverTextEntity,
            ComponentType::DRAWABLE));

    auto gameWinTextComp = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(_gameWinTextEntity,
            ComponentType::DRAWABLE));

    auto restartTextComp = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(_restartTextEntity,
            ComponentType::DRAWABLE));

    if (gridComp->isGameOver()) {
        if (gameOverTextComp)
            gameOverTextComp->setVisibility(!gridComp->isGameWon());

        if (gameWinTextComp)
            gameWinTextComp->setVisibility(gridComp->isGameWon());

        if (restartTextComp)
            restartTextComp->setVisibility(true);
    } else {
        if (gameOverTextComp)
            gameOverTextComp->setVisibility(false);

        if (gameWinTextComp)
            gameWinTextComp->setVisibility(false);

        if (restartTextComp)
            restartTextComp->setVisibility(false);
    }
}

// Implementation of update() and other methods remain the same

}  // namespace PacMan
}  // namespace Arcade
