// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Ghost Logic for Pacman implementation
*/

#include <chrono>
#include <ctime>
#include <memory>
#include <algorithm>
#include <iostream>
#include "Games/PacMan/System/GhostLogic.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"

namespace Arcade {
namespace PacMan {

GhostLogic::GhostLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
                       std::shared_ptr<Arcade::IEntityManager> entityManager,
                       const std::map<std::string, DrawableComponent>& assets)
    : _componentManager(componentManager), _entityManager(entityManager), _assets(assets) {
}

std::shared_ptr<IDrawableComponent>
GhostLogic::getDrawableAsset(const std::string& key) const {
    auto it = _assets.find(key);
    if (it != _assets.end()) {
        auto component = std::make_shared<DrawableComponent>(it->second);
        return component;
    }
    return nullptr;
}

void GhostLogic::moveGhost(std::shared_ptr<GhostComponent> ghostComp,
                          std::shared_ptr<IEntity> entity,
                          std::shared_ptr<GridComponent> grid,
                          std::shared_ptr<PacmanComponent> pacman) {
    if (!ghostComp->canMove() || !grid || !pacman || ghostComp->isMoving())
        return;

    size_t pacmanX = pacman->getGridX();
    size_t pacmanY = pacman->getGridY();
    Direction pacmanDir = pacman->getCurrentDirection();
    size_t targetX = 0;
    size_t targetY = 0;

    if (ghostComp->getState() == GhostState::SCARED) {
        unsigned int seed = static_cast<unsigned int>(
            std::time(nullptr)
            + reinterpret_cast<std::uintptr_t>(entity.get()) +
            static_cast<unsigned int>(std::chrono::duration_cast
                <std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now
                ().time_since_epoch()).count() % 10000));
        Direction dirs[] = {Direction::UP, Direction::DOWN,
            Direction::LEFT, Direction::RIGHT};
        ghostComp->setCurrentDirection(dirs[rand_r(&seed) % 4]);
    } else if (ghostComp->getState() == GhostState::RETURNING) {
        // Find ghost spawn location
        bool foundSpawn = false;
        for (size_t y = 0; y < grid->getHeight(); y++) {
            for (size_t x = 0; x < grid->getWidth(); x++) {
                if (grid->getCellType(x, y) == CellType::GHOST_SPAWN) {
                    targetX = x;
                    targetY = y;
                    foundSpawn = true;
                    break;
                }
            }
            if (foundSpawn) break;
        }
        
        // Debug output always when in RETURNING state
        std::cout << "Ghost " << ghostComp->getName() << " RETURNING - position: ("
                  << ghostComp->getGridX() << "," << ghostComp->getGridY() 
                  << ") - spawn at: (" << targetX << "," << targetY << ")" << std::endl;
        
        // Always update the eyes appearance while returning
        auto ghostSprite = std::dynamic_pointer_cast<IDrawableComponent>(
            _componentManager->getComponentByType(entity,
                ComponentType::DRAWABLE));
                
        if (ghostSprite) {
            // Remove static variable - it causes issues with multiple ghosts
            // Each ghost should have its own state
            std::string currentPath = ghostSprite->getPath();
            
            // Only update sprite if it's not already using eyes
            if (currentPath.find("eyes") == std::string::npos) {
                // CHANGE: Use our specialized function for setting eyes appearance
                setGhostToEyesAppearance(entity, ghostComp);
            }
        }

        // Check if ghost has reached spawn - CRITICAL FIX: Use exact match
        bool hasReachedSpawn = (ghostComp->getGridX() == targetX && ghostComp->getGridY() == targetY);
        
        // Also verify with visual position for extra safety
        if (!hasReachedSpawn) {
            // Check if visually very close to spawn (within 2 pixels)
            float gridPosX = 0, gridPosY = 0;
            auto gridEntity = findGridEntity();
            if (gridEntity) {
                auto gridPos = std::dynamic_pointer_cast<PositionComponent>(
                    _componentManager->getComponentByType(gridEntity, ComponentType::POSITION));
                if (gridPos) {
                    gridPosX = gridPos->x;
                    gridPosY = gridPos->y;
                    
                    float cellSize = grid->getCellSize();
                    float spawnVisualX = gridPosX + (targetX * cellSize);
                    float spawnVisualY = gridPosY + (targetY * cellSize);
                    
                    float dx = std::abs(ghostComp->getVisualX() - spawnVisualX);
                    float dy = std::abs(ghostComp->getVisualY() - spawnVisualY);
                    
                    // If within 2 pixels of spawn, consider it arrived
                    if (dx < 2.0f && dy < 2.0f) {
                        hasReachedSpawn = true;
                        std::cout << "Ghost " << ghostComp->getName() << " visually at spawn!" << std::endl;
                    }
                }
            }
        }

        if (hasReachedSpawn) {
            std::cout << "SPAWN REACHED! Ghost " << ghostComp->getName() 
                      << " at position (" << ghostComp->getGridX() << "," << ghostComp->getGridY() 
                      << "), resetting state and appearance..." << std::endl;
            
            // Force position to exact spawn coordinates
            ghostComp->setGridPosition(targetX, targetY);
            
            // First reset state - IMPORTANT: do this before updating sprite
            ghostComp->setState(GhostState::NORMAL);
            ghostComp->setCurrentDirection(Direction::NONE);

            // CRITICAL FIX: Force sync all positions to avoid any mismatch
            auto gridEntity = findGridEntity();
            if (gridEntity) {
                auto gridPos = std::dynamic_pointer_cast<PositionComponent>(
                    _componentManager->getComponentByType(gridEntity, ComponentType::POSITION));
                if (gridPos) {
                    float startX = gridPos->x;
                    float startY = gridPos->y;
                    float cellSize = grid->getCellSize();
                    
                    float visualX = startX + (targetX * cellSize);
                    float visualY = startY + (targetY * cellSize);
                    
                    // Sync all positions
                    ghostComp->setVisualPosition(visualX, visualY);
                    ghostComp->setTargetPosition(visualX, visualY);
                    
                    auto posComp = std::dynamic_pointer_cast<PositionComponent>(
                        _componentManager->getComponentByType(entity, ComponentType::POSITION));
                    if (posComp) {
                        posComp->x = visualX;
                        posComp->y = visualY;
                    }
                }
            }

            // Reset the ghost's appearance with normal sprite - use our improved method
            try {
                resetGhostAppearance(entity, ghostComp);
                std::cout << "Ghost appearance reset SUCCESS" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "ERROR resetting ghost appearance: " << e.what() << std::endl;
                
                // CRITICAL FIX: Force a basic reset if the normal reset fails
                auto ghostSprite = std::dynamic_pointer_cast<IDrawableComponent>(
                    _componentManager->getComponentByType(entity, ComponentType::DRAWABLE));
                    
                if (ghostSprite) {
                    std::string ghostPath;
                    Color ghostColor;
                    
                    switch (ghostComp->getGhostType()) {
                        case GhostType::RED:
                            ghostPath = "assets/pacman/ghost_red.png";
                            ghostColor = Color::RED;
                            break;
                        case GhostType::PINK:
                            ghostPath = "assets/pacman/ghost_pink.png";
                            ghostColor = Color::MAGENTA;
                            break;
                        case GhostType::BLUE:
                            ghostPath = "assets/pacman/ghost_cyan.png";
                            ghostColor = Color::CYAN;
                            break;
                        case GhostType::ORANGE:
                            ghostPath = "assets/pacman/ghost_orange.png";
                            ghostColor = Color::YELLOW;
                            break;
                    }
                    
                    ghostSprite->setAsTexture(ghostPath, 32, 32);
                    ghostSprite->setColor(ghostColor);
                    ghostSprite->setPosition(ghostComp->getVisualX(), ghostComp->getVisualY());
                    std::cout << "Fallback appearance reset applied" << std::endl;
                }
            }
            
            // Ensure the ghost doesn't get stuck in the spawn
            ghostComp->setCanMove(true);
            ghostComp->resetMovementTimer();
            ghostComp->setMoving(false);
            
            // Give the ghost a random direction to exit spawn
            unsigned int seed = static_cast<unsigned int>(std::time(nullptr));
            Direction exitDirs[] = {Direction::UP, Direction::LEFT, Direction::RIGHT};
            ghostComp->setCurrentDirection(exitDirs[rand_r(&seed) % 3]); // Don't use DOWN to exit
            
            // Don't continue with the normal movement logic - we've handled it here
            return;
        }
        
        // IMPORTANT FIX: If ghost has been stuck for too long, teleport it to spawn
        static std::unordered_map<GhostComponent*, float> stuckTimers;
        if (stuckTimers.find(ghostComp.get()) == stuckTimers.end()) {
            stuckTimers[ghostComp.get()] = 0.0f;
        }
        stuckTimers[ghostComp.get()] += 0.1f; // Increment each frame
        
        // Debug output - only log when position changes to reduce spam
        static std::unordered_map<GhostComponent*, std::pair<size_t, size_t>> lastPositions;
        auto ghostKey = ghostComp.get();
        if (lastPositions.find(ghostKey) == lastPositions.end() || 
            lastPositions[ghostKey] != std::make_pair(ghostComp->getGridX(), ghostComp->getGridY())) {
            
            std::cout << "Ghost " << ghostComp->getName() 
                      << " RETURNING to spawn at (" << targetX << "," << targetY 
                      << ") - current position (" << ghostComp->getGridX() << "," 
                      << ghostComp->getGridY() << ") - stuck timer: "
                      << stuckTimers[ghostKey] << "s" << std::endl;
                      
            lastPositions[ghostKey] = {ghostComp->getGridX(), ghostComp->getGridY()};
        }
        
        if (stuckTimers[ghostComp.get()] > 10.0f) {
            std::cout << "Ghost " << ghostComp->getName() << " was stuck for "
                      << stuckTimers[ghostComp.get()] << "s. Teleporting to spawn!" << std::endl;
            
            // Reset stuck timer
            stuckTimers[ghostComp.get()] = 0.0f;
            
            // Teleport to spawn
            ghostComp->setGridPosition(targetX, targetY);
            
            // Find the grid entity for position reference
            std::shared_ptr<IEntity> gridEntity = nullptr;
            for (const auto& [e, name] : _entityManager->getEntitiesMap()) {
                if (name == "Grid") {
                    gridEntity = e;
                    break;
                }
            }
            
            if (gridEntity) {
                auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
                    _componentManager->getComponentByType(gridEntity,
                        ComponentType::POSITION));
                        
                if (gridPosComp) {
                    float startX = gridPosComp->x;
                    float startY = gridPosComp->y;
                    float cellSize = grid->getCellSize();
                    
                    // Update visual position
                    float visualX = startX + (targetX * cellSize);
                    float visualY = startY + (targetY * cellSize);
                    ghostComp->setVisualPosition(visualX, visualY);
                    ghostComp->setTargetPosition(visualX, visualY);
                    
                    // Update position component
                    auto posComp = std::dynamic_pointer_cast<PositionComponent>(
                        _componentManager->getComponentByType(entity,
                            ComponentType::POSITION));
                    if (posComp) {
                        posComp->x = visualX;
                        posComp->y = visualY;
                    }
                    
                    // Update sprite position
                    auto ghostSprite = std::dynamic_pointer_cast<IDrawableComponent>(
                        _componentManager->getComponentByType(entity,
                            ComponentType::DRAWABLE));
                    if (ghostSprite) {
                        ghostSprite->setPosition(visualX, visualY);
                    }
                }
            }
            
            // Proceed with state reset
            std::cout << "Ghost " << ghostComp->getName() << " teleported to spawn! Resetting state..." << std::endl;
            
            // Reset state BEFORE updating appearance
            ghostComp->setState(GhostState::NORMAL);
            ghostComp->setCurrentDirection(Direction::NONE);
            
            // Use common method to reset appearance
            resetGhostAppearance(entity, ghostComp);
            
            // Ensure the ghost doesn't get stuck
            ghostComp->setCanMove(true);
            ghostComp->resetMovementTimer();
            ghostComp->setMoving(false);
            
            // Don't continue with the normal movement logic
            return;
        }
        
