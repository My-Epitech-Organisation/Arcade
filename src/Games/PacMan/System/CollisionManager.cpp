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
#include "Games/PacMan/System/CollisionManager.hpp"
#include "Games/PacMan/System/GameStateManager.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "Games/PacMan/Components/FoodComponent.hpp"
#include "Games/PacMan/Components/GhostComponent.hpp"
#include "ECS/Components/Position/PositionComponent.hpp" // Add this include for PositionComponent

namespace Arcade {
namespace PacMan {

CollisionManager::CollisionManager(std::shared_ptr<Arcade::IComponentManager> componentManager,
                                  std::shared_ptr<Arcade::IEntityManager> entityManager,
                                  const std::map<std::string, DrawableComponent>& assets)
    : _componentManager(componentManager), _entityManager(entityManager), _assets(assets) {
    
    // Pre-cache asset lookups
    _cachedAssets["ghosts.eyes"] = getDrawableAsset("ghosts.eyes");
    _cachedAssets["map.empty"] = getDrawableAsset("map.empty");
    _cachedAssets["ghosts.frightened"] = getDrawableAsset("ghosts.frightened");
    
    // Initialize collision grid for faster lookups
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
    
    // Get the grid entity for reference positions
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
        _componentManager->getComponentByType(gridEntity, ComponentType::POSITION));
    float gridStartX = gridPosComp ? gridPosComp->x : 0.0f;
    float gridStartY = gridPosComp ? gridPosComp->y : 0.0f;
    
    // Only add entities that are NOT eaten to the collision grid
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        // Add ghosts to the collision grid based on their visual positions
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
                
        if (ghostComp) {
            // Convert visual position to grid coordinates
            size_t gridX = static_cast<size_t>((ghostComp->getVisualX() - gridStartX) / cellSize);
            size_t gridY = static_cast<size_t>((ghostComp->getVisualY() - gridStartY) / cellSize);
            std::pair<size_t, size_t> pos = {gridX, gridY};
            _entityGrid[pos].push_back({entity, EntityType::GHOST});
            continue;
        }
        
