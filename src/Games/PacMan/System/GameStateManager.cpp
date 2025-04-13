// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game State Manager for Pacman implementation
*/

#include <iostream>
#include <memory>
#include "Games/PacMan/System/GameStateManager.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "Games/PacMan/Components/FoodComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "Games/PacMan/Components/GhostComponent.hpp"


namespace Arcade {
namespace PacMan {

GameStateManager::GameStateManager(std::shared_ptr<Arcade::IComponentManager> componentManager,
                                  std::shared_ptr<Arcade::IEntityManager> entityManager,
                                  const std::map<std::string, DrawableComponent>& assets)
    : _componentManager(componentManager), _entityManager(entityManager), _assets(assets) {
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
    
    // Additional debug output
    static int lastFoodCount = -1;
    if (lastFoodCount != grid->getFoodCount()) {
        std::cout << "Food count: " << grid->getFoodCount() << "/" 
                  << grid->getTotalFoodCount() << std::endl;
        lastFoodCount = grid->getFoodCount();
    }
        
    // Win if all food is eaten
    if (grid->getFoodCount() <= 0) {
        std::cout << "Level completed! Moving to next level." << std::endl;
        
        // Clear existing game over flag (in case it was set)
        grid->setGameOver(false);
        
        // Set win flag
        grid->setGameWon(true);
        
        // Advance level
        grid->incrementLevel();
        std::cout << "Advanced to level " << grid->getLevel() << std::endl;
        
        // Reload map with new settings
        reloadCurrentMap();
        
        // Increase game speed for next level
        increaseGameSpeed();
    }
}

void GameStateManager::resetEntireGame() {
    // First, reset all positions
    resetPositions();
    
    // ENHANCED: Reset Pacman score and lives with better logging and verification
    bool scoreProperlySaved = false;
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Pacman") {
            auto pacman = std::dynamic_pointer_cast<PacmanComponent>(
                _componentManager->getComponentByType(entity,
                    static_cast<ComponentType>(1001)));
            
            if (pacman) {
                int oldScore = pacman->getScore();
                std::cout << "SCORE RESET: Pacman score from " << oldScore << " to 0" << std::endl;
                
                // Explicitly reset score to 0
                pacman->setScore(0);
                pacman->setLives(3);
                
                // Verify score was reset - important for ensuring it worked
                if (pacman->getScore() != 0) {
                    std::cerr << "WARNING: Score reset failed! Score is still: " << pacman->getScore() << std::endl;
                } else {
                    scoreProperlySaved = true;
                    std::cout << "✓ Score successfully reset to 0" << std::endl;
                }
            }
        }
    }
    
    if (!scoreProperlySaved) {
        std::cerr << "WARNING: Could not find Pacman entity to reset score!" << std::endl;
    }
    
    // Find the grid entity
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
            // Reset food count
            grid->setFoodCount(grid->getTotalFoodCount());
            std::cout << "Full game reset: Restored all " << grid->getTotalFoodCount() << " food items" << std::endl;
            