        // Set target for pathfinding - use A* or similar pathfinding to spawn
        ghostComp->setTarget(targetX, targetY);
        
        // IMPORTANT: Force direct path to spawn - don't respect normal movement rules
        // Calculate the best direction to reach the spawn point
        size_t ghostX = ghostComp->getGridX();
        size_t ghostY = ghostComp->getGridY();
        
        // Calculate differences
        int dx = static_cast<int>(targetX) - static_cast<int>(ghostX);
        int dy = static_cast<int>(targetY) - static_cast<int>(ghostY);
        
        // Prioritize the axis with the larger difference
        Direction primaryDir = Direction::NONE;
        Direction secondaryDir = Direction::NONE;
        
        if (std::abs(dx) > std::abs(dy)) {
            // Horizontal movement is prioritized
            primaryDir = (dx > 0) ? Direction::RIGHT : Direction::LEFT;
            secondaryDir = (dy > 0) ? Direction::DOWN : Direction::UP;
        } else {
            // Vertical movement is prioritized
            primaryDir = (dy > 0) ? Direction::DOWN : Direction::UP;
            secondaryDir = (dx > 0) ? Direction::RIGHT : Direction::LEFT;
        }
        
        // First try the primary direction
        size_t newX = ghostX;
        size_t newY = ghostY;
        bool canMove = false;
        
