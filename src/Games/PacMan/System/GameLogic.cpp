// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game Logic System for Pacman
*/

#include <iostream>
#include <chrono>
#include "Games/PacMan/System/GameLogic.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/FoodComponent.hpp"

namespace Arcade {
namespace PacMan {

GameLogic::GameLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
                   std::shared_ptr<Arcade::IEntityManager> entityManager)
    : _componentManager(componentManager), _entityManager(entityManager) {
    _lastUpdateTime = std::chrono::high_resolution_clock::now();
}

void GameLogic::update() {
    // Find PacMan and Grid entities
    Arcade::Entity pacmanEntity = 0;
    Arcade::Entity gridEntity = 0;
    
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto pacmanComp = _componentManager->getComponentByType(entity, static_cast<ComponentType>(1001));
        if (pacmanComp) {
            pacmanEntity = entity;
        }
        
        auto gridComp = _componentManager->getComponentByType(entity, static_cast<ComponentType>(1000));
        if (gridComp) {
            gridEntity = entity;
        }
    }
    
    if (!pacmanEntity || !gridEntity) {
        return;
    }
    
    auto pacman = std::dynamic_pointer_cast<PacmanComponent>(
        _componentManager->getComponentByType(pacmanEntity, static_cast<ComponentType>(1001)));
    auto grid = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity, static_cast<ComponentType>(1000)));
    
    if (!pacman || !grid || grid->isGameOver()) {
        return;
    }
    
    float deltaTime = getDeltaTime();
    
    // Update timers
    pacman->updateMovementTimer(deltaTime);
    
    // Update all ghost timers
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity, static_cast<ComponentType>(1002)));
        
        if (ghostComp) {
            ghostComp->updateStateTimer(deltaTime);
            ghostComp->updateMovementTimer(deltaTime);
        }
    }
    
    // Try to move PacMan in the requested direction
    tryChangePacmanDirection(pacman, grid);
    
    // Move entities
    movePacman(pacman, grid);
    moveGhosts();
    
    // Check collisions
    checkCollisions();
    
    // Check win condition
    checkWinCondition(grid);
}

float GameLogic::getDeltaTime() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - _lastUpdateTime).count();
    _lastUpdateTime = currentTime;
    return deltaTime;
}

void GameLogic::tryChangePacmanDirection(std::shared_ptr<PacmanComponent> pacman, 
                                       std::shared_ptr<GridComponent> grid) {
    if (pacman->getNextDirection() != Direction::NONE) {
        Direction nextDir = pacman->getNextDirection();
        size_t x = pacman->getGridX();
        size_t y = pacman->getGridY();
        
        if (canMoveInDirection(nextDir, x, y, grid)) {
            pacman->setCurrentDirection(nextDir);
            pacman->setNextDirection(Direction::NONE);
        }
    }
}

bool GameLogic::canMoveInDirection(Direction dir, size_t x, size_t y,
                                 std::shared_ptr<GridComponent> grid) {
    if (!grid) return false;
    
    size_t nextX = x;
    size_t nextY = y;
    
    switch (dir) {
        case Direction::UP:
            if (y > 0) nextY--;
            else return false;
            break;
        case Direction::DOWN:
            if (y < grid->getHeight() - 1) nextY++;
            else return false;
            break;
        case Direction::LEFT:
            if (x > 0) nextX--;
            else return false;
            break;
        case Direction::RIGHT:
            if (x < grid->getWidth() - 1) nextX++;
            else return false;
            break;
        case Direction::NONE:
            return false;
    }
    
    return grid->getCellType(nextX, nextY) != CellType::WALL;
}

void GameLogic::movePacman(std::shared_ptr<PacmanComponent> pacman, 
                         std::shared_ptr<GridComponent> grid) {
    if (!pacman->canMove() || !grid) {
        return;
    }
    
    Direction dir = pacman->getCurrentDirection();
    if (dir == Direction::NONE) {
        return;
    }
    
    size_t x = pacman->getGridX();
    size_t y = pacman->getGridY();
    
    if (canMoveInDirection(dir, x, y, grid)) {
        // Update grid position
        switch (dir) {
            case Direction::UP:
                y--;
                break;
            case Direction::DOWN:
                y++;
                break;
            case Direction::LEFT:
                x--;
                break;
            case Direction::RIGHT:
                x++;
                break;
            default:
                break;
        }
        
        // Update pacman's position in the grid
        pacman->setGridPosition(x, y);
        
        // Update visual position
        Arcade::Entity pacmanEntity = 0;
        for (const auto& [entity, name] : _entityManager->getEntities()) {
            auto component = _componentManager->getComponentByType(entity, static_cast<ComponentType>(1001));
            if (component) {
                pacmanEntity = entity;
                break;
            }
        }
        
        if (pacmanEntity) {
            auto posComp = std::dynamic_pointer_cast<PositionComponent>(
                _componentManager->getComponentByType(pacmanEntity, ComponentType::POSITION));
            
            if (posComp) {
                posComp->x = static_cast<float>(x * 32); // assuming 32 pixel cells
                posComp->y = static_cast<float>(y * 32);
            }
        }
        
        // Set movement cooldown
        pacman->setCanMove(false);
    }
}

