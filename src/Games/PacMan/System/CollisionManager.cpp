// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Collision Manager for Pacman implementation
*/

#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <string>
#include <map>
#include "Games/PacMan/System/CollisionManager.hpp"
#include "Games/PacMan/System/GameStateManager.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "Games/PacMan/Components/FoodComponent.hpp"
#include "Games/PacMan/Components/GhostComponent.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
namespace Arcade {
namespace PacMan {

CollisionManager::CollisionManager(
std::shared_ptr<Arcade::IComponentManager> componentManager,
std::shared_ptr<Arcade::IEntityManager> entityManager,
const std::map<std::string, DrawableComponent>& assets)
: _componentManager(componentManager),
_entityManager(entityManager),
_assets(assets) {
    _cachedAssets["ghosts.eyes"] = getDrawableAsset("ghosts.eyes");
    _cachedAssets["map.empty"] = getDrawableAsset("map.empty");
    _cachedAssets["ghosts.frightened"] = getDrawableAsset("ghosts.frightened");
    _cachedGameStateManager
        = std::make_shared<GameStateManager>(_componentManager,
            _entityManager, _assets);
    _rebuildCollisionGridTimer = 0.0f;
    rebuildCollisionGrid();
}

std::shared_ptr<IDrawableComponent>
CollisionManager::getDrawableAsset(const std::string& key) const {
    auto it = _assets.find(key);
    if (it != _assets.end()) {
        auto component = std::make_shared<DrawableComponent>(it->second);
        return component;
    }
    return nullptr;
}

void CollisionManager::rebuildCollisionGrid() {
    _entityGrid.clear();
    std::shared_ptr<IEntity> gridEntity = nullptr;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Grid") {
            gridEntity = entity;
            break;
        }
    }
    if (!gridEntity) return;
    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));
    if (!gridComp) return;
    float cellSize = gridComp->getCellSize();
    auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));
    float gridStartX = gridPosComp ? gridPosComp->x : 0.0f;
    float gridStartY = gridPosComp ? gridPosComp->y : 0.0f;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
        if (ghostComp) {
            size_t gridX = static_cast<size_t>((ghostComp->getVisualX()
                - gridStartX) / cellSize);
            size_t gridY = static_cast<size_t>((ghostComp->getVisualY()
                - gridStartY) / cellSize);
            std::pair<size_t, size_t> pos = {gridX, gridY};
            _entityGrid[pos].push_back({entity, EntityType::GHOST});
            continue;
        }
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1003)));

        if (foodComp && !foodComp->isEaten()) {
            auto spriteComp = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));
            if (spriteComp && spriteComp->isRenderable()) {
                std::pair<size_t, size_t> pos
                    = {foodComp->getGridX(), foodComp->getGridY()};
                _entityGrid[pos].push_back({entity, EntityType::FOOD});
            }
        }
    }
}

