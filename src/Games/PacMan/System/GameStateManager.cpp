// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game State Manager for Pacman implementation
*/

#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <map>
#include <vector>
#include "Games/PacMan/System/GameStateManager.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "Games/PacMan/Components/FoodComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "Games/PacMan/Components/GhostComponent.hpp"


namespace Arcade {
namespace PacMan {

GameStateManager::GameStateManager(
std::shared_ptr<Arcade::IComponentManager> componentManager,
std::shared_ptr<Arcade::IEntityManager> entityManager,
const std::map<std::string, DrawableComponent>& assets)
: _componentManager(componentManager),
_entityManager(entityManager), _assets(assets) {
}

std::shared_ptr<IDrawableComponent>
GameStateManager::getDrawableAsset(const std::string& key) const {
    auto it = _assets.find(key);
    if (it != _assets.end()) {
        auto component = std::make_shared<DrawableComponent>(it->second);
        return component;
    }
    return nullptr;
}

void GameStateManager::checkWinCondition(std::shared_ptr<GridComponent> grid) {
    if (!grid)
        return;
    static int lastFoodCount = -1;
    if (lastFoodCount != grid->getFoodCount()) {
        lastFoodCount = grid->getFoodCount();
    }
    if (grid->getFoodCount() <= 0) {
        grid->setGameOver(false);
        grid->setGameWon(true);
        grid->incrementLevel();
        reloadCurrentMap();
        increaseGameSpeed();
    }
}

void GameStateManager::resetEntireGame() {
    resetPositions();
    bool scoreProperlySaved = false;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Pacman") {
            auto pacman = std::dynamic_pointer_cast<PacmanComponent>(
                _componentManager->getComponentByType(entity,
                    static_cast<ComponentType>(1001)));
            if (pacman) {
                int oldScore = pacman->getScore();
                pacman->setScore(0);
                pacman->setLives(3);
                if (pacman->getScore() != 0) {
                } else {
                    scoreProperlySaved = true;
                }
            }
        }
    }
    std::shared_ptr<IEntity> gridEntity = nullptr;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Grid") {
            gridEntity = entity;
            break;
        }
    }
    if (gridEntity) {
        auto grid = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(gridEntity,
                static_cast<ComponentType>(1000)));
        if (grid) {
            grid->setFoodCount(grid->getTotalFoodCount());
            grid->setGameOver(false);
            grid->setGameWon(false);
        }
    }
    int totalFoodReset = 0;
    int visibilityFixed = 0;
    int textureReset = 0;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1003)));
        if (foodComp) {
            bool wasEaten = foodComp->isEaten();
            foodComp->setEaten(false);
            totalFoodReset++;
            auto spriteComp
            = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));
            if (spriteComp) {
                if (!spriteComp->isRenderable()) {
                    spriteComp->setVisibility(true);
                    visibilityFixed++;
                }
                float gridStartX = 0.0f;
                float gridStartY = 0.0f;
                float cellSize = 32.0f;
                for (const auto& [gEntity, gName]
                    : _entityManager->getEntitiesMap()) {
                    if (gName == "Grid") {
                        auto gridComp
                            = std::dynamic_pointer_cast<GridComponent>(
                            _componentManager->getComponentByType(gEntity,
                                static_cast<ComponentType>(1000)));
                        if (gridComp) {
                            cellSize = gridComp->getCellSize();
                        }
                        auto gridPos
                            = std::dynamic_pointer_cast<PositionComponent>(
                            _componentManager->getComponentByType(gEntity,
                                ComponentType::POSITION));
                        if (gridPos) {
                            gridStartX = gridPos->x;
                            gridStartY = gridPos->y;
                        }
                        break;
                    }
                }
                float dotX = gridStartX + (foodComp->getGridX() * cellSize);
                float dotY = gridStartY + (foodComp->getGridY() * cellSize);
                std::shared_ptr<DrawableComponent> freshSprite
                    = std::make_shared<DrawableComponent>();

                if (foodComp->getFoodType() == FoodType::NORMAL_DOT) {
                    auto foodAsset = getDrawableAsset("map.food");
                    if (foodAsset) {
                        auto castedFoodAsset
                            = std::dynamic_pointer_cast
                                <DrawableComponent>(foodAsset);
                        if (castedFoodAsset) {
                            *freshSprite = *castedFoodAsset;
                        } else {
                            freshSprite->setAsTexture
                                ("assets/pacman/dot.png", 32, 32);
                            freshSprite->setAsCharacter('.');
                        }
                    } else {
                        freshSprite->setAsTexture
                            ("assets/pacman/dot.png", 32, 32);
                        freshSprite->setAsCharacter('.');
                    }
                } else {
                    auto powerAsset = getDrawableAsset("map.power_pellet");
                    if (powerAsset) {
                        auto castedPowerAsset
                            = std::dynamic_pointer_cast
                            <DrawableComponent>(powerAsset);
                        if (castedPowerAsset) {
                            *freshSprite = *castedPowerAsset;
                        } else {
                            // Fallback if cast fails
                            freshSprite->setAsTexture
                                ("assets/pacman/power_pellet.png", 32, 32);
                            freshSprite->setAsCharacter('U');
                        }
                    } else {
                        freshSprite->setAsTexture
                            ("assets/pacman/power_pellet.png", 32, 32);
                        freshSprite->setAsCharacter('U');
                    }
                }
                auto castedSpriteComp
                    = std::dynamic_pointer_cast<DrawableComponent>(spriteComp);
                if (castedSpriteComp) {
                    *castedSpriteComp = *freshSprite;
                    castedSpriteComp->setVisibility(true);
                    castedSpriteComp->setPosition(dotX, dotY);
                } else {
                    spriteComp->setVisibility(true);
                    spriteComp->setPosition(dotX, dotY);
                    spriteComp->setPath(freshSprite->getPath());
                    spriteComp->setDimensions(freshSprite->getWidth(),
                        freshSprite->getHeight());
                    spriteComp->setAsCharacter(freshSprite->getCharacter());
                }
                textureReset++;
            }
        }
    }
}

