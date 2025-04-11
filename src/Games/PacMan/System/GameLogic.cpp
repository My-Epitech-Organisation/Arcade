// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game Logic System for Pacman
*/

#include <iostream>
#include <chrono>
#include <memory>
#include <utility>
#include <algorithm>
#include <array>
#include <string>
#include <ctime>
#include "Games/PacMan/System/GameLogic.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/FoodComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace PacMan {

GameLogic::GameLogic(std::shared_ptr<Arcade::IComponentManager>
componentManager,
std::shared_ptr<Arcade::IEntityManager> entityManager)
: _componentManager(componentManager), _entityManager(entityManager) {
    _lastUpdateTime = std::chrono::high_resolution_clock::now();
}

void GameLogic::moveGhost(std::shared_ptr<GhostComponent> ghostComp,
Arcade::Entity entity, std::shared_ptr<GridComponent> grid,
std::shared_ptr<PacmanComponent> pacman) {
    if (!ghostComp->canMove() || !grid || !pacman)
        return;

    size_t pacmanX = pacman->getGridX();
    size_t pacmanY = pacman->getGridY();
    Direction pacmanDir = pacman->getCurrentDirection();
    size_t targetX = 0;
    size_t targetY = 0;

    if (ghostComp->getState() == GhostState::SCARED) {
        unsigned int seed = static_cast<unsigned int>(
            std::time(nullptr) + entity +
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

        auto ghostSpriteTemp = std::dynamic_pointer_cast<DrawableComponent>(
            _componentManager->getComponentByType(entity,
                ComponentType::DRAWABLE));

        ghostSpriteTemp->path = "assets/pacman/eyes.png";
        ghostSpriteTemp->setAsTexture("assets/pacman/eyes.png", 32, 32);
        ghostSpriteTemp->setAsCharacter('G');

        if (ghostComp->getGridX() == targetX &&
            ghostComp->getGridY() == targetY) {
            ghostComp->setState(GhostState::NORMAL);
            ghostComp->setCurrentDirection(Direction::NONE);

            auto ghostSprite = std::dynamic_pointer_cast<DrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));
            ghostSprite->isVisible = true;
            if (ghostSprite) {
                switch (ghostComp->getGhostType()) {
                    case GhostType::RED:
                        ghostSprite->path =
                            "assets/pacman/ghost_red.png";
                        ghostSprite->setAsTexture(
                            "assets/pacman/ghost_red.png", 32, 32);
                        ghostSprite->setAsCharacter('r');
                        break;
                    case GhostType::PINK:
                        ghostSprite->path =
                            "assets/pacman/ghost_pink.png";
                        ghostSprite->setAsTexture(
                            "assets/pacman/ghost_pink.png", 32, 32);
                        ghostSprite->setAsCharacter('i');
                        break;
                    case GhostType::BLUE:
                        ghostSprite->path =
                            "assets/pacman/ghost_cyan.png";
                        ghostSprite->setAsTexture(
                            "assets/pacman/ghost_cyan.png", 32, 32);
                        ghostSprite->setAsCharacter('c');
                        break;
                    case GhostType::ORANGE:
                        ghostSprite->path =
                            "assets/pacman/ghost_orange.png";
                        ghostSprite->setAsTexture(
                            "assets/pacman/ghost_orange.png", 32, 32);
                        ghostSprite->setAsCharacter('o');
                        break;
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
                        _entityManager->getEntities()) {
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

    if (canMoveInDirection(dir, x, y, grid)) {
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
            default: break;
        }

        bool ghostCollision = false;
        for (const auto& [e, name] : _entityManager->getEntities()) {
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
            ghostComp->setGridPosition(newX, newY);
            auto posComp = std::dynamic_pointer_cast<PositionComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::POSITION));
            if (posComp) {
                auto gridEntity = findGridEntity();
                auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
                    _componentManager->getComponentByType(gridEntity,
                        ComponentType::POSITION));
                float startX = gridPosComp ? gridPosComp->x : 0;
                float startY = gridPosComp ? gridPosComp->y : 0;
                float cellSize = grid->getCellSize();
                posComp->x = startX + (newX * cellSize);
                posComp->y = startY + (newY * cellSize);
                // Add this code to update the DrawableComponent
                auto ghostDrawable
                    = std::dynamic_pointer_cast<DrawableComponent>(
                    _componentManager->getComponentByType(entity,
                        ComponentType::DRAWABLE));
                if (ghostDrawable) {
                    ghostDrawable->posX = startX + (newX * cellSize);
                    ghostDrawable->posY = startY + (newY * cellSize);
                }
            }
            ghostComp->setCanMove(false);
            ghostComp->resetMovementTimer();
        } else {
            ghostComp->setCurrentDirection(Direction::NONE);
        }
    } else {
        ghostComp->setCurrentDirection(Direction::NONE);
    }
}

