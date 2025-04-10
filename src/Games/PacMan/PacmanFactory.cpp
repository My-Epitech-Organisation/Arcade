// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Factory for creating Pacman game entities
*/

#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>
#include "Games/PacMan/PacmanFactory.hpp"

namespace Arcade {
namespace PacMan {

Arcade::Entity PacmanFactory::createGrid(size_t width, size_t height) {
    Arcade::Entity gridEntity = _entityManager->createEntity("Grid");

    float screenWidth = 800.0f;
    float screenHeight = 600.0f;
    float cellSizeX = screenWidth / (width + 4);
    float cellSizeY = screenHeight / (height + 4);
    float cellSize = std::min(cellSizeX, cellSizeY);
    float boardWidth = width * cellSize;
    float boardHeight = height * cellSize;
    float x = (screenWidth - boardWidth) / 2;
    float y = (screenHeight - boardHeight) / 2;

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(gridEntity, positionComponent);

    auto gridComponent = std::make_shared<GridComponent>(width, height);
    gridComponent->setName("PacManGrid");
    gridComponent->loadDefaultMap();
    gridComponent->setCellSize(cellSize);
    _componentManager->registerComponent(gridEntity, gridComponent);

    return gridEntity;
}

Arcade::Entity PacmanFactory::createPacman(float x, float y,
size_t gridX, size_t gridY) {
    Arcade::Entity pacmanEntity = _entityManager->createEntity("Pacman");

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(pacmanEntity, positionComponent);

    auto spriteComponent = std::make_shared<SpriteComponent>
        ("assets/pacman/pacman.png");
    _componentManager->registerComponent(pacmanEntity, spriteComponent);

    auto pacmanComponent = std::make_shared<PacmanComponent>();
    pacmanComponent->setName("Pacman");
    pacmanComponent->setGridPosition(gridX, gridY);
    pacmanComponent->setCurrentDirection(Direction::NONE);
    _componentManager->registerComponent(pacmanEntity, pacmanComponent);

    return pacmanEntity;
}

Arcade::Entity PacmanFactory::createGhost(float x,
float y, size_t gridX, size_t gridY, GhostType type) {
    std::string ghostName;
    switch (type) {
        case GhostType::RED:
            ghostName = "Blinky";
            break;
        case GhostType::PINK:
            ghostName = "Pinky";
            break;
        case GhostType::BLUE:
            ghostName = "Inky";
            break;
        case GhostType::ORANGE:
            ghostName = "Clyde";
            break;
    }

    Arcade::Entity ghostEntity = _entityManager->createEntity(ghostName);

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(ghostEntity, positionComponent);

    auto spriteComponent = std::make_shared<SpriteComponent>(
        getGhostSpritePath(type));
    _componentManager->registerComponent(ghostEntity, spriteComponent);

    auto ghostComponent = std::make_shared<GhostComponent>(type);
    ghostComponent->setName(ghostName);
    ghostComponent->setGridPosition(gridX, gridY);
    ghostComponent->setState(GhostState::NORMAL);
    _componentManager->registerComponent(ghostEntity, ghostComponent);

    return ghostEntity;
}

Arcade::Entity PacmanFactory::createFood(float x, float y,
size_t gridX, size_t gridY, FoodType type) {
    std::string foodName = type == FoodType::POWER_PILL ? "PowerPill" : "Food";
    foodName += "_" + std::to_string(gridX) + "_" + std::to_string(gridY);

    Arcade::Entity foodEntity = _entityManager->createEntity(foodName);

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(foodEntity, positionComponent);

    std::string spritePath = type == FoodType::POWER_PILL ?
                            "assets/pacman/power_pellet.png" :
                            "assets/pacman/dot.png";
    auto spriteComponent = std::make_shared<SpriteComponent>(spritePath);
    _componentManager->registerComponent(foodEntity, spriteComponent);

    auto foodComponent = std::make_shared<FoodComponent>(type);
    foodComponent->setName(foodName);
    foodComponent->setGridPosition(gridX, gridY);
    _componentManager->registerComponent(foodEntity, foodComponent);

    return foodEntity;
}

Arcade::Entity PacmanFactory::createWall(float x, float y,
size_t gridX, size_t gridY) {
    std::string wallName = "Wall_" + std::to_string(gridX)
    + "_" + std::to_string(gridY);
    Arcade::Entity wallEntity = _entityManager->createEntity(wallName);

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(wallEntity, positionComponent);

    auto spriteComponent = std::make_shared<SpriteComponent>(
        "assets/pacman/wall.png");
    _componentManager->registerComponent(wallEntity, spriteComponent);

    return wallEntity;
}

std::string PacmanFactory::getGhostSpritePath(GhostType type) {
    switch (type) {
        case GhostType::RED:
            return "assets/pacman/ghost_red.png";
        case GhostType::PINK:
            return "assets/pacman/ghost_pink.png";
        case GhostType::BLUE:
            return "assets/pacman/ghost_cyan.png";
        case GhostType::ORANGE:
            return "assets/pacman/ghost_orange.png";
        default:
            return "assets/pacman/ghost_red.png";
    }
}

void PacmanFactory::initializeGame(float cellSize) {
    Arcade::Entity gridEntity = createGrid(28, 31);
    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));
    if (!gridComp)
        return;
    auto posComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));
    float startX = posComp ? posComp->x : 0;
    float startY = posComp ? posComp->y : 0;
    createWallEntities(gridComp, gridComp->getCellSize(), gridEntity);
    createFoodEntities(gridComp, gridComp->getCellSize(), gridEntity);
    createPacmanEntity(gridComp, gridComp->getCellSize(), gridEntity);
    createGhostEntities(gridComp, gridComp->getCellSize(), gridEntity);
}