        // Try primary direction first
        switch (primaryDir) {
            case Direction::UP:
                if (ghostY > 0 && grid->getCellType(ghostX, ghostY - 1) != CellType::WALL) {
                    newY--;
                    canMove = true;
                }
                break;
            case Direction::DOWN:
                if (ghostY < grid->getHeight() - 1 && grid->getCellType(ghostX, ghostY + 1) != CellType::WALL) {
                    newY++;
                    canMove = true;
                }
                break;
            case Direction::LEFT:
                if (ghostX > 0 && grid->getCellType(ghostX - 1, ghostY) != CellType::WALL) {
                    newX--;
                    canMove = true;
                }
                break;
            case Direction::RIGHT:
                if (ghostX < grid->getWidth() - 1 && grid->getCellType(ghostX + 1, ghostY) != CellType::WALL) {
                    newX++;
                    canMove = true;
                }
                break;
            default:
                break;
        }
        
        // If we can't move in the primary direction, try the secondary
        if (!canMove && secondaryDir != Direction::NONE) {
            newX = ghostX;
            newY = ghostY;
            
            switch (secondaryDir) {
                case Direction::UP:
                    if (ghostY > 0 && grid->getCellType(ghostX, ghostY - 1) != CellType::WALL) {
                        newY--;
                        canMove = true;
                        primaryDir = Direction::UP;
                    }
                    break;
                case Direction::DOWN:
                    if (ghostY < grid->getHeight() - 1 && grid->getCellType(ghostX, ghostY + 1) != CellType::WALL) {
                        newY++;
                        canMove = true;
                        primaryDir = Direction::DOWN;
                    }
                    break;
                case Direction::LEFT:
                    if (ghostX > 0 && grid->getCellType(ghostX - 1, ghostY) != CellType::WALL) {
                        newX--;
                        canMove = true;
                        primaryDir = Direction::LEFT;
                    }
                    break;
                case Direction::RIGHT:
                    if (ghostX < grid->getWidth() - 1 && grid->getCellType(ghostX + 1, ghostY) != CellType::WALL) {
                        newX++;
                        canMove = true;
                        primaryDir = Direction::RIGHT;
                    }
                    break;
                default:
                    break;
            }
        }
        