void GameStateManager::reloadCurrentMap() {
    resetEntireGame();
}

void GameStateManager::increaseGameSpeed() {
    std::shared_ptr<IEntity> pacmanEntity = nullptr;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Pacman") {
            pacmanEntity = entity;
            break;
        }
    }

    if (pacmanEntity) {
        auto pacman = std::dynamic_pointer_cast<PacmanComponent>(
            _componentManager->getComponentByType(pacmanEntity,
                static_cast<ComponentType>(1001)));

        if (pacman) {
            float newThreshold = pacman->getMovementThreshold() * 0.7f;
            pacman->setMovementThreshold(newThreshold);
            float newSpeed = pacman->getMovementSpeed() * 1.2f;
            pacman->setMovementSpeed(newSpeed);
        }
    }

    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));

        if (ghostComp) {
            float newThreshold = ghostComp->getMovementThreshold() * 0.7f;
            ghostComp->setMovementThreshold(newThreshold);
            float newSpeed = ghostComp->getMovementSpeed() * 1.2f;
            ghostComp->setMovementSpeed(newSpeed);
        }
    }
}

void GameStateManager::resetPositions() {
    // Cache frequently used entity lookups
    static std::shared_ptr<IEntity> gridEntity = nullptr;
    if (!gridEntity) {
        for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
            if (name == "Grid") {
                gridEntity = entity;
                break;
            }
        }
    }
    auto grid = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));

    if (!grid)
        return;
    size_t pacmanSpawnX = 0;
    size_t pacmanSpawnY = 0;
    std::vector<std::pair<size_t, size_t>> ghostSpawnPositions;
    for (size_t y = 0; y < grid->getHeight(); y++) {
        for (size_t x = 0; x < grid->getWidth(); x++) {
            if (grid->getCellType(x, y) == CellType::PACMAN_SPAWN) {
                pacmanSpawnX = x;
                pacmanSpawnY = y;
            } else if (grid->getCellType(x, y) == CellType::GHOST_SPAWN) {
                ghostSpawnPositions.push_back({x, y});
            }
        }
    }
    auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));

    if (!gridPosComp)
        return;
    float startX = gridPosComp->x;
    float startY = gridPosComp->y;
    float cellSize = grid->getCellSize();

    // Reset Pac-Man position
    std::shared_ptr<IEntity> pacmanEntity = nullptr;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Pacman") {
            pacmanEntity = entity;
            auto pacman = std::dynamic_pointer_cast<PacmanComponent>(
                _componentManager->getComponentByType(entity,
                    static_cast<ComponentType>(1001)));
            if (pacman) {
                pacman->setGridPosition(pacmanSpawnX, pacmanSpawnY);
                pacman->setCurrentDirection(Direction::NONE);
                pacman->setNextDirection(Direction::NONE);
                pacman->setMoving(false);
                float visualX = startX + (pacmanSpawnX * cellSize);
                float visualY = startY + (pacmanSpawnY * cellSize);
                pacman->setVisualPosition(visualX, visualY);
                pacman->setTargetPosition(visualX, visualY);
                auto drawableComp
                    = std::dynamic_pointer_cast<IDrawableComponent>(
                    _componentManager->getComponentByType(entity,
                        ComponentType::DRAWABLE));
                if (drawableComp) {
                    drawableComp->setPosition(visualX, visualY);
                    drawableComp->setVisibility(true);
                }
                auto posComp
                    = std::dynamic_pointer_cast<PositionComponent>(
                    _componentManager->getComponentByType(entity,
                        ComponentType::POSITION));
                if (posComp) {
                    posComp->x = visualX;
                    posComp->y = visualY;
                }
            }
            break;
        }
    }
    size_t ghostIndex = 0;
    std::vector<std::shared_ptr<IEntity>> ghostEntities;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
        if (ghostComp) {
            ghostEntities.push_back(entity);
        }
    }
    for (auto ghostEntity : ghostEntities) {
        auto ghost = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(ghostEntity,
                static_cast<ComponentType>(1002)));
        if (ghost && ghostIndex < ghostSpawnPositions.size()) {
            auto [spawnX, spawnY] = ghostSpawnPositions[ghostIndex++];
            GhostState prevState = ghost->getState();
            std::string prevStateStr = "";
            switch (prevState) {
                case GhostState::NORMAL: prevStateStr = "NORMAL"; break;
                case GhostState::SCARED: prevStateStr = "SCARED"; break;
                case GhostState::RETURNING: prevStateStr = "RETURNING"; break;
            }
            ghost->setGridPosition(spawnX, spawnY);
            ghost->setCurrentDirection(Direction::NONE);
            ghost->setState(GhostState::NORMAL);
            ghost->setMoving(false);
            float visualX = startX + (spawnX * cellSize);
            float visualY = startY + (spawnY * cellSize);
            ghost->setVisualPosition(visualX, visualY);
            ghost->setTargetPosition(visualX, visualY);
            auto drawableComp = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(ghostEntity,
                    ComponentType::DRAWABLE));
            if (drawableComp) {
                std::string oldPath = drawableComp->getPath();
                bool wasScared = (oldPath.find("scared")
                            != std::string::npos) ||
                            (drawableComp->getColor() == Color::BLUE);
                drawableComp->setPath("");
                drawableComp->setFont("");
                drawableComp->setText("");
                drawableComp->setVisibility(true);
                drawableComp->setScale(1.0f);
                drawableComp->setRotation(0.0f);
                drawableComp->setColor(Color::WHITE);
                drawableComp->setPosition(visualX, visualY);
                std::string ghostAssetKey;
                Color ghostColor = Color::WHITE;
                switch (ghost->getGhostType()) {
                    case GhostType::RED:
                        ghostAssetKey = "ghosts.red";
                        ghostColor = Color::RED;
                        break;
                    case GhostType::PINK:
                        ghostAssetKey = "ghosts.pink";
                        ghostColor = Color::MAGENTA;
                        break;
                    case GhostType::BLUE:
                        ghostAssetKey = "ghosts.blue";
                        ghostColor = Color::CYAN;
                        break;
                    case GhostType::ORANGE:
                        ghostAssetKey = "ghosts.orange";
                        ghostColor = Color::YELLOW;
                        break;
                }
                auto assetIter = _assets.find(ghostAssetKey);
                if (assetIter != _assets.end()) {
                    DrawableComponent freshCopy = assetIter->second;
                    drawableComp->setPath(freshCopy.getPath());
                    drawableComp->setDimensions(freshCopy.getWidth(),
                        freshCopy.getHeight());
                    drawableComp->setAsCharacter(freshCopy.getCharacter());
                } else {
                    std::string fallbackPath;
                    switch (ghost->getGhostType()) {
                        case GhostType::RED:
                            fallbackPath = "assets/pacman/ghost_red.png";
                            drawableComp->setAsCharacter('r');
                            drawableComp->setColor(Color::RED);
                            break;
                        case GhostType::PINK:
                            fallbackPath = "assets/pacman/ghost_pink.png";
                            drawableComp->setAsCharacter('i');
                            drawableComp->setColor(Color::MAGENTA);
                            break;
                        case GhostType::BLUE:
                            fallbackPath = "assets/pacman/ghost_blue.png";
                            drawableComp->setAsCharacter('c');
                            drawableComp->setColor(Color::CYAN);
                            break;
                        case GhostType::ORANGE:
                            fallbackPath = "assets/pacman/ghost_orange.png";
                            drawableComp->setAsCharacter('o');
                            drawableComp->setColor(Color::YELLOW);
                            break;
                    }
                    drawableComp->setAsTexture(fallbackPath, 32, 32);
                }
                drawableComp->setColor(ghostColor);
                drawableComp->setVisibility(true);
                drawableComp->setPosition(visualX, visualY);
                std::string newPath = drawableComp->getPath();
                if (newPath.find("scared") != std::string::npos ||
                    newPath.find("blue") != std::string::npos ||
                    drawableComp->getColor() == Color::BLUE) {
                    switch (ghost->getGhostType()) {
                        case GhostType::RED:
                            drawableComp->setAsTexture
                                ("assets/pacman/ghost_red.png", 32, 32);
                            drawableComp->setColor(Color::RED);
                            break;
                        case GhostType::PINK:
                            drawableComp->setAsTexture
                                ("assets/pacman/ghost_pink.png", 32, 32);
                            drawableComp->setColor(Color::MAGENTA);
                            break;
                        case GhostType::BLUE:
                            drawableComp->setAsTexture
                                ("assets/pacman/ghost_blue.png", 32, 32);
                            drawableComp->setColor(Color::CYAN);
                            break;
                        case GhostType::ORANGE:
                            drawableComp->setAsTexture
                                ("assets/pacman/ghost_orange.png", 32, 32);
                            drawableComp->setColor(Color::YELLOW);
                            break;
                    }
                    drawableComp->setVisibility(true);
                    drawableComp->setPosition(visualX, visualY);
                }
            }
            auto posComp = std::dynamic_pointer_cast<PositionComponent>(
                _componentManager->getComponentByType(ghostEntity,
                    ComponentType::POSITION));
            if (posComp) {
                posComp->x = visualX;
                posComp->y = visualY;
            }
        }
    }
}

}  // namespace PacMan
}  // namespace Arcade