void PacmanFactory::createFoodEntities(std::shared_ptr<GridComponent>
grid, float cellSize, Arcade::Entity gridEntity) {
    auto posComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));

    float startX = posComp ? posComp->x : 0;
    float startY = posComp ? posComp->y : 0;

    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            CellType cellType = grid->getCellType(x, y);
            if (cellType == CellType::FOOD) {
                float foodX = startX + (x * cellSize);
                float foodY = startY + (y * cellSize);
                Arcade::Entity foodEntity = createFood(foodX,
                    foodY, x, y, FoodType::NORMAL_DOT);
                grid->setEntityAtCell(x, y, foodEntity);
            } else if (cellType == CellType::POWER_PILL) {
                float foodX = startX + (x * cellSize);
                float foodY = startY + (y * cellSize);
                Arcade::Entity foodEntity = createFood(foodX, foodY,
                    x, y, FoodType::POWER_PILL);
                grid->setEntityAtCell(x, y, foodEntity);
            }
        }
    }
}

void PacmanFactory::createWallEntities(std::shared_ptr<GridComponent> grid,
float cellSize, Arcade::Entity gridEntity) {
    auto posComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));

    float startX = posComp ? posComp->x : 0;
    float startY = posComp ? posComp->y : 0;

    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            if (grid->getCellType(x, y) == CellType::WALL) {
                float wallX = startX + (x * cellSize);
                float wallY = startY + (y * cellSize);
                Arcade::Entity wallEntity = createWall(wallX, wallY, x, y);
                grid->setEntityAtCell(x, y, wallEntity);
            }
        }
    }
}

void PacmanFactory::createGhostEntities(std::shared_ptr<GridComponent>
grid, float cellSize, Arcade::Entity gridEntity) {
    auto posComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));

    float startX = posComp ? posComp->x : 0;
    float startY = posComp ? posComp->y : 0;

    std::vector<std::pair<size_t, size_t>> ghostSpawns;
    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            if (grid->getCellType(x, y) == CellType::GHOST_SPAWN) {
                ghostSpawns.push_back({x, y});
            }
        }
    }

    GhostType types[] = {GhostType::RED, GhostType::PINK,
        GhostType::BLUE, GhostType::ORANGE};
    for (size_t i = 0; i < ghostSpawns.size() && i < 4; ++i) {
        auto [x, y] = ghostSpawns[i];
        float ghostX = startX + (x * cellSize);
        float ghostY = startY + (y * cellSize);
        Arcade::Entity ghostEntity = createGhost(ghostX, ghostY,
            x, y, types[i]);
        grid->setEntityAtCell(x, y, ghostEntity);
    }
}

void PacmanFactory::createPacmanEntity(std::shared_ptr<GridComponent>
grid, float cellSize, Arcade::Entity gridEntity) {
    auto posComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));

    float startX = posComp ? posComp->x : 0;
    float startY = posComp ? posComp->y : 0;

    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            if (grid->getCellType(x, y) == CellType::PACMAN_SPAWN) {
                float pacmanX = startX + (x * cellSize);
                float pacmanY = startY + (y * cellSize);
                Arcade::Entity pacmanEntity = createPacman(pacmanX,
                    pacmanY, x, y);
                grid->setEntityAtCell(x, y, pacmanEntity);
                return;
            }
        }
    }
    // Fallback: Place Pacman at the center if no spawn point found
    size_t centerX = grid->getWidth() / 2;
    size_t centerY = grid->getHeight() / 2;
    float pacmanX = startX + (centerX * cellSize);
    float pacmanY = startY + (centerY * cellSize);
    Arcade::Entity pacmanEntity = createPacman(pacmanX,
        pacmanY, centerX, centerY);
    grid->setEntityAtCell(centerX, centerY, pacmanEntity);
}

}  // namespace PacMan
}  // namespace Arcade