void GameLogic::moveGhosts() {
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity, static_cast<ComponentType>(1002)));
        
        if (!ghostComp || !ghostComp->canMove()) {
            continue;
        }
        
        // Find grid component
        Arcade::Entity gridEntity = 0;
        for (const auto& [e, n] : _entityManager->getEntities()) {
            auto gridComp = _componentManager->getComponentByType(e, static_cast<ComponentType>(1000));
            if (gridComp) {
                gridEntity = e;
                break;
            }
        }
        
        if (!gridEntity) {
            continue;
        }
        
        auto grid = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(gridEntity, static_cast<ComponentType>(1000)));
        
        if (!grid) {
            continue;
        }
        
        // Choose a new direction if needed or randomly
        if (ghostComp->getCurrentDirection() == Direction::NONE || rand() % 10 == 0) {
            ghostComp->chooseNextDirection();
        }
        
        Direction dir = ghostComp->getCurrentDirection();
        if (dir == Direction::NONE) {
            continue;
        }
        
        size_t x = ghostComp->getGridX();
        size_t y = ghostComp->getGridY();
        
        if (canMoveInDirection(dir, x, y, grid)) {
            // Update grid position
            switch (dir) {
                case Direction::UP:
                    y--;
                    break;
                case Direction::DOWN:
                    y++;
                    break;
                case Direction::LEFT:
                    x--;
                    break;
                case Direction::RIGHT:
                    x++;
                    break;
                default:
                    break;
            }
            
            // Update ghost's position in the grid
            ghostComp->setGridPosition(x, y);
            
            // Update visual position
            auto posComp = std::dynamic_pointer_cast<PositionComponent>(
                _componentManager->getComponentByType(entity, ComponentType::POSITION));
            
            if (posComp) {
                posComp->x = static_cast<float>(x * 32); // assuming 32 pixel cells
                posComp->y = static_cast<float>(y * 32);
            }
            
            // Set movement cooldown
            ghostComp->setCanMove(false);
        } else {
            // If can't move in current direction, choose a new one next update
            ghostComp->setCurrentDirection(Direction::NONE);
        }
    }
}

void GameLogic::checkCollisions() {
    // Find PacMan and Grid entities
    Arcade::Entity pacmanEntity = 0;
    Arcade::Entity gridEntity = 0;
    
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto pacmanComp = _componentManager->getComponentByType(entity, static_cast<ComponentType>(1001));
        if (pacmanComp) {
            pacmanEntity = entity;
        }
        
        auto gridComp = _componentManager->getComponentByType(entity, static_cast<ComponentType>(1000));
        if (gridComp) {
            gridEntity = entity;
        }
    }
    
    if (!pacmanEntity || !gridEntity) {
        return;
    }
    
    auto pacman = std::dynamic_pointer_cast<PacmanComponent>(
        _componentManager->getComponentByType(pacmanEntity, static_cast<ComponentType>(1001)));
    auto grid = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity, static_cast<ComponentType>(1000)));
    
    if (!pacman || !grid) {
        return;
    }
    
    // Check collisions with ghosts and food
    checkCollisionsWithGhosts(pacman, grid);
    checkCollisionsWithFood(pacman, grid);
}