        // If we still can't move, try any valid direction
        if (!canMove) {
            // Try all four directions in order
            Direction dirs[] = {Direction::UP, Direction::RIGHT, Direction::DOWN, Direction::LEFT};
            for (Direction dir : dirs) {
                newX = ghostX;
                newY = ghostY;
                
                switch (dir) {
                    case Direction::UP:
                        if (ghostY > 0 && grid->getCellType(ghostX, ghostY - 1) != CellType::WALL) {
                            newY--;
                            canMove = true;
                            primaryDir = Direction::UP;
                        }
                        break;
                    case Direction::DOWN:
                        if (ghostY < grid->getHeight() - 1 && grid->getCellType(ghostX, ghostY + 1) != CellType::WALL) {
                            newY++;
                            canMove = true;
                            primaryDir = Direction::DOWN;
                        }
                        break;
                    case Direction::LEFT:
                        if (ghostX > 0 && grid->getCellType(ghostX - 1, ghostY) != CellType::WALL) {
                            newX--;
                            canMove = true;
                            primaryDir = Direction::LEFT;
                        }
                        break;
                    case Direction::RIGHT:
                        if (ghostX < grid->getWidth() - 1 && grid->getCellType(ghostX + 1, ghostY) != CellType::WALL) {
                            newX++;
                            canMove = true;
                            primaryDir = Direction::RIGHT;
                        }
                        break;
                    default:
                        break;
                }
                
                if (canMove) break;
            }
        }
        
        // If the ghost can move, update its position and sprite
        if (canMove) {
            ghostComp->setCurrentDirection(primaryDir);
            ghostComp->setGridPosition(newX, newY);
            
            // Update visual position for smooth movement
            // Find the grid entity for position reference
            std::shared_ptr<IEntity> gridEntity = nullptr;
            for (const auto& [e, name] : _entityManager->getEntitiesMap()) {
                if (name == "Grid") {
                    gridEntity = e;
                    break;
                }
            }
            
            auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
                _componentManager->getComponentByType(gridEntity,
                    ComponentType::POSITION));
            float startX = gridPosComp ? gridPosComp->x : 0;
            float startY = gridPosComp ? gridPosComp->y : 0;
            float cellSize = grid->getCellSize();
            
            // Get current visual position or initialize it
            float currentX = ghostComp->getVisualX();
            float currentY = ghostComp->getVisualY();
            if (currentX == 0 && currentY == 0) {
                currentX = startX + (ghostX * cellSize);
                currentY = startY + (ghostY * cellSize);
                ghostComp->setVisualPosition(currentX, currentY);
            }
            
            // Set target position for visual interpolation
            float targetVisualX = startX + (newX * cellSize);
            float targetVisualY = startY + (newY * cellSize);
            
            // Start animation
            ghostComp->setTargetPosition(targetVisualX, targetVisualY);
            ghostComp->setMoving(true);
            
