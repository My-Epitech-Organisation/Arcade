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
#include <utility>
#include <memory>
#include <iostream>
#include "Games/PacMan/PacmanFactory.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace PacMan {

std::shared_ptr<Arcade::IEntity>
PacmanFactory::createGrid(size_t width, size_t height) {
    std::shared_ptr<Arcade::IEntity> gridEntity
    = _entityManager->createEntity("Grid");

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

std::shared_ptr<Arcade::IEntity> PacmanFactory::createPacman(float x, float y,
size_t gridX, size_t gridY) {
    std::shared_ptr<Arcade::IEntity> pacmanEntity
        = _entityManager->createEntity("Pacman");
    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(pacmanEntity, positionComponent);
    auto spriteComponent = getDrawableAsset("pacman.default");
    if (!spriteComponent) {
        spriteComponent = std::make_shared<DrawableComponent>();
        spriteComponent->setAsTexture("assets/pacman/pacman.png", 32, 32);
    }
    spriteComponent->setPosition(x, y);
    _componentManager->registerComponent(pacmanEntity, spriteComponent);
    auto pacmanComponent = std::make_shared<PacmanComponent>();
    pacmanComponent->setName("Pacman");
    pacmanComponent->setGridPosition(gridX, gridY);
    pacmanComponent->setVisualPosition(x, y);
    pacmanComponent->setTargetPosition(x, y);
    pacmanComponent->setCurrentDirection(Direction::NONE);
    pacmanComponent->resetAnimation();
    pacmanComponent->setAnimationEnabled(true);
    _componentManager->registerComponent(pacmanEntity, pacmanComponent);
    return pacmanEntity;
}

std::shared_ptr<Arcade::IEntity> PacmanFactory::createGhost(float x,
float y, size_t gridX, size_t gridY, GhostType type) {
    std::string ghostName;
    std::string assetKey;
    switch (type) {
        case GhostType::RED:
            ghostName = "Blinky";
            assetKey = "ghosts.red";
            break;
        case GhostType::PINK:
            ghostName = "Pinky";
            assetKey = "ghosts.pink";
            break;
        case GhostType::BLUE:
            ghostName = "Inky";
            assetKey = "ghosts.blue";
            break;
        case GhostType::ORANGE:
            ghostName = "Clyde";
            assetKey = "ghosts.orange";
            break;
    }
    std::shared_ptr<Arcade::IEntity> ghostEntity
        = _entityManager->createEntity(ghostName);
    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(ghostEntity, positionComponent);
    auto spriteComponent = getDrawableAsset(assetKey);
    if (!spriteComponent) {
        spriteComponent = std::make_shared<DrawableComponent>();
        spriteComponent->setAsTexture(getGhostSpritePath(type), 32, 32);
        spriteComponent->setAsCharacter(getGhostCharacter(type));
    }
    spriteComponent->setPosition(x, y);
    spriteComponent->setVisibility(true);
    _componentManager->registerComponent(ghostEntity, spriteComponent);

    auto ghostComponent = std::make_shared<GhostComponent>(type);
    ghostComponent->setName(ghostName);
    ghostComponent->setGridPosition(gridX, gridY);
    ghostComponent->setVisualPosition(x, y);
    ghostComponent->setTargetPosition(x, y);
    ghostComponent->setState(GhostState::NORMAL);
    _componentManager->registerComponent(ghostEntity, ghostComponent);

    return ghostEntity;
}

std::shared_ptr<Arcade::IEntity> PacmanFactory::createFood(float x, float y,
size_t gridX, size_t gridY, FoodType type) {
    std::string foodName = type == FoodType::POWER_PILL ? "PowerPill" : "Food";
    std::string assetKey = type == FoodType::POWER_PILL
        ? "map.power_pellet" : "map.food";
    foodName += "_" + std::to_string(gridX) + "_" + std::to_string(gridY);

    std::shared_ptr<Arcade::IEntity> foodEntity
        = _entityManager->createEntity(foodName);

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(foodEntity, positionComponent);

    auto spriteComponent = getDrawableAsset(assetKey);
    if (!spriteComponent) {
        spriteComponent = std::make_shared<DrawableComponent>();
        std::string spritePath = type == FoodType::POWER_PILL ?
                                "assets/pacman/power_pellet.png" :
                                "assets/pacman/dot.png";
        spriteComponent->setAsTexture(spritePath, 32, 32);
        spriteComponent->setAsCharacter
            (type == FoodType::POWER_PILL ? 'O' : '.');
    }
    spriteComponent->setPosition(x, y);
    spriteComponent->setVisibility(true);
    _componentManager->registerComponent(foodEntity, spriteComponent);
    auto foodComponent = std::make_shared<FoodComponent>(type);
    foodComponent->setName(foodName);
    foodComponent->setGridPosition(gridX, gridY);
    _componentManager->registerComponent(foodEntity, foodComponent);
    return foodEntity;
}

std::shared_ptr<Arcade::IEntity> PacmanFactory::createWall(float x, float y,
size_t gridX, size_t gridY) {
    std::string wallName = "Wall_" + std::to_string(gridX)
    + "_" + std::to_string(gridY);
    std::shared_ptr<Arcade::IEntity> wallEntity
        = _entityManager->createEntity(wallName);
    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(wallEntity, positionComponent);
    auto spriteComponent = getDrawableAsset("map.wall");
    if (!spriteComponent) {
        spriteComponent = std::make_shared<DrawableComponent>();
        spriteComponent->setAsTexture("assets/pacman/wall.png", 32, 32);
        spriteComponent->setAsCharacter('#');
    }
    spriteComponent->setPosition(x, y);
    spriteComponent->setVisibility(true);
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

char PacmanFactory::getGhostCharacter(GhostType type) {
    switch (type) {
        case GhostType::RED:
            return 'r';
        case GhostType::PINK:
            return 'i';
        case GhostType::BLUE:
            return 'c';
        case GhostType::ORANGE:
            return 'o';
        default:
            return 'G';
    }
}

void PacmanFactory::initializeGame(float cellSize) {
    std::shared_ptr<Arcade::IEntity> gridEntity = createGrid(28, 31);
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
grid, float cellSize, std::shared_ptr<Arcade::IEntity> gridEntity) {
    auto posComp = std::dynamic_pointer_cast<PositionComponent>(
        _componentManager->getComponentByType(gridEntity,
            ComponentType::POSITION));

    float startX = posComp ? posComp->x : 0;
    float startY = posComp ? posComp->y : 0;

    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            CellType cellType = grid->getCellType(x, y);
            if (cellType == CellType::FOOD
                || cellType == CellType::POWER_PILL) {
                float xPos = startX + (x * cellSize);
                float yPos = startY + (y * cellSize);
                FoodType foodType = (cellType == CellType::POWER_PILL) ?
                 FoodType::POWER_PILL : FoodType::NORMAL_DOT;
                auto foodEntity = createFood(xPos, yPos, x, y, foodType);
                grid->setEntityAtCell(x, y, foodEntity);
            }
        }
    }
}