void GameLogic::checkCollisionsWithGhosts(std::shared_ptr<PacmanComponent> pacman,
                                        std::shared_ptr<GridComponent> grid) {
    size_t pacmanX = pacman->getGridX();
    size_t pacmanY = pacman->getGridY();
    
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity, static_cast<ComponentType>(1002)));
        
        if (ghostComp) {
            size_t ghostX = ghostComp->getGridX();
            size_t ghostY = ghostComp->getGridY();
            
            if (pacmanX == ghostX && pacmanY == ghostY) {
                // Collision with ghost
                if (ghostComp->getState() == GhostState::SCARED) {
                    // PacMan eats the ghost
                    pacman->addScore(200);
                    
                    // Reset ghost position to spawn
                    for (size_t y = 0; y < grid->getHeight(); y++) {
                        for (size_t x = 0; x < grid->getWidth(); x++) {
                            if (grid->getCellType(x, y) == CellType::GHOST_SPAWN) {
                                ghostComp->setGridPosition(x, y);
                                
                                // Update visual position
                                auto posComp = std::dynamic_pointer_cast<PositionComponent>(
                                    _componentManager->getComponentByType(entity, ComponentType::POSITION));
                                
                                if (posComp) {
                                    posComp->x = static_cast<float>(x * 32);
                                    posComp->y = static_cast<float>(y * 32);
                                }
                                
                                ghostComp->setState(GhostState::NORMAL);
                                break;
                            }
                        }
                    }
                } else {
                    // PacMan loses a life
                    pacman->decrementLives();
                    
                    if (pacman->isDead()) {
                        // Game over
                        grid->setGameOver(true);
                    } else {
                        // Reset PacMan position to spawn
                        for (size_t y = 0; y < grid->getHeight(); y++) {
                            for (size_t x = 0; x < grid->getWidth(); x++) {
                                if (grid->getCellType(x, y) == CellType::PACMAN_SPAWN) {
                                    pacman->setGridPosition(x, y);
                                    
                                    // Update visual position
                                    Arcade::Entity pacmanEntity = 0;
                                    for (const auto& [e, n] : _entityManager->getEntities()) {
                                        auto comp = _componentManager->getComponentByType(e, static_cast<ComponentType>(1001));
                                        if (comp) {
                                            pacmanEntity = e;
                                            break;
                                        }
                                    }
                                    
                                    if (pacmanEntity) {
                                        auto posComp = std::dynamic_pointer_cast<PositionComponent>(
                                            _componentManager->getComponentByType(pacmanEntity, ComponentType::POSITION));
                                        
                                        if (posComp) {
                                            posComp->x = static_cast<float>(x * 32);
                                            posComp->y = static_cast<float>(y * 32);
                                        }
                                    }
                                    
                                    pacman->setCurrentDirection(Direction::NONE);
                                    pacman->setNextDirection(Direction::NONE);
                                    break;
                                }
                            }
                        }
                        
                        // Reset ghost positions
                        for (const auto& [e, n] : _entityManager->getEntities()) {
                            auto ghost = std::dynamic_pointer_cast<GhostComponent>(
                                _componentManager->getComponentByType(e, static_cast<ComponentType>(1002)));
                            
                            if (ghost) {
                                for (size_t y = 0; y < grid->getHeight(); y++) {
                                    for (size_t x = 0; x < grid->getWidth(); x++) {
                                        if (grid->getCellType(x, y) == CellType::GHOST_SPAWN) {
                                            ghost->setGridPosition(x, y);
                                            
                                            auto posComp = std::dynamic_pointer_cast<PositionComponent>(
                                                _componentManager->getComponentByType(e, ComponentType::POSITION));
                                            
                                            if (posComp) {
                                                posComp->x = static_cast<float>(x * 32);
                                                posComp->y = static_cast<float>(y * 32);
                                            }
                                            
                                            ghost->setState(GhostState::NORMAL);
                                            ghost->setCurrentDirection(Direction::NONE);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void GameLogic::checkCollisionsWithFood(std::shared_ptr<PacmanComponent> pacman,
                                      std::shared_ptr<GridComponent> grid) {
    size_t pacmanX = pacman->getGridX();
    size_t pacmanY = pacman->getGridY();
    
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity, static_cast<ComponentType>(1003)));
        
        if (foodComp && !foodComp->isEaten()) {
            size_t foodX = foodComp->getGridX();
            size_t foodY = foodComp->getGridY();
            
            if (pacmanX == foodX && pacmanY == foodY) {
                // Collision with food
                foodComp->setEaten(true);
                pacman->addScore(foodComp->getPoints());
                grid->decrementFoodCount();
                
                // Make the food invisible
                auto spriteComp = std::dynamic_pointer_cast<SpriteComponent>(
                    _componentManager->getComponentByType(entity, ComponentType::SPRITE));
                
                if (spriteComp) {
                    spriteComp->spritePath = "assets/pacman/empty.png";
                }
                
                // If power pill, make ghosts scared
                if (foodComp->getFoodType() == FoodType::POWER_PILL) {
                    for (const auto& [e, n] : _entityManager->getEntities()) {
                        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
                            _componentManager->getComponentByType(e, static_cast<ComponentType>(1002)));
                        
                        if (ghostComp) {
                            ghostComp->setState(GhostState::SCARED);
                            ghostComp->resetStateTimer();
                            
                            // Update sprite to scared ghost
                            auto ghostSprite = std::dynamic_pointer_cast<SpriteComponent>(
                                _componentManager->getComponentByType(e, ComponentType::SPRITE));
                            
                            if (ghostSprite) {
                                ghostSprite->spritePath = "assets/pacman/scared_ghost.png";
                            }
                        }
                    }
                }
            }
        }
    }
}

void GameLogic::checkWinCondition(std::shared_ptr<GridComponent> grid) {
    if (grid->getFoodCount() <= 0) {
        grid->setGameWon(true);
        grid->setGameOver(true);
    }
}

} // namespace PacMan
} // namespace Arcade