            // Update PositionComponent for accurate collision detection
            auto posComp = std::dynamic_pointer_cast<PositionComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::POSITION));
            if (posComp) {
                posComp->x = targetVisualX;
                posComp->y = targetVisualY;
            }
            
            ghostComp->setCanMove(false);
            ghostComp->resetMovementTimer();
            
            // Reset stuck counter since we moved successfully
            stuckTimers[ghostComp.get()] = 0.0f;
            
            // Early return since we don't want normal movement logic
            return;
        }
    } else {
        if (ghostComp->getMode() == GhostMode::SCATTER) {
            targetX = ghostComp->getHomeCornerX();
            targetY = ghostComp->getHomeCornerY();
        } else {
            switch (ghostComp->getGhostType()) {
                case GhostType::RED:
                    targetX = pacmanX;
                    targetY = pacmanY;
                    break;
                case GhostType::PINK:
                    targetX = pacmanX;
                    targetY = pacmanY;
                    switch (pacmanDir) {
                        case Direction::UP:
                            targetY = (targetY >= 4) ? targetY - 4 : 0;
                            targetX = (targetX >= 4) ? targetX - 4 : 0;
                            break;
                        case Direction::DOWN:
                            targetY = std::min(targetY + 4,
                                grid->getHeight() - 1);
                            break;
                        case Direction::LEFT:
                            targetX = (targetX >= 4) ? targetX - 4 : 0;
                            break;
                        case Direction::RIGHT:
                            targetX = std::min(targetX + 4,
                                grid->getWidth() - 1);
                            break;
                        default:
                            break;
                    }
                    break;
                case GhostType::BLUE: {
                    std::shared_ptr<GhostComponent> blinkyComp = nullptr;
                    for (const auto& [e, name] :
                        _entityManager->getEntitiesMap()) {
                        auto ghost = std::dynamic_pointer_cast<GhostComponent>(
                            _componentManager->getComponentByType(e,
                                static_cast<ComponentType>(1002)));
                        if (ghost && ghost->getGhostType() == GhostType::RED) {
                            blinkyComp = ghost;
                            break;
                        }
                    }
                    if (blinkyComp) {
                        size_t intermediateX = pacmanX;
                        size_t intermediateY = pacmanY;
                        switch (pacmanDir) {
                            case Direction::UP:
                                if (intermediateY >= 2) intermediateY -= 2;
                                else intermediateY = 0;
                                break;
                            case Direction::DOWN:
                                intermediateY = std::min(intermediateY + 2,
                                    grid->getHeight() - 1);
                                break;
                            case Direction::LEFT:
                                if (intermediateX >= 2) intermediateX -= 2;
                                else intermediateX = 0;
                                break;
                            case Direction::RIGHT:
                                intermediateX = std::min(intermediateX + 2,
                                    grid->getWidth() - 1);
                                break;
                            default:
                                break;
                        }

                        int vectorX = static_cast<int>(intermediateX)
                            - static_cast<int>(blinkyComp->getGridX());
                        int vectorY = static_cast<int>(intermediateY)
                            - static_cast<int>(blinkyComp->getGridY());

                        int targetXInt = static_cast<int>(intermediateX)
                            + vectorX;
                        int targetYInt = static_cast<int>(intermediateY)
                            + vectorY;

                        targetX = std::clamp(targetXInt, 0,
                            static_cast<int>(grid->getWidth() - 1));
                        targetY = std::clamp(targetYInt, 0,
                            static_cast<int>(grid->getHeight() - 1));
                    } else {
                        targetX = pacmanX;
                        targetY = pacmanY;
                    }
                    break;
                }
                case GhostType::ORANGE:
                {
                    size_t distance = std::abs(static_cast<int>
                        (ghostComp->getGridX()) - static_cast<int>(pacmanX)) +
                        std::abs(static_cast<int>
                        (ghostComp->getGridY()) - static_cast<int>(pacmanY));
                    if (distance > 8) {
                        targetX = pacmanX;
                        targetY = pacmanY;
                    } else {
                        targetX = ghostComp->getHomeCornerX();
                        targetY = ghostComp->getHomeCornerY();
                    }
                    break;
                }
            }
        }
        ghostComp->setTarget(targetX, targetY);
    }


    Direction bestDirection = ghostComp->getCurrentDirection();
    if (ghostComp->getState() != GhostState::SCARED) {
        size_t bestDistance = SIZE_MAX;
        Direction possibleDirections[] = {Direction::UP, Direction::LEFT,
            Direction::DOWN, Direction::RIGHT};

        size_t ghostX = ghostComp->getGridX();
        size_t ghostY = ghostComp->getGridY();
        Direction oppositeDir = ghostComp->getOppositeDirection();

        for (Direction dir : possibleDirections) {
            if (dir == oppositeDir && ghostComp->getCurrentDirection()
                != Direction::NONE)
                continue;

            size_t newX = ghostX;
            size_t newY = ghostY;

            switch (dir) {
                case Direction::UP:
                    if (ghostY > 0) newY--;
                    else newY = grid->getHeight() - 1;
                    break;
                case Direction::DOWN:
                    if (ghostY < grid->getHeight() - 1) newY++;
                    else newY = 0;
                    break;
                case Direction::LEFT:
                    if (ghostX > 0) newX--;
                    else newX = grid->getWidth() - 1;
                    break;
                case Direction::RIGHT:
                    if (ghostX < grid->getWidth() - 1) newX++;
                    else newX = 0;
                    break;
                default:
                    continue;
            }

            if (grid->getCellType(newX, newY) == CellType::WALL)
                continue;

            size_t distance = std::abs(static_cast<int>(newX)
                - static_cast<int>(targetX)) +
                std::abs(static_cast<int>(newY)
                    - static_cast<int>(targetY));

            if (distance < bestDistance) {
                bestDistance = distance;
                bestDirection = dir;
            }
        }
    }

    ghostComp->setCurrentDirection(bestDirection);

    Direction dir = ghostComp->getCurrentDirection();
    if (dir == Direction::NONE)
        return;

    size_t x = ghostComp->getGridX();
    size_t y = ghostComp->getGridY();

    // Check if ghost can move in the requested direction
    size_t newX = x;
    size_t newY = y;

    switch (dir) {
        case Direction::UP:
            newY = (y == 0) ? grid->getHeight() - 1 : y - 1;
            break;
        case Direction::DOWN:
            newY = (y == grid->getHeight() - 1) ? 0 : y + 1;
            break;
        case Direction::LEFT:
            newX = (x == 0) ? grid->getWidth() - 1 : x - 1;
            break;
        case Direction::RIGHT:
            newX = (x == grid->getWidth() - 1) ? 0 : x + 1;
            break;
        default: return;
    }

    if (grid->getCellType(newX, newY) == CellType::WALL) {
        ghostComp->setCurrentDirection(Direction::NONE);
        return;
    }

    bool ghostCollision = false;
    for (const auto& [e, name] : _entityManager->getEntitiesMap()) {
        if (e != entity) {
            auto otherGhost = std::dynamic_pointer_cast<GhostComponent>(
                _componentManager->getComponentByType(e,
                    static_cast<ComponentType>(1002)));

            if (otherGhost && otherGhost->getGridX() == newX &&
                otherGhost->getGridY() == newY) {
                ghostCollision = true;
                break;
            }
        }
    }

    if (!ghostCollision) {
        // Update grid position for game logic
        ghostComp->setGridPosition(newX, newY);
        
        // Find the grid entity for position reference
        std::shared_ptr<IEntity> gridEntity = nullptr;
        for (const auto& [e, name] : _entityManager->getEntitiesMap()) {
            if (name == "Grid") {
                gridEntity = e;
                break;
            }
        }
        
        auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
            _componentManager->getComponentByType(gridEntity,
                ComponentType::POSITION));
        float startX = gridPosComp ? gridPosComp->x : 0;
        float startY = gridPosComp ? gridPosComp->y : 0;
        float cellSize = grid->getCellSize();
        
        // Get current visual position or initialize it
        float currentX = ghostComp->getVisualX();
        float currentY = ghostComp->getVisualY();
        if (currentX == 0 && currentY == 0) {
            currentX = startX + (x * cellSize);
            currentY = startY + (y * cellSize);
            ghostComp->setVisualPosition(currentX, currentY);
        }
        
        // Set target position for visual interpolation
        float targetX = startX + (newX * cellSize);
        float targetY = startY + (newY * cellSize);
        
        // Handle teleportation edge case
        if (ghostComp->getCurrentDirection() == Direction::LEFT && x == 0 && newX == grid->getWidth() - 1) {
            // Going left from leftmost column to rightmost column
            ghostComp->setVisualPosition(startX + ((grid->getWidth()) * cellSize), currentY);
        } else if (ghostComp->getCurrentDirection() == Direction::RIGHT && x == grid->getWidth() - 1 && newX == 0) {
            // Going right from rightmost column to leftmost column
            ghostComp->setVisualPosition(startX - cellSize, currentY);
        } else if (ghostComp->getCurrentDirection() == Direction::UP && y == 0 && newY == grid->getHeight() - 1) {
            // Going up from top row to bottom row
            ghostComp->setVisualPosition(currentX, startY + ((grid->getHeight()) * cellSize));
        } else if (ghostComp->getCurrentDirection() == Direction::DOWN && y == grid->getHeight() - 1 && newY == 0) {
            // Going down from bottom row to top row
            ghostComp->setVisualPosition(currentX, startY - cellSize);
        }
        
        // Start animation
        ghostComp->setTargetPosition(targetX, targetY);
        ghostComp->setMoving(true);
        
        // Update PositionComponent for accurate collision detection
        auto posComp = std::dynamic_pointer_cast<PositionComponent>(
            _componentManager->getComponentByType(entity,
                ComponentType::POSITION));
        if (posComp) {
            posComp->x = targetX;
            posComp->y = targetY;
        }
        
        ghostComp->setCanMove(false);
        ghostComp->resetMovementTimer();
    } else {
        ghostComp->setCurrentDirection(Direction::NONE);
    }
}