        // Only add non-eaten food to the collision grid
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1003)));
                
        if (foodComp && !foodComp->isEaten()) {
            auto spriteComp = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));
            
            if (spriteComp && spriteComp->isRenderable()) {
                std::pair<size_t, size_t> pos = {foodComp->getGridX(), foodComp->getGridY()};
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
    
    // Get pacman's visual position and dimensions for collision check
    float pacmanLeft = pacman->getVisualX();
    float pacmanTop = pacman->getVisualY();
    float pacmanWidth = 32.0f;  // Default sprite size, should match actual sprite
    float pacmanHeight = 32.0f;
    
    // Apply a hitbox reduction factor - make hitbox ~60% of sprite size
    float pacmanHitboxScale = 0.6f;
    float pacmanHitboxPadding = (1.0f - pacmanHitboxScale) * 0.5f * pacmanWidth; 
    float hitboxLeft = pacmanLeft + pacmanHitboxPadding;
    float hitboxTop = pacmanTop + pacmanHitboxPadding;
    float hitboxWidth = pacmanWidth * pacmanHitboxScale;
    float hitboxHeight = pacmanHeight * pacmanHitboxScale;
    
    // For optimization: Calculate a region of interest around Pacman for narrowing collision checks
    constexpr float collisionMargin = 32.0f; // Check slightly beyond Pacman's position
    float checkLeft = pacmanLeft - collisionMargin;
    float checkTop = pacmanTop - collisionMargin;
    float checkRight = pacmanLeft + pacmanWidth + collisionMargin;
    float checkBottom = pacmanTop + pacmanHeight + collisionMargin;
    
    // Update collision grid periodically, not every frame
    _rebuildCollisionGridTimer += 1.0f / 60.0f; // Assuming 60fps
    if (_rebuildCollisionGridTimer >= COLLISION_GRID_REFRESH_TIME) {
        rebuildCollisionGrid();
        _rebuildCollisionGridTimer = 0.0f;
    }
    
    // Check collisions with ghosts using bounding boxes
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
                
        if (!ghostComp) continue;
        
        // Get ghost's visual position
        float ghostLeft = ghostComp->getVisualX();
        float ghostTop = ghostComp->getVisualY();
        float ghostWidth = 32.0f;  // Default sprite size
        float ghostHeight = 32.0f;
        
        // Apply the same hitbox reduction to ghosts
        float ghostHitboxScale = 0.6f;
        float ghostHitboxPadding = (1.0f - ghostHitboxScale) * 0.5f * ghostWidth;
        float ghostHitboxLeft = ghostLeft + ghostHitboxPadding;
        float ghostHitboxTop = ghostTop + ghostHitboxPadding;
        float ghostHitboxWidth = ghostWidth * ghostHitboxScale;
        float ghostHitboxHeight = ghostHeight * ghostHitboxScale;
        
        // Skip ghosts outside the region of interest
        if (ghostLeft > checkRight || ghostLeft + ghostWidth < checkLeft ||
            ghostTop > checkBottom || ghostTop + ghostHeight < checkTop) {
            continue;
        }
        
        // Check for collision using reduced-size bounding boxes
        if (!(hitboxLeft + hitboxWidth < ghostHitboxLeft ||
              hitboxLeft > ghostHitboxLeft + ghostHitboxWidth ||
              hitboxTop + hitboxHeight < ghostHitboxTop ||
              hitboxTop > ghostHitboxTop + ghostHitboxHeight)) {
            if (ghostComp->getState() == GhostState::SCARED) {
                // Eat ghost logic
                ghostComp->setState(GhostState::RETURNING);
                pacman->addScore(200);
                
                auto ghostSprite = std::dynamic_pointer_cast<IDrawableComponent>(
                    _componentManager->getComponentByType(entity,
                        ComponentType::DRAWABLE));
                        
                auto eyesAsset = _cachedAssets["ghosts.eyes"];
                if (eyesAsset) {
                    *ghostSprite = *eyesAsset;
                } else {
                    ghostSprite->setAsTexture("assets/pacman/eyes.png", 32, 32);
                    ghostSprite->setAsCharacter('e');
                }
            } else if (ghostComp->getState() == GhostState::NORMAL) {
                // Lose a life
                pacman->decrementLives();
                
                if (pacman->getLives() > 0) {
                    // Reset positions but keep playing
                    _cachedGameStateManager = 
                        _cachedGameStateManager ? _cachedGameStateManager :
                        std::make_shared<GameStateManager>(_componentManager, _entityManager, _assets);
                    _cachedGameStateManager->resetPositions();
                } else {
                    // Game over
                    std::cout << "Game over - setting game over state" << std::endl;
                    grid->setGameOver(true);
                    
                    // Make ghosts visible after death
                    for (const auto& [ghostEntity, name] : _entityManager->getEntitiesMap()) {
                        auto ghost = std::dynamic_pointer_cast<GhostComponent>(
                            _componentManager->getComponentByType(ghostEntity,
                                static_cast<ComponentType>(1002)));
                                
                        if (ghost) {
                            auto ghostDrawable = std::dynamic_pointer_cast<IDrawableComponent>(
                                _componentManager->getComponentByType(ghostEntity,
                                    ComponentType::DRAWABLE));
                                    
                            if (ghostDrawable) {
                                ghostDrawable->setVisibility(true);
                            }
                        }
                    }
                }
                
                // Always break after ghost collision
                return;
            }
        }
    }
    
    // Check collisions with food using visual positions
    // First, get grid cell size and position for reference
    float cellSize = grid->getCellSize();
    auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity, ComponentType::POSITION));
    float gridStartX = gridPosComp ? gridPosComp->x : 0.0f;
    float gridStartY = gridPosComp ? gridPosComp->y : 0.0f;
    
    // Check collisions with food dots
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1003)));
                
        if (!foodComp || foodComp->isEaten()) continue;
        
        // Calculate food's visual position based on its grid position
        float foodLeft = gridStartX + (foodComp->getGridX() * cellSize);
        float foodTop = gridStartY + (foodComp->getGridY() * cellSize);
        float foodWidth = cellSize;
        float foodHeight = cellSize;
        
        // Skip food outside the region of interest
        if (foodLeft > checkRight || foodLeft + foodWidth < checkLeft ||
            foodTop > checkBottom || foodTop + foodHeight < checkTop) {
            continue;
        }
        
        // Check for collision using a smaller hitbox for the food (15% of cell size)
        float foodHitboxScale = 0.15f;  // Even smaller food hitbox
        float hitboxPadding = (1.0f - foodHitboxScale) * 0.5f * cellSize;
        float foodHitboxLeft = foodLeft + hitboxPadding;
        float foodHitboxTop = foodTop + hitboxPadding;
        float foodHitboxWidth = cellSize * foodHitboxScale;
        float foodHitboxHeight = cellSize * foodHitboxScale;
        
        if (!(hitboxLeft + hitboxWidth < foodHitboxLeft ||
              hitboxLeft > foodHitboxLeft + foodHitboxWidth ||
              hitboxTop + hitboxHeight < foodHitboxTop ||
              hitboxTop > foodHitboxTop + foodHitboxHeight)) {
            // Food collision logic
            foodComp->setEaten(true);
            pacman->addScore(foodComp->getPoints());
            grid->decrementFoodCount();
            
            // Get and update the sprite component
            auto spriteComp = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(entity, ComponentType::DRAWABLE));
            
            if (spriteComp) {
                // Set visibility to false
                spriteComp->setVisibility(false);
                
                // Use the empty sprite texture for the cell
                auto emptyAsset = _cachedAssets["map.empty"];
                if (emptyAsset) {
                    // Copy properties from the empty asset
                    spriteComp->setPath(emptyAsset->getPath());
                    spriteComp->setDimensions(emptyAsset->getWidth(), emptyAsset->getHeight());
                    spriteComp->setAsCharacter(' ');
                } else {
                    spriteComp->setAsTexture("assets/pacman/empty.png", 32, 32);
                    spriteComp->setAsCharacter(' ');
                }
                
                // Remove the entity from the collision grid
                rebuildCollisionGrid();
            }
            
            // Handle power pill effect
            if (foodComp->getFoodType() == FoodType::POWER_PILL) {
                updateGhostsForPowerPill();
            }
            
            // Force a collision grid rebuild next frame
            _rebuildCollisionGridTimer = COLLISION_GRID_REFRESH_TIME;
        }
    }
}