            // Reset any game state flags
            grid->setGameOver(false);
            grid->setGameWon(false);
        }
    }
    
    // Improved food entity reset
    int totalFoodReset = 0;
    int visibilityFixed = 0;
    int textureReset = 0;
    
    // Reset all food entities to uneaten
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1003)));
                
        if (foodComp) {
            // Explicitly mark as not eaten
            bool wasEaten = foodComp->isEaten();
            foodComp->setEaten(false);
            totalFoodReset++;
            
            auto spriteComp = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));
                    
            if (spriteComp) {
                // CRITICAL: First, ensure sprite is fully visible regardless of other properties
                if (!spriteComp->isRenderable()) {
                    spriteComp->setVisibility(true);
                    visibilityFixed++;
                }
                
                // Get the visual position from the grid reference
                float gridStartX = 0.0f;
                float gridStartY = 0.0f;
                float cellSize = 32.0f; // Default value
                
                // Find grid entity for position reference
                for (const auto& [gEntity, gName] : _entityManager->getEntitiesMap()) {
                    if (gName == "Grid") {
                        auto gridComp = std::dynamic_pointer_cast<GridComponent>(
                            _componentManager->getComponentByType(gEntity,
                                static_cast<ComponentType>(1000)));
                        if (gridComp) {
                            cellSize = gridComp->getCellSize();
                        }
                        
                        auto gridPos = std::dynamic_pointer_cast<PositionComponent>(
                            _componentManager->getComponentByType(gEntity,
                                ComponentType::POSITION));
                        if (gridPos) {
                            gridStartX = gridPos->x;
                            gridStartY = gridPos->y;
                        }
                        break;
                    }
                }
                
                // CRUCIAL: Calculate the right position based on grid coordinates
                float dotX = gridStartX + (foodComp->getGridX() * cellSize);
                float dotY = gridStartY + (foodComp->getGridY() * cellSize);
                
                // Reset food sprite based on type - use FRESH component to avoid inheritance issues
                std::shared_ptr<DrawableComponent> freshSprite = std::make_shared<DrawableComponent>();
                
                if (foodComp->getFoodType() == FoodType::NORMAL_DOT) {
                    auto foodAsset = getDrawableAsset("map.food");
                    if (foodAsset) {
                        // FIX: Need to downcast from IDrawableComponent to DrawableComponent
                        auto castedFoodAsset = std::dynamic_pointer_cast<DrawableComponent>(foodAsset);
                        if (castedFoodAsset) {
                            *freshSprite = *castedFoodAsset;
                        } else {
                            // Fallback if cast fails
                            freshSprite->setAsTexture("assets/pacman/dot.png", 32, 32);
                            freshSprite->setAsCharacter('.');
                        }
                    } else {
                        freshSprite->setAsTexture("assets/pacman/dot.png", 32, 32);
                        freshSprite->setAsCharacter('.');
                    }
                } else { // Power pill
                    auto powerAsset = getDrawableAsset("map.power_pellet");
                    if (powerAsset) {
                        // FIX: Need to downcast from IDrawableComponent to DrawableComponent
                        auto castedPowerAsset = std::dynamic_pointer_cast<DrawableComponent>(powerAsset);
                        if (castedPowerAsset) {
                            *freshSprite = *castedPowerAsset;
                        } else {
                            // Fallback if cast fails
                            freshSprite->setAsTexture("assets/pacman/power_pellet.png", 32, 32);
                            freshSprite->setAsCharacter('U');
                        }
                    } else {
                        freshSprite->setAsTexture("assets/pacman/power_pellet.png", 32, 32);
                        freshSprite->setAsCharacter('U');
                    }
                }
                
                // FIX: We also need to downcast spriteComp to DrawableComponent to use assignment operator
                auto castedSpriteComp = std::dynamic_pointer_cast<DrawableComponent>(spriteComp);
                if (castedSpriteComp) {
                    *castedSpriteComp = *freshSprite;
                    
                    // CRITICAL: Force visibility and position AFTER the copy
                    castedSpriteComp->setVisibility(true);
                    castedSpriteComp->setPosition(dotX, dotY);
                } else {
                    // Manual properties copy if downcast fails
                    spriteComp->setVisibility(true);
                    spriteComp->setPosition(dotX, dotY);
                    spriteComp->setPath(freshSprite->getPath());
                    spriteComp->setDimensions(freshSprite->getWidth(), freshSprite->getHeight());
                    spriteComp->setAsCharacter(freshSprite->getCharacter());
                }
                
                textureReset++;
                
                if (wasEaten) {
                    std::cout << "Restored food at (" << foodComp->getGridX() << "," 
                              << foodComp->getGridY() << ")" << std::endl;
                }
            }
        }
    }
    
    std::cout << "✓ Reset " << totalFoodReset << " food items - Fixed visibility: " 
              << visibilityFixed << ", Reset textures: " << textureReset << std::endl;
    
    std::cout << "Full game reset completed - SCORE RESET TO 0 - ALL FOOD RESTORED" << std::endl;
}

