// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Player Logic for Pacman implementation
*/

#include <iostream>
#include <memory>
#include <utility>
#include "Games/PacMan/System/PlayerLogic.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace PacMan {

PlayerLogic::PlayerLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
                         std::shared_ptr<Arcade::IEntityManager> entityManager,
                         const std::map<std::string, DrawableComponent>& assets)
    : _componentManager(componentManager), _entityManager(entityManager), _assets(assets) {
}

void PlayerLogic::movePacman(std::shared_ptr<PacmanComponent> pacman, 
                            std::shared_ptr<GridComponent> grid, 
                            std::shared_ptr<IEntity> pacmanEntity,
                            std::shared_ptr<IEntity> gridEntity) {
    if (!pacman || !grid)
        return;

    // If pacman is currently in animated movement, don't start a new move
    if (pacman->isMoving()) {
        return;
    }

    // Add debug output when movement is attempted but denied
    if (!pacman->canMove()) {
        static int debugCounter = 0;
        if (++debugCounter % 60 == 0) {  // Only log every 60 frames
            std::cout << "Pacman can't move right now, waiting for timer..." << std::endl;
        }
        return;
    }

    // Reset debug counter when movement is allowed
    static int debugCounter = 0;
    debugCounter = 0;

    Direction nextDir = pacman->getNextDirection();
    Direction currentDir = pacman->getCurrentDirection();
    Direction moveDir = Direction::NONE;

    size_t x = pacman->getGridX();
    size_t y = pacman->getGridY();

    if (nextDir != Direction::NONE &&
        canMoveInDirection(nextDir, x, y, grid)) {
        moveDir = nextDir;
        pacman->setCurrentDirection(nextDir);
    } else if (currentDir != Direction::NONE &&
        canMoveInDirection(currentDir, x, y, grid)) {
        moveDir = currentDir;
    }

    if (moveDir != Direction::NONE) {
        size_t newX = x;
        size_t newY = y;

        switch (moveDir) {
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
            default: break;
        }

        // Update grid position immediately for game logic
        pacman->setGridPosition(newX, newY);

        if (pacmanEntity) {
            auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
                _componentManager->getComponentByType(gridEntity,
                    ComponentType::POSITION));
            float startX = gridPosComp ? gridPosComp->x : 0;
            float startY = gridPosComp ? gridPosComp->y : 0;
            float cellSize = grid->getCellSize();
            
            // Set target position for smooth movement
            float targetX = startX + (newX * cellSize);
            float targetY = startY + (newY * cellSize);
            
            // Get current visual position or initialize it if not set
            float currentX = pacman->getVisualX();
            float currentY = pacman->getVisualY();
            if (currentX == 0 && currentY == 0) {
                currentX = startX + (x * cellSize);
                currentY = startY + (y * cellSize);
                pacman->setVisualPosition(currentX, currentY);
            }
            
            // Handle teleportation edge case
            if (moveDir == Direction::LEFT && x == 0 && newX == grid->getWidth() - 1) {
                // Going left from leftmost column to rightmost column
                pacman->setVisualPosition(startX + ((grid->getWidth()) * cellSize), currentY);
            } else if (moveDir == Direction::RIGHT && x == grid->getWidth() - 1 && newX == 0) {
                // Going right from rightmost column to leftmost column
                pacman->setVisualPosition(startX - cellSize, currentY);
            } else if (moveDir == Direction::UP && y == 0 && newY == grid->getHeight() - 1) {
                // Going up from top row to bottom row
                pacman->setVisualPosition(currentX, startY + ((grid->getHeight()) * cellSize));
            } else if (moveDir == Direction::DOWN && y == grid->getHeight() - 1 && newY == 0) {
                // Going down from bottom row to top row
                pacman->setVisualPosition(currentX, startY - cellSize);
            }
            
            // Set target position and mark as moving
            pacman->setTargetPosition(targetX, targetY);
            pacman->setMoving(true);
            
            // Update PositionComponent for collision detection
            auto pacmanPosComp = std::dynamic_pointer_cast<PositionComponent>(
                _componentManager->getComponentByType(pacmanEntity,
                    ComponentType::POSITION));
            if (pacmanPosComp) {
                // Update the actual position component for collision detection
                pacmanPosComp->x = targetX;
                pacmanPosComp->y = targetY;
            }
        }

        pacman->setCanMove(false);
        pacman->resetMovementTimer(); // Make sure the timer starts from 0
    }
}

void PlayerLogic::tryChangePacmanDirection(std::shared_ptr<PacmanComponent> pacman,
                                        std::shared_ptr<GridComponent> grid) {
    if (pacman->getNextDirection() != Direction::NONE) {
        Direction nextDir = pacman->getNextDirection();
        size_t x = pacman->getGridX();
        size_t y = pacman->getGridY();

        if (canMoveInDirection(nextDir, x, y, grid)) {
            // Use the new method that handles animation correctly
            pacman->updateDirectionAndAnimation(nextDir);
            pacman->setNextDirection(Direction::NONE);
            
            // Debug when direction changes
            std::cout << "Pacman direction changed to: " << static_cast<int>(nextDir) << std::endl;
        }
    }
}

bool PlayerLogic::canMoveInDirection(Direction dir, size_t x, size_t y,
                                    std::shared_ptr<GridComponent> grid) {
    if (!grid) return false;

    size_t nextX = x;
    size_t nextY = y;

    switch (dir) {
        case Direction::UP:
            nextY = (y == 0) ? grid->getHeight() - 1 : y - 1;
            break;
        case Direction::DOWN:
            nextY = (y == grid->getHeight() - 1) ? 0 : y + 1;
            break;
        case Direction::LEFT:
            nextX = (x == 0) ? grid->getWidth() - 1 : x - 1;
            break;
        case Direction::RIGHT:
            nextX = (x == grid->getWidth() - 1) ? 0 : x + 1;
            break;
        case Direction::NONE:
            return false;
    }

    return grid->getCellType(nextX, nextY) != CellType::WALL;
}

}  // namespace PacMan
}  // namespace Arcade
