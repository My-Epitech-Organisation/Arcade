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

void GameStateManager::reloadCurrentMap() {
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

    // Use more efficient handling of food items
    std::vector<std::pair<std::shared_ptr<IEntity>, std::shared_ptr<FoodComponent>>> foodEntities;
    foodEntities.reserve(100); // Pre-allocate space for better performance

    // First pass - collect food entities to avoid repeated entity searches
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1003)));

        if (foodComp) {
            foodEntities.push_back({entity, foodComp});
        }
    }

    // Second pass - process collected food entities
    for (auto& [entity, foodComp] : foodEntities) {
        foodComp->setEaten(false);

        auto spriteComp = std::dynamic_pointer_cast<IDrawableComponent>(
            _componentManager->getComponentByType(entity,
                ComponentType::DRAWABLE));

        if (spriteComp) {
            spriteComp->setVisibility(true); // Ensure visibility is on
            if (foodComp->getFoodType() == FoodType::NORMAL_DOT) {
                auto foodAsset = getDrawableAsset("map.food");
                if (foodAsset) {
                    *spriteComp = *foodAsset;
                } else {
                    spriteComp->setAsTexture
                        ("assets/pacman/dot.png", 32, 32);
                    spriteComp->setAsCharacter('.');
                }
            } else {
                auto powerAsset = getDrawableAsset("map.power_pellet");
                if (powerAsset) {
                    *spriteComp = *powerAsset;
                } else {
                    spriteComp->setAsTexture
                        ("assets/pacman/power_pellet.png", 32, 32);
                    spriteComp->setAsCharacter('U');
                }
            }
        }
    }

    size_t totalFoodCount = grid->getTotalFoodCount();
    grid->setFoodCount(totalFoodCount);
    
    // Cache pacman entity lookup
    static std::shared_ptr<IEntity> pacmanEntity = nullptr;
    if (!pacmanEntity) {
        for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
            if (name == "Pacman") {
                pacmanEntity = entity;
                break;
            }
        }
    }
    
    auto pacman = std::dynamic_pointer_cast<PacmanComponent>(
        _componentManager->getComponentByType(pacmanEntity,
            static_cast<ComponentType>(1001)));

    auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));

    float startX = gridPosComp ? gridPosComp->x : 0;
    float startY = gridPosComp ? gridPosComp->y : 0;
    float cellSize = grid->getCellSize();

    for (size_t y = 0; y < grid->getHeight(); y++) {
        for (size_t x = 0; x < grid->getWidth(); x++) {
            if (grid->getCellType(x, y) == CellType::PACMAN_SPAWN) {
                pacman->setGridPosition(x, y);
                pacman->setCurrentDirection(Direction::NONE);
                pacman->setNextDirection(Direction::NONE);

                auto pacmanPosComp
                = std::dynamic_pointer_cast<PositionComponent>(
                    _componentManager->getComponentByType(pacmanEntity,
                        ComponentType::POSITION));

                if (pacmanPosComp) {
                    pacmanPosComp->x = startX + (x * cellSize);
                    pacmanPosComp->y = startY + (y * cellSize);
                }
                auto pacmanDrawable
                = std::dynamic_pointer_cast<IDrawableComponent>(
                    _componentManager->getComponentByType(pacmanEntity,
                        ComponentType::DRAWABLE));
                if (pacmanDrawable) {
                    pacmanDrawable->setPosition(startX + (x * cellSize),
                        startY + (y * cellSize));
                }
            }

            if (grid->getCellType(x, y) == CellType::GHOST_SPAWN) {
                for (const auto& [entity, name]
                    : _entityManager->getEntitiesMap()) {
                    auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
                        _componentManager->getComponentByType(entity,
                            static_cast<ComponentType>(1002)));
                    if (ghostComp) {
                        auto ghostDrawable
                            = std::dynamic_pointer_cast<IDrawableComponent>(
                            _componentManager->getComponentByType(entity,
                                ComponentType::DRAWABLE));
                        if (ghostDrawable) {
                            std::string ghostAssetKey;
                            switch (ghostComp->getGhostType()) {
                                case GhostType::RED:
                                    ghostAssetKey = "ghosts.red";
                                    break;
                                case GhostType::PINK:
                                    ghostAssetKey = "ghosts.pink";
                                    break;
                                case GhostType::BLUE:
                                    ghostAssetKey = "ghosts.blue";
                                    break;
                                case GhostType::ORANGE:
                                    ghostAssetKey = "ghosts.orange";
                                    break;
                            }
                            auto ghostAsset
                                = getDrawableAsset(ghostAssetKey);
                            if (ghostAsset) {
                                *ghostDrawable = *ghostAsset;
                            } else {
                                switch (ghostComp->getGhostType()) {
                                    case GhostType::RED:
                                        ghostDrawable->setAsTexture
                                            ("assets/pacman/ghost_red.png",
                                                32, 32);
                                        ghostDrawable->setAsCharacter('r');
                                        break;
                                    case GhostType::PINK:
                                        ghostDrawable->setAsTexture
                                            ("assets/pacman/ghost_pink.png",
                                                32, 32);
                                        ghostDrawable->setAsCharacter('i');
                                        break;
                                    case GhostType::BLUE:
                                        ghostDrawable->setAsTexture
                                            ("assets/pacman/ghost_cyan.png",
                                                32, 32);
                                        ghostDrawable->setAsCharacter('c');
                                        break;
                                    case GhostType::ORANGE:
                                        ghostDrawable->setAsTexture
                                            ("assets/pacman/ghost_orange.png",
                                                32, 32);
                                        ghostDrawable->setAsCharacter('o');
                                        break;
                                }
                            }

                            // Ensure visibility
                            ghostDrawable->setVisibility(true);
                        }
                    }
                }
            }
        }
    }
    
    grid->setGameOver(false);
    grid->setGameWon(false);
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
            
            // Reset grid position
            ghost->setGridPosition(spawnX, spawnY);
            ghost->setCurrentDirection(Direction::NONE);
            ghost->setState(GhostState::NORMAL);
            ghost->setMoving(false);
            
            // Reset visual position
            float visualX = startX + (spawnX * cellSize);
            float visualY = startY + (spawnY * cellSize);
            ghost->setVisualPosition(visualX, visualY);
            ghost->setTargetPosition(visualX, visualY);
            
            // Update drawable component with correct sprite
            auto drawableComp = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(ghostEntity,
                    ComponentType::DRAWABLE));
            if (drawableComp) {
                drawableComp->setPosition(visualX, visualY);
                
                // Reset to appropriate ghost sprite
                std::string ghostAssetKey;
                switch (ghost->getGhostType()) {
                    case GhostType::RED:
                        ghostAssetKey = "ghosts.red";
                        break;
                    case GhostType::PINK:
                        ghostAssetKey = "ghosts.pink";
                        break;
                    case GhostType::BLUE:
                        ghostAssetKey = "ghosts.blue";
                        break;
                    case GhostType::ORANGE:
                        ghostAssetKey = "ghosts.orange";
                        break;
                }
                
                auto ghostAsset = getDrawableAsset(ghostAssetKey);
                if (ghostAsset) {
                    *drawableComp = *ghostAsset;
                    drawableComp->setVisibility(true);
                    drawableComp->setPosition(visualX, visualY);
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