void GameStateManager::reloadCurrentMap() {
    // For a complete restart, use resetEntireGame() which now handles everything properly
    resetEntireGame();
    
    // No need for duplicate code here as resetEntireGame handles everything
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
            // More aggressive speed increase with level
            float newThreshold = pacman->getMovementThreshold() * 0.7f;
            pacman->setMovementThreshold(newThreshold);
            
            // Also increase visual movement speed
            float newSpeed = pacman->getMovementSpeed() * 1.2f;
            pacman->setMovementSpeed(newSpeed);
        }
    }

    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));

        if (ghostComp) {
            // More aggressive speed increase with level
            float newThreshold = ghostComp->getMovementThreshold() * 0.7f;
            ghostComp->setMovementThreshold(newThreshold);
            
            // Also increase visual movement speed
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

    // Find spawn positions for Pac-Man and ghosts
    size_t pacmanSpawnX = 0;
    size_t pacmanSpawnY = 0;
    std::vector<std::pair<size_t, size_t>> ghostSpawnPositions;
    
    for (size_t y = 0; y < grid->getHeight(); y++) {
        for (size_t x = 0; x < grid->getWidth(); x++) {
            if (grid->getCellType(x, y) == CellType::PACMAN_SPAWN) {
                pacmanSpawnX = x;
                pacmanSpawnY = y;
            }
            else if (grid->getCellType(x, y) == CellType::GHOST_SPAWN) {
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
                // Reset grid position
                pacman->setGridPosition(pacmanSpawnX, pacmanSpawnY);
                pacman->setCurrentDirection(Direction::NONE);
                pacman->setNextDirection(Direction::NONE);
                pacman->setMoving(false);
                
                // Reset visual position
                float visualX = startX + (pacmanSpawnX * cellSize);
                float visualY = startY + (pacmanSpawnY * cellSize);
                pacman->setVisualPosition(visualX, visualY);
                pacman->setTargetPosition(visualX, visualY);
                
                // Update drawable component
                auto drawableComp = std::dynamic_pointer_cast<IDrawableComponent>(
                    _componentManager->getComponentByType(entity,
                        ComponentType::DRAWABLE));
                if (drawableComp) {
                    drawableComp->setPosition(visualX, visualY);
                    drawableComp->setVisibility(true);
                }
                
                // Update position component
                auto posComp = std::dynamic_pointer_cast<PositionComponent>(
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
    
    // Reset Ghost positions
    size_t ghostIndex = 0;
    std::vector<std::shared_ptr<IEntity>> ghostEntities;
    
    // Collect all ghost entities first
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
        if (ghostComp) {
            ghostEntities.push_back(entity);
        }
    }
    
    // Reset each ghost
    for (auto ghostEntity : ghostEntities) {
        auto ghost = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(ghostEntity,
                static_cast<ComponentType>(1002)));
        
        if (ghost && ghostIndex < ghostSpawnPositions.size()) {
            auto [spawnX, spawnY] = ghostSpawnPositions[ghostIndex++];
            
            // Get previous state for debugging
            GhostState prevState = ghost->getState();
            std::string prevStateStr = "";
            switch (prevState) {
                case GhostState::NORMAL: prevStateStr = "NORMAL"; break;
                case GhostState::SCARED: prevStateStr = "SCARED"; break;
                case GhostState::RETURNING: prevStateStr = "RETURNING"; break;
            }
            
            // Reset grid position
            ghost->setGridPosition(spawnX, spawnY);
            ghost->setCurrentDirection(Direction::NONE);
            
            // IMPORTANT: Always reset to NORMAL state regardless of previous state
            ghost->setState(GhostState::NORMAL);
            ghost->setMoving(false);
            
            // Print debug when resetting from any state other than NORMAL
            if (prevState != GhostState::NORMAL) {
                std::cout << "Ghost " << ghost->getName() << " reset from " << prevStateStr 
                          << " state to NORMAL" << std::endl;
            }
            
            // Reset visual position
            float visualX = startX + (spawnX * cellSize);
            float visualY = startY + (spawnY * cellSize);
            ghost->setVisualPosition(visualX, visualY);
            ghost->setTargetPosition(visualX, visualY);
            
            // Update drawable component with correct sprite based on ghost type
            auto drawableComp = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(ghostEntity,
                    ComponentType::DRAWABLE));
            
            if (drawableComp) {
                // First store old path for debug
                std::string oldPath = drawableComp->getPath();
                bool wasScared = (oldPath.find("scared") != std::string::npos) || 
                                 (drawableComp->getColor() == Color::BLUE);
                
                // CRITICAL FIX: Completely reset the drawable properties before applying new ones
                drawableComp->setPath("");
                drawableComp->setFont("");
                drawableComp->setText("");
                drawableComp->setVisibility(true);
                drawableComp->setScale(1.0f);
                drawableComp->setRotation(0.0f);
                drawableComp->setColor(Color::WHITE); // Reset to default color
                
                // Set position first to ensure it's preserved across asset changes
                drawableComp->setPosition(visualX, visualY);
                
                // Reset to appropriate ghost sprite
                std::string ghostAssetKey;
                Color ghostColor = Color::WHITE;
                
                // IMPROVED: Use direct asset lookup by exact type key for more reliability
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
                
                // IMPROVED: Look up asset directly from the map for each reset
                auto assetIter = _assets.find(ghostAssetKey);
                if (assetIter != _assets.end()) {
                    // Apply asset directly
                    DrawableComponent freshCopy = assetIter->second;
                    
                    // Copy the fresh asset properties directly
                    drawableComp->setPath(freshCopy.getPath());
                    drawableComp->setDimensions(freshCopy.getWidth(), freshCopy.getHeight());
                    drawableComp->setAsCharacter(freshCopy.getCharacter());
                    
                    // Debug output
                    if (prevState != GhostState::NORMAL || wasScared) {
                        std::cout << "Ghost sprite direct asset reset from " << oldPath << " to " 
                                  << drawableComp->getPath() << std::endl;
                    }
                } else {
                    // Fallback with hardcoded paths
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
                            fallbackPath = "assets/pacman/ghost_blue.png"; // Renamed from ghost_cyan.png for clarity
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
                    
                    // Debug output for fallback path
                    if (prevState != GhostState::NORMAL || wasScared) {
                        std::cout << "Ghost sprite fallback path reset from " << oldPath << " to " 
                                  << fallbackPath << std::endl;
                    }
                }
                
                // Apply color AFTER asset assignment to ensure it's not overwritten
                drawableComp->setColor(ghostColor);
                
                // Make sure visibility and position are set after asset assignment
                drawableComp->setVisibility(true);
                drawableComp->setPosition(visualX, visualY);
                
                // Verify sprite is not still a frightened sprite after reset
                std::string newPath = drawableComp->getPath();
                if (newPath.find("scared") != std::string::npos ||
                    newPath.find("blue") != std::string::npos ||
                    drawableComp->getColor() == Color::BLUE) {
                    
                    std::cout << "ERROR: Ghost still appears frightened after reset! "
                              << "Current path: " << newPath
                              << ", Color: " << static_cast<int>(drawableComp->getColor()) << std::endl;
                    
                    // Force direct texture override
                    switch (ghost->getGhostType()) {
                        case GhostType::RED:
                            drawableComp->setAsTexture("assets/pacman/ghost_red.png", 32, 32);
                            drawableComp->setColor(Color::RED);
                            break;
                        case GhostType::PINK:
                            drawableComp->setAsTexture("assets/pacman/ghost_pink.png", 32, 32);
                            drawableComp->setColor(Color::MAGENTA);
                            break;
                        case GhostType::BLUE:
                            drawableComp->setAsTexture("assets/pacman/ghost_blue.png", 32, 32);
                            drawableComp->setColor(Color::CYAN);
                            break;
                        case GhostType::ORANGE:
                            drawableComp->setAsTexture("assets/pacman/ghost_orange.png", 32, 32);
                            drawableComp->setColor(Color::YELLOW);
                            break;
                    }
                    
                    drawableComp->setVisibility(true);
                    drawableComp->setPosition(visualX, visualY);
                    
                    std::cout << "Applied emergency texture override to " 
                              << drawableComp->getPath() << std::endl;
                }
            }
            
            // Update position component
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