void GameLogic::update() {
    updateDeltaTime();
    float deltaTime = _currentDeltaTime;

    Arcade::Entity pacmanEntity = findPacmanEntity();
    Arcade::Entity gridEntity = findGridEntity();

    if (!pacmanEntity || (gridEntity != 0))
        return;

    auto pacman = std::dynamic_pointer_cast<PacmanComponent>(
        _componentManager->getComponentByType(pacmanEntity,
            static_cast<ComponentType>(1001)));
    auto grid = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));

    if (!pacman || !grid)
        return;

    if (grid->isGameOver() || pacman->isDead()) {
        grid->setGameOver(true);
        return;
    }
    pacman->updateMovementTimer(deltaTime);
    pacman->addGameTime(deltaTime);

    if (pacman->canMove())
        movePacman();

    if (pacman->getCurrentDirection() == Direction::NONE)
        pacman->setCurrentDirection(Direction::RIGHT);

    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));
        if (!ghostComp)
            continue;
        if (!ghostComp->canLeaveBox()) {
            ghostComp->updateReleaseTimer(deltaTime);
            continue;
        }
        if (ghostComp) {
            GhostState previousState = ghostComp->getState();

            ghostComp->addGameTime(deltaTime);
            ghostComp->updateStateTimer(deltaTime);
            ghostComp->updateMovementTimer(deltaTime);
            ghostComp->updateModeTimer(deltaTime);


            if (ghostComp->canMove()) {
                if (ghostComp->getCurrentDirection() == Direction::NONE) {
                    unsigned int seed = static_cast<unsigned int>(
                        std::time(nullptr) + entity +
                        static_cast<unsigned int>(std::chrono::duration_cast
                            <std::chrono::milliseconds>(
                            std::chrono::high_resolution_clock::
                                now().time_since_epoch()).count() % 10000));
                    Direction dirs[] = {Direction::UP, Direction::DOWN,
                        Direction::LEFT, Direction::RIGHT};
                    ghostComp->setCurrentDirection(dirs[rand_r(&seed) % 4]);
                }
                moveGhost(ghostComp, entity, grid, pacman);
            }
            if (previousState == GhostState::SCARED &&
                ghostComp->getState() == GhostState::NORMAL) {
                auto ghostSprite = std::dynamic_pointer_cast<DrawableComponent>(
                    _componentManager->getComponentByType(entity,
                        ComponentType::DRAWABLE));
                if (ghostSprite) {
                    switch (ghostComp->getGhostType()) {
                        case GhostType::RED:
                            ghostSprite->path =
                                "assets/pacman/ghost_red.png";
                            ghostSprite->setAsTexture(
                                "assets/pacman/ghost_red.png", 32, 32);
                            ghostSprite->setAsCharacter('r');
                            break;
                        case GhostType::PINK:
                            ghostSprite->path =
                                "assets/pacman/ghost_pink.png";
                            ghostSprite->setAsTexture(
                                "assets/pacman/ghost_pink.png", 32, 32);
                            ghostSprite->setAsCharacter('i');
                            break;
                        case GhostType::BLUE:
                            ghostSprite->path =
                                "assets/pacman/ghost_cyan.png";
                            ghostSprite->setAsTexture(
                                "assets/pacman/ghost_cyan.png", 32, 32);
                            ghostSprite->setAsCharacter('c');
                            break;
                        case GhostType::ORANGE:
                            ghostSprite->path =
                                "assets/pacman/ghost_orange.png";
                            ghostSprite->setAsTexture(
                                "assets/pacman/ghost_orange.png", 32, 32);
                            ghostSprite->setAsCharacter('o');
                            break;
                    }
                }
            }
        }
    }

    tryChangePacmanDirection(pacman, grid);
    checkCollisions();
    checkWinCondition(grid);
}
float GameLogic::getDeltaTime() {
    return _currentDeltaTime;
}

