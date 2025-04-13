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
    if (!_cachedPacmanComponent) return;

    auto scoreTextComp = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(_scoreTextEntity,
            ComponentType::DRAWABLE));
    if (scoreTextComp) {
        // Only update if score has changed
        static int lastScore = -1;
        int currentScore = _cachedPacmanComponent->getScore();
        if (lastScore != currentScore) {
            scoreTextComp->setText("SCORE: " + std::to_string(currentScore));
            lastScore = currentScore;
        }
    }

    auto levelTextComp = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(_levelTextEntity,
            ComponentType::DRAWABLE));
    if (levelTextComp && _cachedGridComponent) {
        // Only update if level has changed
        static int lastLevel = -1;
        int currentLevel = _cachedGridComponent->getLevel();
        if (lastLevel != currentLevel) {
            levelTextComp->setText("LEVEL: " + std::to_string(currentLevel));
            lastLevel = currentLevel;
        }
    }
}

void UISystem::updateLivesIcons() {
    if (!_cachedPacmanComponent) return;

    // Only update if lives have changed
    static int lastLives = -1;
    int lives = _cachedPacmanComponent->getLives();
    
    if (lastLives == lives) {
        return;
    }
    
    lastLives = lives;

    for (size_t i = 0; i < _livesIconEntities.size(); i++) {
        auto spriteComp = std::dynamic_pointer_cast<IDrawableComponent>(
            _componentManager->getComponentByType(_livesIconEntities[i],
                ComponentType::DRAWABLE));

        if (spriteComp) {
            spriteComp->setVisibility(i < static_cast<size_t>(lives));
        }
    }
}

void UISystem::update() {
    // Only update UI every few frames to reduce overhead
    static int frameCounter = 0;
    frameCounter = (frameCounter + 1) % 5; // Update UI only every 5 frames
    
    if (frameCounter != 0) {
        return;
    }
    
    // Cache Pacman entity lookup
    if (!_cachedPacmanEntity) {
        for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
            if (name == "Pacman") {
                _cachedPacmanEntity = entity;
                break;
            }
        }
    }
    
    // Cache Grid entity lookup
    if (!_cachedGridEntity) {
        for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
            if (name == "Grid") {
                _cachedGridEntity = entity;
                break;
            }
        }
    }
    
    // Check if entities are still valid
    if (!_cachedPacmanEntity || !_cachedGridEntity) {
        return;
    }

    // Cache components
    if (!_cachedPacmanComponent) {
        _cachedPacmanComponent = std::dynamic_pointer_cast<PacmanComponent>(
            _componentManager->getComponentByType(_cachedPacmanEntity,
                static_cast<ComponentType>(1001)));
    }
    
    if (!_cachedGridComponent) {
        _cachedGridComponent = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(_cachedGridEntity,
                static_cast<ComponentType>(1000)));
    }
    
    // Early exit if components aren't available
    if (!_cachedPacmanComponent || !_cachedGridComponent) {
        return;
    }

    // Update the UI elements
    updateUITexts();
    updateLivesIcons();
    updateGameOverState();
}

void UISystem::updateGameOverState() {
    if (!_cachedGridComponent) return;

    // Only update if game state has changed
    static bool lastGameOver = false;
    static bool lastGameWon = false;
    
    bool gameOver = _cachedGridComponent->isGameOver();
    bool gameWon = _cachedGridComponent->isGameWon();
    
    if (lastGameOver == gameOver && lastGameWon == gameWon) {
        return;
    }
    
    lastGameOver = gameOver;
    lastGameWon = gameWon;

    auto gameOverTextComp = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(_gameOverTextEntity,
            ComponentType::DRAWABLE));

    auto gameWinTextComp = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(_gameWinTextEntity,
            ComponentType::DRAWABLE));

    auto restartTextComp = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(_restartTextEntity,
            ComponentType::DRAWABLE));

    if (gameOver) {
        if (gameOverTextComp)
            gameOverTextComp->setVisibility(!gameWon);

        if (gameWinTextComp)
            gameWinTextComp->setVisibility(gameWon);

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

}  // namespace PacMan
}  // namespace Arcade