void CollisionManager::checkCollisions(std::shared_ptr<PacmanComponent> pacman,
std::shared_ptr<GridComponent> grid,
std::shared_ptr<IEntity> pacmanEntity,
std::shared_ptr<IEntity> gridEntity) {
    if (!pacman || !grid || !pacmanEntity || !gridEntity)
        return;
    float pacmanLeft = pacman->getVisualX();
    float pacmanTop = pacman->getVisualY();
    float pacmanWidth = 32.0f;
    float pacmanHeight = 32.0f;
    float pacmanHitboxScale = 0.6f;
    float pacmanHitboxPadding = (1.0f - pacmanHitboxScale)
        * 0.5f * pacmanWidth;
    float hitboxLeft = pacmanLeft + pacmanHitboxPadding;
    float hitboxTop = pacmanTop + pacmanHitboxPadding;
    float hitboxWidth = pacmanWidth * pacmanHitboxScale;
    float hitboxHeight = pacmanHeight * pacmanHitboxScale;
    constexpr float collisionMargin = 32.0f;
    float checkLeft = pacmanLeft - collisionMargin;
    float checkTop = pacmanTop - collisionMargin;
    float checkRight = pacmanLeft + pacmanWidth + collisionMargin;
    float checkBottom = pacmanTop + pacmanHeight + collisionMargin;
    _rebuildCollisionGridTimer += 1.0f / 60.0f;
    if (_rebuildCollisionGridTimer >= COLLISION_GRID_REFRESH_TIME) {
        rebuildCollisionGrid();
        _rebuildCollisionGridTimer = 0.0f;
    }
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
        if (!ghostComp) continue;
        float ghostLeft = ghostComp->getVisualX();
        float ghostTop = ghostComp->getVisualY();
        float ghostWidth = 32.0f;
        float ghostHeight = 32.0f;
        float ghostHitboxScale = 0.6f;
        float ghostHitboxPadding = (1.0f - ghostHitboxScale)
            * 0.5f * ghostWidth;
        float ghostHitboxLeft = ghostLeft + ghostHitboxPadding;
        float ghostHitboxTop = ghostTop + ghostHitboxPadding;
        float ghostHitboxWidth = ghostWidth * ghostHitboxScale;
        float ghostHitboxHeight = ghostHeight * ghostHitboxScale;
        if (ghostLeft > checkRight || ghostLeft + ghostWidth < checkLeft ||
            ghostTop > checkBottom || ghostTop + ghostHeight < checkTop) {
            continue;
        }
        if (!(hitboxLeft + hitboxWidth < ghostHitboxLeft ||
              hitboxLeft > ghostHitboxLeft + ghostHitboxWidth ||
              hitboxTop + hitboxHeight < ghostHitboxTop ||
              hitboxTop > ghostHitboxTop + ghostHitboxHeight)) {
            if (ghostComp->getState() == GhostState::SCARED) {
                ghostComp->setState(GhostState::RETURNING);
                pacman->addScore(200);
                auto ghostSprite
                    = std::dynamic_pointer_cast<IDrawableComponent>(
                    _componentManager->getComponentByType(entity,
                        ComponentType::DRAWABLE));
                if (ghostSprite) {
                    std::string oldPath = ghostSprite->getPath();
                    char oldChar = ghostSprite->getCharacter();
                    ghostSprite->setVisibility(true);
                    ghostSprite->setScale(1.0f);
                    ghostSprite->setRotation(0.0f);
                    ghostSprite->setColor(Color::WHITE);
                    ghostSprite->setPath("");
                    ghostSprite->setText("");
                    ghostSprite->setFont("");
                    float visualX = ghostComp->getVisualX();
                    float visualY = ghostComp->getVisualY();
                    auto eyesAsset = _cachedAssets["ghosts.eyes"];
                    if (eyesAsset) {
                        ghostSprite->setPath(eyesAsset->getPath());
                        ghostSprite->setDimensions(eyesAsset->getWidth(),
                            eyesAsset->getHeight());
                        ghostSprite->setAsCharacter(eyesAsset->getCharacter());
                    } else {
                        ghostSprite->setAsTexture("assets/pacman/eyes.png",
                            32, 32);
                        ghostSprite->setAsCharacter('e');
                    }
                    ghostSprite->setPosition(visualX, visualY);
                }
            } else if (ghostComp->getState() == GhostState::NORMAL) {
                pacman->decrementLives();
                if (pacman->getLives() > 0) {
                    pacman->setMoving(false);
                    for (const auto& [e, n] :
                        _entityManager->getEntitiesMap()) {
                        auto ghost
                        = std::dynamic_pointer_cast<GhostComponent>(
                            _componentManager->getComponentByType
                                (e, static_cast<ComponentType>(1002)));
                        if (ghost) {
                            ghost->setMoving(false);
                        }
                    }
                    _cachedGameStateManager->resetPositions();

                    for (const auto& [e, n]
                        : _entityManager->getEntitiesMap()) {
                        if (n == "Pacman") {
                            auto p = std::dynamic_pointer_cast<PacmanComponent>(
                                _componentManager->getComponentByType
                                    (e, static_cast<ComponentType>(1001)));
                            if (p) {
                                p->setMoving(false);
                                p->setCanMove(true);
                                p->resetMovementTimer();
                                p->setCurrentDirection(Direction::NONE);
                                p->setNextDirection(Direction::NONE);
                            }
                        }
                        auto ghost = std::dynamic_pointer_cast<GhostComponent>(
                            _componentManager->getComponentByType
                            (e, static_cast<ComponentType>(1002)));
                        if (ghost) {
                            ghost->setMoving(false);
                            ghost->setCanMove(true);
                            ghost->resetMovementTimer();
                            ghost->setCurrentDirection(Direction::NONE);
                        }
                    }
                } else {
                    grid->setGameOver(true);
                    for (const auto& [ghostEntity, name]
                        : _entityManager->getEntitiesMap()) {
                        auto ghost = std::dynamic_pointer_cast<GhostComponent>(
                            _componentManager->getComponentByType(ghostEntity,
                                static_cast<ComponentType>(1002)));
                        if (ghost) {
                            auto ghostDrawable
                                = std::dynamic_pointer_cast<IDrawableComponent>(
                                _componentManager->getComponentByType
                                    (ghostEntity,
                                    ComponentType::DRAWABLE));
                            if (ghostDrawable) {
                                ghostDrawable->setVisibility(true);
                            }
                        }
                    }
                }
                return;
            }
        }
    }
    float cellSize = grid->getCellSize();
    auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType
        (gridEntity, ComponentType::POSITION));
    float gridStartX = gridPosComp ? gridPosComp->x : 0.0f;
    float gridStartY = gridPosComp ? gridPosComp->y : 0.0f;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1003)));
        if (!foodComp || foodComp->isEaten()) continue;
        float foodLeft = gridStartX + (foodComp->getGridX() * cellSize);
        float foodTop = gridStartY + (foodComp->getGridY() * cellSize);
        float foodWidth = cellSize;
        float foodHeight = cellSize;
        if (foodLeft > checkRight || foodLeft + foodWidth < checkLeft ||
            foodTop > checkBottom || foodTop + foodHeight < checkTop) {
            continue;
        }
        float foodHitboxScale = 0.15f;
        float hitboxPadding = (1.0f - foodHitboxScale) * 0.5f * cellSize;
        float foodHitboxLeft = foodLeft + hitboxPadding;
        float foodHitboxTop = foodTop + hitboxPadding;
        float foodHitboxWidth = cellSize * foodHitboxScale;
        float foodHitboxHeight = cellSize * foodHitboxScale;
        if (!(hitboxLeft + hitboxWidth < foodHitboxLeft ||
              hitboxLeft > foodHitboxLeft + foodHitboxWidth ||
              hitboxTop + hitboxHeight < foodHitboxTop ||
              hitboxTop > foodHitboxTop + foodHitboxHeight)) {
            foodComp->setEaten(true);
            pacman->addScore(foodComp->getPoints());
            grid->decrementFoodCount();
            auto spriteComp = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType
                (entity, ComponentType::DRAWABLE));
            if (spriteComp) {
                spriteComp->setVisibility(false);
                auto emptyAsset = _cachedAssets["map.empty"];
                if (emptyAsset) {
                    spriteComp->setPath(emptyAsset->getPath());
                    spriteComp->setDimensions(emptyAsset->getWidth(),
                        emptyAsset->getHeight());
                    spriteComp->setAsCharacter(' ');
                } else {
                    spriteComp->setAsTexture("assets/pacman/empty.png", 32, 32);
                    spriteComp->setAsCharacter(' ');
                }
                rebuildCollisionGrid();
            }
            if (foodComp->getFoodType() == FoodType::POWER_PILL) {
                updateGhostsForPowerPill();
            }
            _rebuildCollisionGridTimer = COLLISION_GRID_REFRESH_TIME;
        }
    }
}