void CollisionManager::updateGhostsForPowerPill() {
    // Get all ghosts from the cache
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
    
    auto frightenedAsset = _cachedAssets["ghosts.frightened"];
    
    for (const auto& ghostEntity : _cachedGhostEntities) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(ghostEntity,
                static_cast<ComponentType>(1002)));
                
        if (!ghostComp) continue;
        
        ghostComp->setState(GhostState::SCARED);
        ghostComp->resetStateTimer();

        auto ghostSprite = std::dynamic_pointer_cast<IDrawableComponent>(
            _componentManager->getComponentByType(ghostEntity,
                ComponentType::DRAWABLE));
                
        if (ghostSprite) {
            if (frightenedAsset) {
                *ghostSprite = *frightenedAsset;
            } else {
                ghostSprite->setAsTexture("assets/pacman/scared_ghost.png", 32, 32);
                ghostSprite->setAsCharacter('s');
            }
        }
    }
}

void CollisionManager::findAndSetPacmanSpawn(std::shared_ptr<PacmanComponent> pacman,
                                           std::shared_ptr<GridComponent> grid) {
    // We'll cache the spawn point to avoid searching the grid every time
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
    
    // Set pacman to spawn position
    pacman->setGridPosition(spawnX, spawnY);
    pacman->setCurrentDirection(Direction::NONE);
    pacman->setNextDirection(Direction::NONE);
}

void CollisionManager::checkCollisionsWithGhosts(std::shared_ptr<PacmanComponent> pacman,
                                               std::shared_ptr<GridComponent> grid) {
    // This function is replaced by the optimized checkCollisions method
}

void CollisionManager::checkCollisionsWithFood(std::shared_ptr<PacmanComponent> pacman,
                                             std::shared_ptr<GridComponent> grid) {
    // This function is replaced by the optimized checkCollisions method
}

void CollisionManager::checkFoodCollision(std::shared_ptr<PacmanComponent> pacman,
                                        std::shared_ptr<GridComponent> grid) {
    // This function is replaced by the optimized checkCollisions method
}

}  // namespace PacMan
}  // namespace Arcade