void GhostLogic::resetGhostAppearance(std::shared_ptr<IEntity> entity, std::shared_ptr<GhostComponent> ghostComp) {
    if (!entity || !ghostComp) {
        std::cerr << "Invalid entity or ghost component" << std::endl;
        return;
    }
        
    auto ghostSprite = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(entity,
            ComponentType::DRAWABLE));
            
    if (!ghostSprite) {
        std::cerr << "Failed to get drawable component for ghost" << std::endl;
        return;
    }
    
    // Save current appearance for debugging
    std::string prevPath = ghostSprite->getPath();
    char prevChar = ghostSprite->getCharacter();
    Color prevColor = ghostSprite->getColor();
    bool prevVisible = ghostSprite->isRenderable();
    
    // CRITICAL FIX: First use a new component rather than modifying the existing one
    auto replacementSprite = std::make_shared<DrawableComponent>();
    
    // Store visual position to preserve after reset
    float visualX = ghostComp->getVisualX();
    float visualY = ghostComp->getVisualY();
    
    // Configure the fresh component
    replacementSprite->setVisibility(true);
    replacementSprite->setScale(1.0f);
    replacementSprite->setRotation(0.0f);
    replacementSprite->setDimensions(32, 32);
    replacementSprite->setPosition(visualX, visualY);
    
    // Determine correct ghost type and color
    std::string ghostAssetKey;
    Color ghostColor;
    
    switch (ghostComp->getGhostType()) {
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
    
    // Try to get from the assets
    auto asset = _assets.find(ghostAssetKey);
    bool appliedSuccessfully = false;
    
    if (asset != _assets.end()) {
        // Apply texture from asset
        replacementSprite->setPath(asset->second.getPath());
        replacementSprite->setDimensions(asset->second.getWidth(), asset->second.getHeight());
        replacementSprite->setAsCharacter(asset->second.getCharacter());
        replacementSprite->setColor(ghostColor);
        appliedSuccessfully = true;
        
        std::cout << "Applied ghost texture from asset: " << ghostAssetKey << std::endl;
    } else {
        // Fall back to hardcoded paths
        switch (ghostComp->getGhostType()) {
            case GhostType::RED:
                replacementSprite->setAsTexture("assets/pacman/ghost_red.png", 32, 32);
                replacementSprite->setAsCharacter('r');
                replacementSprite->setColor(Color::RED);
                break;
            case GhostType::PINK:
                replacementSprite->setAsTexture("assets/pacman/ghost_pink.png", 32, 32);
                replacementSprite->setAsCharacter('i');
                replacementSprite->setColor(Color::MAGENTA);
                break;
            case GhostType::BLUE:
                replacementSprite->setAsTexture("assets/pacman/ghost_blue.png", 32, 32);
                replacementSprite->setAsCharacter('c');
                replacementSprite->setColor(Color::CYAN);
                break;
            case GhostType::ORANGE:
                replacementSprite->setAsTexture("assets/pacman/ghost_orange.png", 32, 32);
                replacementSprite->setAsCharacter('o');
                replacementSprite->setColor(Color::YELLOW);
                break;
        }
        appliedSuccessfully = true;
        
        std::cout << "Applied fallback ghost texture type: " << static_cast<int>(ghostComp->getGhostType()) << std::endl;
    }
    
    // CRITICAL FIX: Now register the replacement component instead of the old one
    if (appliedSuccessfully) {
        // First unregister the original component - use string "DRAWABLE" instead of ComponentType enum
        _componentManager->unregisterComponent(entity, "DRAWABLE");
        
        // Register the replacement component - ComponentManager will extract the type
        _componentManager->registerComponent(entity, replacementSprite);
        
        std::cout << "Ghost appearance fully replaced: Old=" << prevPath 
                  << "(" << prevChar << ", color=" << static_cast<int>(prevColor)
                  << ", visible=" << (prevVisible ? "yes" : "no") << ") -> New="
                  << replacementSprite->getPath() << "(" << replacementSprite->getCharacter()
                  << ", color=" << static_cast<int>(replacementSprite->getColor())
                  << ", visible=" << (replacementSprite->isRenderable() ? "yes" : "no") << ")" << std::endl;
                  
        // Verify appearance was actually changed
        auto verifySprite = std::dynamic_pointer_cast<IDrawableComponent>(
            _componentManager->getComponentByType(entity, ComponentType::DRAWABLE));
            
        if (!verifySprite || !verifySprite->isRenderable()) {
            std::cerr << "WARNING: Ghost sprite is still not visible after component replacement!" << std::endl;
            throw std::runtime_error("Failed to make ghost visible after reset");
        }
    }
}