void GameLogic::updateDeltaTime() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    _currentDeltaTime = std::chrono::duration<float>(currentTime
        - _lastUpdateTime).count();
    _lastUpdateTime = currentTime;
}

void GameLogic::tryChangePacmanDirection(std::shared_ptr<PacmanComponent>
pacman, std::shared_ptr<GridComponent> grid) {
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

Arcade::Entity GameLogic::findPacmanEntity() {
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto pacmanComp = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1001));
        if (pacmanComp)
            return entity;
    }
    return -1;
}

Arcade::Entity GameLogic::findGridEntity() {
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto gridComp = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1000));
        if (gridComp)
            return entity;
    }
    return -1;
}

std::pair<std::shared_ptr<PacmanComponent>, std::shared_ptr<GridComponent>>
GameLogic::getPacmanAndGridComponents() {
    Arcade::Entity pacmanEntity = findPacmanEntity();
    Arcade::Entity gridEntity = findGridEntity();

    std::shared_ptr<PacmanComponent> pacmanComp = nullptr;
    std::shared_ptr<GridComponent> gridComp = nullptr;

    if (pacmanEntity) {
        pacmanComp = std::dynamic_pointer_cast<PacmanComponent>(
            _componentManager->getComponentByType(pacmanEntity,
                static_cast<ComponentType>(1001)));
    }

    if (gridEntity == 0) {
        gridComp = std::dynamic_pointer_cast<GridComponent>(
            _componentManager->getComponentByType(gridEntity,
                static_cast<ComponentType>(1000)));
    }

    return {pacmanComp, gridComp};
}

void GameLogic::checkFoodCollision(std::shared_ptr<PacmanComponent> pacman,
std::shared_ptr<GridComponent> grid) {
    checkCollisionsWithFood(pacman, grid);
}

