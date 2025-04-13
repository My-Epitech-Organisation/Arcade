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
        for (size_t y = 0; y < grid->getHeight(); y++) {
            for (size_t x = 0; x < grid->getWidth(); x++) {
                if (grid->getCellType(x, y) == CellType::GHOST_SPAWN) {
                    targetX = x;
                    targetY = y;
                    break;
                }
            }
        }
        auto ghostSpriteTemp = std::dynamic_pointer_cast<IDrawableComponent>(
            _componentManager->getComponentByType(entity,
                ComponentType::DRAWABLE));
        auto eyesAsset = _assets.find("ghosts.eyes");
        if (eyesAsset != _assets.end()) {
            *ghostSpriteTemp = eyesAsset->second;
        } else {
            ghostSpriteTemp->setAsTexture("assets/pacman/eyes.png", 32, 32);
            ghostSpriteTemp->setAsCharacter('e');
        }

        if (ghostComp->getGridX() == targetX &&
            ghostComp->getGridY() == targetY) {
            ghostComp->setState(GhostState::NORMAL);
            ghostComp->setCurrentDirection(Direction::NONE);

            auto ghostSprite = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));
            if (ghostSprite) {
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
                auto ghostAsset = _assets.find(ghostAssetKey);
                if (ghostAsset != _assets.end()) {
                    *ghostSprite = ghostAsset->second;
                } else {
                    // Fallback to hardcoded paths if asset not found
                    switch (ghostComp->getGhostType()) {
                        case GhostType::RED:
                            ghostSprite->setAsTexture
                                ("assets/pacman/ghost_red.png", 32, 32);
                            ghostSprite->setAsCharacter('r');
                            break;
                        case GhostType::PINK:
                            ghostSprite->setAsTexture
                                ("assets/pacman/ghost_pink.png", 32, 32);
                            ghostSprite->setAsCharacter('i');
                            break;
                        case GhostType::BLUE:
                            ghostSprite->setAsTexture
                                ("assets/pacman/ghost_cyan.png", 32, 32);
                            ghostSprite->setAsCharacter('c');
                            break;
                        case GhostType::ORANGE:
                            ghostSprite->setAsTexture
                                    ("assets/pacman/ghost_orange.png", 32, 32);
                            ghostSprite->setAsCharacter('o');
                            break;
                    }
                }
            }
        }
        ghostComp->setTarget(targetX, targetY);
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
            auto ghostSprite
                = std::dynamic_pointer_cast<IDrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));
            if (ghostSprite) {
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
                
                auto ghostAsset = getDrawableAsset(ghostAssetKey);
                if (ghostAsset) {
                    *ghostSprite = *ghostAsset;
                } else {
                    // Fallback to hardcoded paths
                    switch (ghostComp->getGhostType()) {
                        case GhostType::RED:
                            ghostSprite->setAsTexture
                                ("assets/pacman/ghost_red.png", 32, 32);
                            ghostSprite->setAsCharacter('r');
                            break;
                        case GhostType::PINK:
                            ghostSprite->setAsTexture
                                ("assets/pacman/ghost_pink.png", 32, 32);
                            ghostSprite->setAsCharacter('i');
                            break;
                        case GhostType::BLUE:
                            ghostSprite->setAsTexture
                                ("assets/pacman/ghost_cyan.png", 32, 32);
                            ghostSprite->setAsCharacter('c');
                            break;
                        case GhostType::ORANGE:
                            ghostSprite->setAsTexture
                                ("assets/pacman/ghost_orange.png", 32, 32);
                            ghostSprite->setAsCharacter('o');
                            break;
                    }
                }
            }
        }
    }
}

}  // namespace PacMan
}  // namespace Arcade