void CollisionManager::updateGhostsForPowerPill() {
    if (_cachedGhostEntities.empty()) {
        for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
            auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
                _componentManager->getComponentByType(entity,
                    static_cast<ComponentType>(1002)));
            if (ghostComp) {
                _cachedGhostEntities.push_back(entity);
            }
        }
    }
    auto frightenedAsset = _assets.find("ghosts.frightened");
    bool assetFound = frightenedAsset != _assets.end();
    int ghostCount = 0;
    for (const auto& ghostEntity : _cachedGhostEntities) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(ghostEntity,
                static_cast<ComponentType>(1002)));
        if (!ghostComp) continue;
        ghostCount++;
        GhostState previousState = ghostComp->getState();
        ghostComp->setState(GhostState::SCARED);
        ghostComp->resetStateTimer();
        auto ghostSprite = std::dynamic_pointer_cast<IDrawableComponent>(
            _componentManager->getComponentByType(ghostEntity,
                ComponentType::DRAWABLE));
        if (ghostSprite) {
            float ghostX = ghostComp->getVisualX();
            float ghostY = ghostComp->getVisualY();
            std::string prevTexture = ghostSprite->getPath();
            std::string prevChar = std::string(1, ghostSprite->getCharacter());
            ghostSprite->setPath("");
            ghostSprite->setText("");
            ghostSprite->setFont("");
            ghostSprite->setColor(Color::BLUE);
            ghostSprite->setScale(1.0f);
            ghostSprite->setRotation(0.0f);
            ghostSprite->setVisibility(true);
            ghostSprite->setAsCharacter('S');
            ghostSprite->setDimensions(32, 32);
            if (assetFound) {
                ghostSprite->setPath(frightenedAsset->second.getPath());
                ghostSprite->setDimensions(frightenedAsset->second.getWidth(),
                                          frightenedAsset->second.getHeight());
            } else {
                ghostSprite->setAsTexture
                    ("assets/pacman/scared_ghost.png", 32, 32);
            }
            ghostSprite->setPosition(ghostX, ghostY);
            if (ghostSprite->getColor() != Color::BLUE ||
                ghostSprite->getPath().find("scared") == std::string::npos) {
                ghostSprite->setAsTexture("assets/pacman/scared_ghost.png",
                    32, 32);
                ghostSprite->setColor(Color::BLUE);
                ghostSprite->setPosition(ghostX, ghostY);
            }
        }
    }
}