// Add a new specialized method for setting ghost to "eyes" appearance
void GhostLogic::setGhostToEyesAppearance(std::shared_ptr<IEntity> entity, std::shared_ptr<GhostComponent> ghostComp) {
    if (!entity || !ghostComp) {
        std::cerr << "Invalid entity or ghost component in setGhostToEyesAppearance" << std::endl;
        return;
    }
        
    auto ghostSprite = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(entity, ComponentType::DRAWABLE));
            
    if (!ghostSprite) {
        std::cerr << "Failed to get drawable component for ghost" << std::endl;
        return;
    }
    
    // Save current properties for debugging
    std::string oldPath = ghostSprite->getPath();
    char oldChar = ghostSprite->getCharacter();
    Color oldColor = ghostSprite->getColor();
    
    // First reset ALL sprite properties to known values
    ghostSprite->setVisibility(true);
    ghostSprite->setScale(1.0f);
    ghostSprite->setRotation(0.0f);
    ghostSprite->setPath("");
    ghostSprite->setText("");
    ghostSprite->setFont("");
    
    // Store current position to preserve after reset
    float visualX = ghostComp->getVisualX();
    float visualY = ghostComp->getVisualY();
    
    // Try to get eyes asset
    auto eyesAsset = _assets.find("ghosts.eyes");
    if (eyesAsset != _assets.end()) {
        // Create a complete copy of the asset
        DrawableComponent tempSprite = eyesAsset->second;
        
        // Copy all properties from the eyes asset to the ghost sprite
        ghostSprite->setPath(tempSprite.getPath());
        ghostSprite->setDimensions(tempSprite.getWidth(), tempSprite.getHeight());
        ghostSprite->setAsCharacter(tempSprite.getCharacter());
        ghostSprite->setColor(Color::WHITE);
        
        std::cout << "Applied eyes sprite from asset to ghost " << ghostComp->getName()
                  << ". Old: " << oldPath << "(" << oldChar 
                  << ", color=" << static_cast<int>(oldColor) << ") New: "
                  << ghostSprite->getPath() << "(" << ghostSprite->getCharacter() 
                  << ", color=" << static_cast<int>(ghostSprite->getColor()) << ")" << std::endl;
    } else {
        // Fallback with hardcoded values
        ghostSprite->setAsTexture("assets/pacman/eyes.png", 32, 32);
        ghostSprite->setAsCharacter('e');
        ghostSprite->setColor(Color::WHITE);
        
        std::cout << "Applied fallback eyes sprite to ghost " << ghostComp->getName() 
                  << " (asset not found)" << std::endl;
    }
    
    // Ensure position is preserved
    ghostSprite->setPosition(visualX, visualY);
    
    // Verify the change worked
    if (ghostSprite->getPath().find("eyes") == std::string::npos && 
        ghostSprite->getCharacter() != 'e') {
        std::cerr << "WARNING: Failed to set eyes appearance! Current path: " 
                  << ghostSprite->getPath() << std::endl;
    }
}