void GameLogic::movePacman() {
    auto [pacmanComp, gridComp] = getPacmanAndGridComponents();
    if (!pacmanComp || !gridComp)
        return;

    Direction nextDir = pacmanComp->getNextDirection();
    Direction currentDir = pacmanComp->getCurrentDirection();
    Direction moveDir = Direction::NONE;

    size_t x = pacmanComp->getGridX();
    size_t y = pacmanComp->getGridY();

    if (nextDir != Direction::NONE &&
        canMoveInDirection(nextDir, x, y, gridComp)) {
        moveDir = nextDir;
        pacmanComp->setCurrentDirection(nextDir);
    } else if (currentDir != Direction::NONE &&
        canMoveInDirection(currentDir, x, y, gridComp)) {
        moveDir = currentDir;
    }

    if (moveDir != Direction::NONE) {
        size_t newX = x;
        size_t newY = y;

        switch (moveDir) {
            case Direction::UP:
                newY = (y == 0) ? gridComp->getHeight() - 1 : y - 1;
                break;
            case Direction::DOWN:
                newY = (y == gridComp->getHeight() - 1) ? 0 : y + 1;
                break;
            case Direction::LEFT:
                newX = (x == 0) ? gridComp->getWidth() - 1 : x - 1;
                break;
            case Direction::RIGHT:
                newX = (x == gridComp->getWidth() - 1) ? 0 : x + 1;
                break;
            default: break;
        }

        pacmanComp->setGridPosition(newX, newY);

        Arcade::Entity pacmanEntity = findPacmanEntity();
        if (pacmanEntity) {
            auto gridPosComp = std::dynamic_pointer_cast<PositionComponent>(
                _componentManager->getComponentByType(findGridEntity(),
                    ComponentType::POSITION));
            float startX = gridPosComp ? gridPosComp->x : 0;
            float startY = gridPosComp ? gridPosComp->y : 0;
            float cellSize = gridComp->getCellSize();
            auto pacmanPosComp = std::dynamic_pointer_cast<PositionComponent>(
                _componentManager->getComponentByType(pacmanEntity,
                    ComponentType::POSITION));
            if (pacmanPosComp) {
                pacmanPosComp->x = startX + (newX * cellSize);
                pacmanPosComp->y = startY + (newY * cellSize);
            }
            auto pacmanDrawable = std::dynamic_pointer_cast<DrawableComponent>(
                _componentManager->getComponentByType(pacmanEntity,
                    ComponentType::DRAWABLE));
            if (pacmanDrawable) {
                pacmanDrawable->posX = startX + (newX * cellSize);
                pacmanDrawable->posY = startY + (newY * cellSize);
            }
        }

        pacmanComp->setCanMove(false);
        checkFoodCollision(pacmanComp, gridComp);
    }
}

size_t calculateDistance(size_t x1, size_t y1, size_t x2, size_t y2) {
    return std::abs(static_cast<int>(x1) - static_cast<int>(x2)) +
           std::abs(static_cast<int>(y1) - static_cast<int>(y2));
}

void GameLogic::checkCollisions() {
    Arcade::Entity pacmanEntity = findPacmanEntity();
    Arcade::Entity gridEntity = findGridEntity();

    if (!pacmanEntity || gridEntity != 0)
        return;

    auto pacman = std::dynamic_pointer_cast<PacmanComponent>(
        _componentManager->getComponentByType(pacmanEntity,
            static_cast<ComponentType>(1001)));
    auto grid = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));

    if (!pacman || !grid)
        return;

    checkCollisionsWithGhosts(pacman, grid);
    checkCollisionsWithFood(pacman, grid);
}

