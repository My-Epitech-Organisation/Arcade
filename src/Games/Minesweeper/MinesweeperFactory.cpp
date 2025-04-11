// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** MinesweeperFactory
*/

#include <random>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include <iostream>
#include <utility>
#include "Games/Minesweeper/MinesweeperFactory.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "Shared/JSONParser/JSONParser.hpp"

namespace Arcade {
std::shared_ptr<IEntity>
Minesweeper::MinesweeperFactory::createBoard(float x, float y,
size_t width, size_t height, size_t mineCount) {
    std::shared_ptr<IEntity> boardEntity
    = _entityManager->createEntity("Board");
    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(boardEntity, positionComponent);
    auto boardComponent = std::make_shared<Board>(width, height, mineCount);
    boardComponent->setName("Board");
    _componentManager->registerComponent(boardEntity, boardComponent);
    auto gameStats = std::make_shared<GameStats>(mineCount, 300);
    _componentManager->registerComponent(boardEntity, gameStats);
    return boardEntity;
}

std::shared_ptr<IEntity>
Minesweeper::MinesweeperFactory::createCell(float x, float y,
size_t gridX, size_t gridY, bool hasMine) {
    std::string cellName = "Cell_" + std::to_string(gridX) +
        "_" + std::to_string(gridY);
    std::shared_ptr<IEntity> cellEntity
        = _entityManager->createEntity(cellName);

    auto positionComponent = std::make_shared<PositionComponent>(x, y);
    _componentManager->registerComponent(cellEntity, positionComponent);

    auto cellComponent = std::make_shared<Cell>(hasMine, 0);
    cellComponent->setName(cellName);
    cellComponent->_gridX = gridX;
    cellComponent->_gridY = gridY;
    _componentManager->registerComponent(cellEntity, cellComponent);

    auto drawable = std::make_shared<DrawableComponent>();
    drawable->setAsTexture("assets/minesweeper/hidden.png", 100.0f, 100.0f);
    drawable->posX = x;
    drawable->posY = y;
    drawable->isVisible = true;
    _componentManager->registerComponent(cellEntity, drawable);

    if (hasMine) {
        auto bombComponent = std::make_shared<BombComponent>(
            "assets/minesweeper/hidden.png",
            "assets/minesweeper/mine.png");
        _componentManager->registerComponent(cellEntity, bombComponent);
    }
    return cellEntity;
}

void Minesweeper::MinesweeperFactory::initializeGame(
std::shared_ptr<IEntity> boardEntity, float startX,
float startY, float cellSize) {
    auto component = _componentManager->getComponentByType(boardEntity,
        ComponentType::BOARD);
    auto boardComponent = std::dynamic_pointer_cast<Board>(component);
    if (!boardComponent) return;
    size_t width = boardComponent->getWidth();
    size_t height = boardComponent->getHeight();
    size_t mineCount = boardComponent->getMineCount();
    JSONParser parser;
    auto assets = parser.convertAssetsToGraphicalElements(
        "./config/minesweeper.json");
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            float cellX = startX + (x * cellSize);
            float cellY = startY + (y * cellSize);
            bool hasMine = false;

            std::shared_ptr<IEntity> cellEntity
                = createCell(cellX, cellY, x, y, hasMine);
            boardComponent->addCellEntity(x, y, cellEntity);
        }
    }
    std::vector<std::pair<size_t, size_t>> positions;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            positions.emplace_back(x, y);
        }
    }
    std::shuffle(positions.begin(), positions.end(),
        std::default_random_engine(std::random_device {}()));

    for (size_t i = 0; i < mineCount && i < positions.size(); i++) {
        auto [x, y] = positions[i];
        auto cellEntity = boardComponent->getCellEntity(x, y);
        auto component = _componentManager->getComponentByType(cellEntity,
            ComponentType::CELL);
        auto cellComponent = std::dynamic_pointer_cast<Cell>(component);
        if (cellComponent) {
            cellComponent->setHasMine(true);

            auto drawableComp = _componentManager->getComponentByType
                (cellEntity, ComponentType::DRAWABLE);
            auto hiddenDrawable = std::dynamic_pointer_cast<DrawableComponent>
                (drawableComp);

            if (hiddenDrawable) {
                auto revealedDrawable = std::make_shared<DrawableComponent>();
                revealedDrawable->setAsTexture("assets/minesweeper/mine.png",
                    100.0f, 100.0f);
                revealedDrawable->posX = hiddenDrawable->posX;
                revealedDrawable->posY = hiddenDrawable->posY;

                auto bombComponent = std::make_shared<BombComponent>
                    (hiddenDrawable, revealedDrawable);
                _componentManager->registerComponent(cellEntity, bombComponent);
            }
        }
    }
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            auto cellEntity = boardComponent->getCellEntity(x, y);
            auto component = _componentManager->getComponentByType(cellEntity,
                ComponentType::CELL);
            auto cellComponent = std::dynamic_pointer_cast<Cell>(component);
            if (!cellComponent || cellComponent->hasMine()) continue;

            size_t adjacentMines = 0;
            for (int offsetY = -1; offsetY <= 1; offsetY++) {
                for (int offsetX = -1; offsetX <= 1; offsetX++) {
                    if (offsetX == 0 && offsetY == 0) continue;

                    size_t neighborX = x + offsetX;
                    size_t neighborY = y + offsetY;

                    if (neighborX < width && neighborY < height) {
                        auto neighborEntity = boardComponent->getCellEntity(
                            neighborX, neighborY);
                        auto neiComp = _componentManager->getComponentByType(
                            neighborEntity, ComponentType::CELL);
                        auto neighborComponent =
                            std::dynamic_pointer_cast<Cell>(neiComp);
                        if (neighborComponent && neighborComponent->hasMine())
                            adjacentMines++;
                    }
                }
            }
            cellComponent->setAdjacentMines(adjacentMines);
        }
    }
}


}  // namespace Arcade