void GhostLogic::updateGhostStates(float deltaTime) {
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
                
        if (!ghostComp)
            continue;
            
        GhostState previousState = ghostComp->getState();

        ghostComp->addGameTime(deltaTime);
        ghostComp->updateStateTimer(deltaTime);
        ghostComp->updateMovementTimer(deltaTime);
        ghostComp->updateModeTimer(deltaTime);
        ghostComp->updateReleaseTimer(deltaTime);

        if (previousState == GhostState::SCARED &&
            ghostComp->getState() == GhostState::NORMAL) {
            std::cout << "CRITICAL STATE CHANGE: Ghost " << ghostComp->getName() 
                      << " returning to normal state from scared state" << std::endl;
            
            // Force ghost to stop moving during transition
            ghostComp->setMoving(false);
            
            // Log before appearance to help debug
            auto ghostSprite = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));
            
            if (ghostSprite) {
                std::cout << "Ghost appearance BEFORE reset: Path=" << ghostSprite->getPath()
                          << ", Visible=" << (ghostSprite->isRenderable() ? "yes" : "no")
                          << ", Color=" << static_cast<int>(ghostSprite->getColor()) << std::endl;
            }
            
            // CRITICAL FIX: Use the same improved resetGhostAppearance method here
            try {
                resetGhostAppearance(entity, ghostComp);
                std::cout << "Ghost appearance successfully reset" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "ERROR resetting ghost appearance: " << e.what() << std::endl;
                // Emergency fallback
                emergencyGhostReset(entity, ghostComp);
            }
            
            // Verify appearance after reset
            if (ghostSprite) {
                std::cout << "Ghost appearance AFTER reset: Path=" << ghostSprite->getPath()
                          << ", Visible=" << (ghostSprite->isRenderable() ? "yes" : "no")
                          << ", Color=" << static_cast<int>(ghostSprite->getColor()) << std::endl;
                
                // Final verification - if still invisible, force visibility
                if (!ghostSprite->isRenderable()) {
                    std::cerr << "CRITICAL: Ghost still invisible after reset! Force enabling visibility" << std::endl;
                    ghostSprite->setVisibility(true);
                }
            }
        }
    }
}

// Add a new emergency fallback method for ghost reset
void GhostLogic::emergencyGhostReset(std::shared_ptr<IEntity> entity, 
                                    std::shared_ptr<GhostComponent> ghostComp) {
    std::cout << "EMERGENCY GHOST RESET for " << ghostComp->getName() << std::endl;
    
    auto ghostSprite = std::dynamic_pointer_cast<IDrawableComponent>(
        _componentManager->getComponentByType(entity, ComponentType::DRAWABLE));
        
    if (!ghostSprite) {
        std::cerr << "Cannot find drawable component for ghost" << std::endl;
        return;
    }
    
    // Force visibility and completely reset properties
    ghostSprite->setVisibility(true);
    ghostSprite->setScale(1.0f);
    ghostSprite->setRotation(0.0f);
    
    // Set color and texture based on ghost type
    switch (ghostComp->getGhostType()) {
        case GhostType::RED:
            ghostSprite->setAsTexture("assets/pacman/ghost_red.png", 32, 32);
            ghostSprite->setAsCharacter('r');
            ghostSprite->setColor(Color::RED);
            break;
        case GhostType::PINK:
            ghostSprite->setAsTexture("assets/pacman/ghost_pink.png", 32, 32);
            ghostSprite->setAsCharacter('i');
            ghostSprite->setColor(Color::MAGENTA);
            break;
        case GhostType::BLUE:
            ghostSprite->setAsTexture("assets/pacman/ghost_blue.png", 32, 32);
            ghostSprite->setAsCharacter('c');
            ghostSprite->setColor(Color::CYAN);
            break;
        case GhostType::ORANGE:
            ghostSprite->setAsTexture("assets/pacman/ghost_orange.png", 32, 32);
            ghostSprite->setAsCharacter('o');
            ghostSprite->setColor(Color::YELLOW);
            break;
    }
    
    // Ensure position is preserved
    ghostSprite->setPosition(ghostComp->getVisualX(), ghostComp->getVisualY());
    
    std::cout << "Emergency ghost reset complete - Ghost should now be visible" << std::endl;
}

std::shared_ptr<IEntity> GhostLogic::findGridEntity() {
    for (const auto& [entity, name] : _entityManager->getEntitiesMap()) {
        if (name == "Grid") {
            return entity;
        }
    }
    return nullptr;
}

}  // namespace PacMan
}  // namespace Arcade