void GameLogic::checkCollisionsWithGhosts(std::shared_ptr<PacmanComponent>
pacman, std::shared_ptr<GridComponent> grid) {
    if (!pacman || !grid) return;

    size_t pacmanX = pacman->getGridX();
    size_t pacmanY = pacman->getGridY();

    for (const auto& [entity, name] : _entityManager->getEntities()) {
        if (name.find("Blinky") != std::string::npos ||
            name.find("Pinky") != std::string::npos ||
            name.find("Inky") != std::string::npos ||
            name.find("Clyde") != std::string::npos) {
            auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
                _componentManager->getComponentByType(entity,
                    static_cast<ComponentType>(1002)));

            if (!ghostComp) continue;

            size_t ghostX = ghostComp->getGridX();
            size_t ghostY = ghostComp->getGridY();

            if (pacmanX == ghostX && pacmanY == ghostY) {
                if (ghostComp->getState() == GhostState::SCARED) {
                    ghostComp->setState(GhostState::RETURNING);
                    pacman->addScore(200);
                } else if (ghostComp->getState() != GhostState::RETURNING) {
                    pacman->decrementLives();
                    if (pacman->getLives() <= 0) {
                        grid->setGameOver(true);
                        return;
                    }

                    for (size_t y = 0; y < grid->getHeight(); y++) {
                        for (size_t x = 0; x < grid->getWidth(); x++) {
                            if (grid->getCellType(x, y) ==
                                CellType::PACMAN_SPAWN) {
                                pacman->setGridPosition(x, y);
                                break;
                            }
                        }
                    }

                    pacman->setCurrentDirection(Direction::NONE);
                    pacman->setNextDirection(Direction::NONE);
                    break;
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
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1003)));

        if (foodComp && !foodComp->isEaten() &&
            foodComp->getGridX() == pacmanX &&
            foodComp->getGridY() == pacmanY) {
            foodComp->setEaten(true);
            pacman->addScore(foodComp->getPoints());
            grid->decrementFoodCount();

            auto spriteComp = std::dynamic_pointer_cast<DrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));

            if (spriteComp) {
                spriteComp->path = "assets/pacman/empty.png";
                spriteComp->setAsTexture("assets/pacman/empty.png", 32, 32);
                spriteComp->setAsCharacter(' ');
            }

            if (foodComp->getFoodType() == FoodType::POWER_PILL) {
                for (const auto& [e, n] : _entityManager->getEntities()) {
                    auto ghostComp = std::dynamic_pointer_cast
                        <GhostComponent>(
                        _componentManager->getComponentByType(e,
                            static_cast<ComponentType>(1002)));

                    if (ghostComp) {
                        ghostComp->setState(GhostState::SCARED);
                        ghostComp->resetStateTimer();

                        auto ghostSprite = std::dynamic_pointer_cast
                            <DrawableComponent>(
                            _componentManager->getComponentByType(e,
                                ComponentType::DRAWABLE));
                        if (ghostSprite) {
                            ghostSprite->path =
                                "assets/pacman/scared_ghost.png";
                            ghostSprite->setAsTexture(
                                "assets/pacman/scared_ghost.png", 32, 32);
                            ghostSprite->setAsCharacter('s');
                        }
                    }
                }
            }
        }
    }
}