void CollisionManager::findAndSetPacmanSpawn
(std::shared_ptr<PacmanComponent> pacman,
std::shared_ptr<GridComponent> grid) {
    static bool spawnPointFound = false;
    static size_t spawnX = 0;
    static size_t spawnY = 0;
    if (!spawnPointFound) {
        for (size_t y = 0; y < grid->getHeight(); y++) {
            for (size_t x = 0; x < grid->getWidth(); x++) {
                if (grid->getCellType(x, y) == CellType::PACMAN_SPAWN) {
                    spawnX = x;
                    spawnY = y;
                    spawnPointFound = true;
                    break;
                }
            }
            if (spawnPointFound) break;
        }
    }
    pacman->setGridPosition(spawnX, spawnY);
    pacman->setCurrentDirection(Direction::NONE);
    pacman->setNextDirection(Direction::NONE);
}

void CollisionManager::checkCollisionsWithGhosts(
std::shared_ptr<PacmanComponent> pacman,
std::shared_ptr<GridComponent> grid) {
}

void CollisionManager::checkCollisionsWithFood
(std::shared_ptr<PacmanComponent> pacman,
std::shared_ptr<GridComponent> grid) {
}

void CollisionManager::checkFoodCollision
(std::shared_ptr<PacmanComponent> pacman,
std::shared_ptr<GridComponent> grid) {
}

}  // namespace PacMan
}  // namespace Arcade