void PacmanFactory::createWallEntities(std::shared_ptr<GridComponent> grid,
float cellSize, std::shared_ptr<Arcade::IEntity> gridEntity) {
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
                std::shared_ptr<Arcade::IEntity> wallEntity
                    = createWall(wallX, wallY, x, y);
                grid->setEntityAtCell(x, y, wallEntity);
            }
        }
    }
}

void PacmanFactory::createGhostEntities(std::shared_ptr<GridComponent>
grid, float cellSize, std::shared_ptr<Arcade::IEntity> gridEntity) {
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
        std::shared_ptr<Arcade::IEntity> ghostEntity
            = createGhost(ghostX, ghostY,
            x, y, types[i]);
        grid->setEntityAtCell(x, y, ghostEntity);
    }
}

void PacmanFactory::createPacmanEntity(std::shared_ptr<GridComponent>
grid, float cellSize, std::shared_ptr<Arcade::IEntity> gridEntity) {
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
                std::shared_ptr<Arcade::IEntity> pacmanEntity
                    = createPacman(pacmanX,
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
    std::shared_ptr<Arcade::IEntity> pacmanEntity = createPacman(pacmanX,
        pacmanY, centerX, centerY);
    grid->setEntityAtCell(centerX, centerY, pacmanEntity);
}

std::shared_ptr<IDrawableComponent>
PacmanFactory::getDrawableAsset(const std::string& key) const {
    auto it = _assets.find(key);
    if (it != _assets.end()) {
        auto component = std::make_shared<DrawableComponent>(it->second);
        return component;
    }
    return nullptr;
}

}  // namespace PacMan
}  // namespace Arcade