void GameLogic::reloadCurrentMap() {
    auto gridEntity = findGridEntity();
    auto grid = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));

    if (!grid)
        return;

    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto foodComp = std::dynamic_pointer_cast<FoodComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1003)));

        if (foodComp) {
            foodComp->setEaten(false);

            auto spriteComp = std::dynamic_pointer_cast<DrawableComponent>(
                _componentManager->getComponentByType(entity,
                    ComponentType::DRAWABLE));

            if (spriteComp) {
                if (foodComp->getFoodType() == FoodType::NORMAL_DOT) {
                    spriteComp->path = "assets/pacman/dot.png";
                    spriteComp->setAsTexture("assets/pacman/dot.png", 32, 32);
                    spriteComp->setAsCharacter('.');
                } else {
                    spriteComp->path = "assets/pacman/power_pellet.png";
                    spriteComp->setAsTexture("assets/pacman/power_pellet.png",
                        32, 32);
                    spriteComp->setAsCharacter('U');
                }
            }
        }
    }

    size_t totalFoodCount = grid->getTotalFoodCount();
    grid->setFoodCount(totalFoodCount);

    auto pacmanEntity = findPacmanEntity();
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

                auto pacmanPosComp = std::dynamic_pointer_cast
                    <PositionComponent>(
                    _componentManager->getComponentByType(pacmanEntity,
                        ComponentType::POSITION));

                if (pacmanPosComp) {
                    pacmanPosComp->x = startX + (x * cellSize);
                    pacmanPosComp->y = startY + (y * cellSize);
                }
                auto pacmanDrawable
                    = std::dynamic_pointer_cast<DrawableComponent>(
                    _componentManager->getComponentByType(pacmanEntity,
                        ComponentType::DRAWABLE));
                if (pacmanDrawable) {
                    pacmanDrawable->posX = startX + (x * cellSize);
                    pacmanDrawable->posY = startY + (y * cellSize);
                }
            }

            if (grid->getCellType(x, y) == CellType::GHOST_SPAWN) {
                for (const auto& [entity, name] :
                        _entityManager->getEntities()) {
                    auto ghostComp = std::dynamic_pointer_cast
                        <GhostComponent>(
                        _componentManager->getComponentByType(entity,
                            static_cast<ComponentType>(1002)));

                    if (ghostComp) {
                        ghostComp->setGridPosition(x, y);
                        ghostComp->setState(GhostState::NORMAL);
                        ghostComp->setCurrentDirection(Direction::NONE);
                        ghostComp->resetReleaseTimer();

                        auto ghostPosComp = std::dynamic_pointer_cast
                            <PositionComponent>(
                            _componentManager->getComponentByType(entity,
                                ComponentType::POSITION));

                        if (ghostPosComp) {
                            ghostPosComp->x = startX + (x * cellSize);
                            ghostPosComp->y = startY + (y * cellSize);
                        }

                        auto ghostDrawable
                            = std::dynamic_pointer_cast<DrawableComponent>(
                            _componentManager->getComponentByType(entity,
                                ComponentType::DRAWABLE));
                        if (ghostDrawable) {
                            ghostDrawable->posX = startX + (x * cellSize);
                            ghostDrawable->posY = startY + (y * cellSize);
                        }

                        auto ghostSprite
                            = std::dynamic_pointer_cast<DrawableComponent>(
                            _componentManager->getComponentByType(entity,
                            ComponentType::DRAWABLE));
                        if (ghostSprite) {
                            switch (ghostComp->getGhostType()) {
                                case GhostType::RED:
                                    ghostSprite->path =
                                        "assets/pacman/ghost_red.png";
                                    ghostSprite->setAsTexture(
                                        "assets/pacman/ghost_red.png", 32, 32);
                                    ghostSprite->setAsCharacter('r');
                                    break;
                                case GhostType::PINK:
                                    ghostSprite->path =
                                        "assets/pacman/ghost_pink.png";
                                    ghostSprite->setAsTexture(
                                        "assets/pacman/ghost_pink.png", 32, 32);
                                    ghostSprite->setAsCharacter('i');
                                    break;
                                case GhostType::BLUE:
                                    ghostSprite->path =
                                        "assets/pacman/ghost_cyan.png";
                                    ghostSprite->setAsTexture(
                                        "assets/pacman/ghost_cyan.png", 32, 32);
                                    ghostSprite->setAsCharacter('c');
                                    break;
                                case GhostType::ORANGE:
                                    ghostSprite->path =
                                        "assets/pacman/ghost_orange.png";
                                    ghostSprite->setAsTexture(
                                        "assets/pacman/ghost_orange.png",
                                        32, 32);
                                    ghostSprite->setAsCharacter('o');
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }

    grid->setGameOver(false);
    grid->setGameWon(false);
}

void GameLogic::increaseGameSpeed() {
    auto pacmanEntity = findPacmanEntity();
    if (pacmanEntity) {
        auto pacman = std::dynamic_pointer_cast<PacmanComponent>(
            _componentManager->getComponentByType(pacmanEntity,
                static_cast<ComponentType>(1001)));

        if (pacman) {
            float newThreshold = pacman->getMovementThreshold() * 0.8f;
            pacman->setMovementThreshold(newThreshold);
        }
    }

    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto ghostComp = std::dynamic_pointer_cast<GhostComponent>(
            _componentManager->getComponentByType(entity,
                static_cast<ComponentType>(1002)));

        if (ghostComp) {
            float newThreshold = ghostComp->getMovementThreshold() * 0.8f;
            ghostComp->setMovementThreshold(newThreshold);
        }
    }
}

void GameLogic::checkWinCondition(std::shared_ptr<GridComponent> grid) {
    if (grid->getFoodCount() <= 0) {
        grid->setGameWon(true);
        grid->setGameOver(true);
        grid->incrementLevel();
        reloadCurrentMap();
        increaseGameSpeed();
    }
}

}  // namespace PacMan
}  // namespace Arcade
