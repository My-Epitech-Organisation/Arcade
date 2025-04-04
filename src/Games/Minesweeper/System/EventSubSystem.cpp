// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Event Subsystem for Minesweeper
*/

#include <iostream>
#include <string>
#include <memory>
#include <queue>
#include <set>
#include <vector>
#include <random>
#include "Games/Minesweeper/System/EventSubSystem.hpp"
#include "Shared/EventManager/KeyEvent/KeyEvent.hpp"
#include "Shared/EventManager/KeyEvent/MouseEvent.hpp"
#include "Shared/Models/EventType.hpp"
#include "Shared/Models/KeysType.hpp"
#include "Shared/Models/MouseButtonType.hpp"
#include "Games/Minesweeper/Components/Board.hpp"
#include "Games/Minesweeper/Components/Cell.hpp"
#include "Games/Minesweeper/Components/Bomb.hpp"
#include "Games/Minesweeper/Components/GameStats.hpp"
#include "Games/Minesweeper/MinesweeperFactory.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"

namespace Arcade {
EventSubSystem::EventSubSystem(
std::shared_ptr<Arcade::IComponentManager> componentManager,
std::shared_ptr<Arcade::IEntityManager> entityManager,
std::shared_ptr<Arcade::IEventManager> eventManager)
: _componentManager(componentManager),
_entityManager(entityManager),
_eventManager(eventManager), _firstClick(true) {
    subscribeToEvents();
}

EventSubSystem::~EventSubSystem() {
    if (_eventManager)
        _eventManager->unsubscribeAll();
}

void EventSubSystem::subscribeToEvents() {
    Arcade::MouseEvent leftClick(Arcade::MouseButton::LEFT,
        Arcade::EventType::MOUSE_BUTTON_PRESSED, 0, 0);
    _eventManager->subscribe(leftClick, [this]() {
        handleLeftClick();
    });

    Arcade::MouseEvent rightClick(Arcade::MouseButton::RIGHT,
        Arcade::EventType::MOUSE_BUTTON_PRESSED, 0, 0);
    _eventManager->subscribe(rightClick, [this]() {
        handleRightClick();
    });

    Arcade::KeyEvent rKey(Arcade::Keys::R, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(rKey, [this]() {
        handleKeyR();
    });
}

void EventSubSystem::handleNoAdjacentMine(int cellX, int cellY,
size_t boardWidth, size_t boardHeight,
std::shared_ptr<Arcade::Minesweeper::Board> board) {
    std::queue<std::pair<int, int>> cellsToReveal;

    std::set<std::pair<int, int>> processedCells;

    cellsToReveal.push({cellX, cellY});
    processedCells.insert({cellX, cellY});

    Arcade::Entity boardEntity = 0;
    for (const auto& entity : _entityManager->getEntities()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }

    auto statsComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::CUSTOM_BASE);
    auto gameStats = std::dynamic_pointer_cast<Arcade::Minesweeper::GameStats>
        (statsComp);

    while (!cellsToReveal.empty()) {
        auto [currentX, currentY] = cellsToReveal.front();
        cellsToReveal.pop();

        for (int offsetY = -1; offsetY <= 1; offsetY++) {
            for (int offsetX = -1; offsetX <= 1; offsetX++) {
                if (offsetX == 0 && offsetY == 0) continue;

                int neighborX = currentX + offsetX;
                int neighborY = currentY + offsetY;

                if (neighborX >= 0 && neighborX <
                    static_cast<int>(boardWidth) &&
                    neighborY >= 0 && neighborY
                    < static_cast<int>(boardHeight)) {
                    if (processedCells.count({neighborX,
                        neighborY}) > 0) continue;

                    Arcade::Entity neighborEntity =
                        board->getCellEntity(neighborX, neighborY);
                    if (neighborEntity == 0) continue;

                    auto neighborComp = _componentManager->getComponentByType(
                        neighborEntity, ComponentType::CELL);
                    auto neighborCell = std::dynamic_pointer_cast<
                        Arcade::Minesweeper::Cell>
                        (neighborComp);

                    if (!neighborCell || neighborCell->getState()
                        != Arcade::Minesweeper::Cell::HIDDEN ||
                        neighborCell->hasMine()) continue;

                    neighborCell->setState(
                        Arcade::Minesweeper::Cell::REVEALED);
                    if (gameStats)
                        gameStats->revealSafeCell();

                    if (neighborCell->getAdjacentMines() > 0) {
                        std::string neighborNumberSpritePath =
                            "assets/minesweeper/number_" +
                            std::to_string(neighborCell->getAdjacentMines())
                            + ".png";

                        auto neighborRevealedSprite =
                            std::make_shared<SpriteComponent>
                            (neighborNumberSpritePath);

                        _componentManager->registerComponent(neighborEntity,
                            neighborRevealedSprite);
                    } else {
                        auto emptySprite = std::make_shared<SpriteComponent>
                            ("assets/minesweeper/revealed.png");
                        _componentManager->registerComponent(neighborEntity,
                            emptySprite);
                    }

                    processedCells.insert({neighborX, neighborY});

                    if (neighborCell->getAdjacentMines() == 0)
                        cellsToReveal.push({neighborX, neighborY});
                }
            }
        }
    }
}

void EventSubSystem::handleFirstClick(size_t boardHeight, size_t boardWidth,
int cellX, int cellY, std::shared_ptr<Arcade::Minesweeper::Board> board) {
    std::vector<std::pair<size_t, size_t>> mineCells;
    std::vector<std::pair<size_t, size_t>> safeCells;

    for (size_t y = 0; y < boardHeight; y++) {
        for (size_t x = 0; x < boardWidth; x++) {
            if (abs(static_cast<int>(x) - cellX) <= 1 &&
                abs(static_cast<int>(y) - cellY) <= 1) {
                continue;
            }

            auto currentEntity = board->getCellEntity(x, y);
            auto currentComp = _componentManager->getComponentByType(
                currentEntity, ComponentType::CELL);
            auto currentCell = std::dynamic_pointer_cast
                <Arcade::Minesweeper::Cell> (currentComp);

            if (currentCell && currentCell->hasMine())
                mineCells.push_back({x, y});
            else
                safeCells.push_back({x, y});
        }
    }

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int nx = cellX + dx;
            int ny = cellY + dy;

            if (nx >= 0 && nx < static_cast<int>(boardWidth) &&
                ny >= 0 && ny < static_cast<int>(boardHeight)) {
                auto neighborEntity = board->getCellEntity(nx, ny);
                auto neighborComp = _componentManager->getComponentByType(
                    neighborEntity, ComponentType::CELL);
                auto neighborCell = std::dynamic_pointer_cast
                    <Arcade::Minesweeper::Cell> (neighborComp);

                if (neighborCell && neighborCell->hasMine()) {
                    neighborCell->setHasMine(false);

                    auto bombComp = _componentManager->getComponentByType(
                        neighborEntity, ComponentType::BOMB);
                    if (bombComp) {
                        _componentManager->unregisterComponent(
                            neighborEntity, typeid(*bombComp).name());
                    }

                    if (!safeCells.empty()) {
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<size_t>
                            dist(0, safeCells.size() - 1);
                        size_t randomIndex = dist(gen);

                        auto [sx, sy] = safeCells[randomIndex];
                        auto safeEntity = board->getCellEntity(sx, sy);
                        auto safeComp = _componentManager->getComponentByType(
                            safeEntity, ComponentType::CELL);
                        auto safeCell = std::dynamic_pointer_cast
                            <Arcade::Minesweeper::Cell> (safeComp);

                        if (safeCell) {
                            safeCell->setHasMine(true);

                            auto spriteComp = _componentManager->
                                getComponentByType(safeEntity,
                                    ComponentType::SPRITE);
                            auto hiddenSprite = std::dynamic_pointer_cast
                                <SpriteComponent> (spriteComp);

                            if (hiddenSprite) {
                                auto revealedSprite = std::make_shared
                                    <SpriteComponent> (
                                        "assets/minesweeper/mine.png");

                                auto bombComponent = std::make_shared
                                    <Minesweeper::BombComponent>
                                        (hiddenSprite, revealedSprite);
                                _componentManager->registerComponent(
                                    safeEntity, bombComponent);
                            }
                        }

                        safeCells.erase(safeCells.begin() + randomIndex);
                    }
                }
            }
        }
    }

    for (size_t y = 0; y < boardHeight; y++) {
        for (size_t x = 0; x < boardWidth; x++) {
            auto currentEntity = board->getCellEntity(x, y);
            auto currentComp = _componentManager->getComponentByType(
                currentEntity, ComponentType::CELL);
            auto currentCell = std::dynamic_pointer_cast
                <Arcade::Minesweeper::Cell> (currentComp);

            if (currentCell && !currentCell->hasMine()) {
                size_t adjacentMines = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue;

                        int nx = static_cast<int>(x) + dx;
                        int ny = static_cast<int>(y) + dy;

                        if (nx >= 0 && nx < static_cast<int>(boardWidth) &&
                            ny >= 0 && ny < static_cast<int>(boardHeight)) {
                            auto neighEntity = board->getCellEntity(nx, ny);
                            auto neighComp = _componentManager->
                                getComponentByType(neighEntity,
                                    ComponentType::CELL);
                            auto neighCell = std::dynamic_pointer_cast
                                <Arcade::Minesweeper::Cell>(neighComp);

                            if (neighCell && neighCell->hasMine())
                                adjacentMines++;
                        }
                    }
                }

                currentCell->setAdjacentMines(adjacentMines);
            }
        }
    }
}

void EventSubSystem::handleLeftClick() {
    auto [mouseX, mouseY] = _eventManager->getMousePosition();
    Arcade::Entity boardEntity = 0;
    for (const auto& entity : _entityManager->getEntities()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }
    auto boardComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>
        (boardComp);

    auto posComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::POSITION);
    auto boardPos = std::dynamic_pointer_cast<PositionComponent>(posComp);

    if (!board || !boardPos) return;
    size_t boardWidth = board->getWidth();
    size_t boardHeight = board->getHeight();

    int cellSize = 100;
    int boardStartX = static_cast<int>(boardPos->x);
    int boardStartY = static_cast<int>(boardPos->y);

    int cellX = (mouseX - boardStartX) / cellSize;
    int cellY = (mouseY - boardStartY) / cellSize;

    if (cellX >= 0 && cellX < static_cast<int>(boardWidth) &&
        cellY >= 0 && cellY < static_cast<int>(boardHeight)) {
        Arcade::Entity cellEntity = board->getCellEntity(cellX, cellY);
        if (cellEntity == 0) return;

        auto cellComp = _componentManager->getComponentByType(cellEntity,
            ComponentType::CELL);
        auto cell = std::dynamic_pointer_cast<Arcade::Minesweeper::Cell>
            (cellComp);

        if (!cell ||
            cell->getState() == Arcade::Minesweeper::Cell::FLAGGED ||
            cell->getState() == Arcade::Minesweeper::Cell::REVEALED) return;

        if (_firstClick) {
            _firstClick = false;
            if (cell->hasMine() || cell->getAdjacentMines() > 0)
                handleFirstClick(boardHeight, boardWidth, cellX, cellY, board);
        }

        cell->setState(Arcade::Minesweeper::Cell::REVEALED);

        auto bombComp = _componentManager->getComponentByType(cellEntity,
            ComponentType::BOMB);
        auto bombComponent =
            std::dynamic_pointer_cast<Arcade::Minesweeper::BombComponent>
                (bombComp);

        if (bombComponent) {
            std::string bombsprtPath = "assets/minesweeper/mine.png";
            auto bombSprite = std::make_shared<SpriteComponent>
                (bombsprtPath);
            _componentManager->registerComponent(cellEntity,
                bombSprite);
            bombComponent->reveal();
            board->setGameOver(true);
        } else {
            auto statsComp = _componentManager->getComponentByType(boardEntity,
                ComponentType::CUSTOM_BASE);
            auto gameStats = std::dynamic_pointer_cast
                <Arcade::Minesweeper::GameStats> (statsComp);
            if (gameStats)
                gameStats->revealSafeCell();
            if (cell->getAdjacentMines() > 0) {
                std::string numberSpritePath = "assets/minesweeper/number_" +
                    std::to_string(cell->getAdjacentMines()) + ".png";

                auto revealedSprite = std::make_shared<SpriteComponent>
                    (numberSpritePath);

                _componentManager->registerComponent(cellEntity,
                    revealedSprite);
            } else {
                auto emptySprite = std::make_shared<SpriteComponent>
                    ("assets/minesweeper/revealed.png");
                _componentManager->registerComponent(cellEntity, emptySprite);
            }

            if (cell->getAdjacentMines() == 0) {
                handleNoAdjacentMine(cellX, cellY, boardWidth,
                    boardHeight, board);
            }
        }
    }
}

void EventSubSystem::handleRightClick() {
    auto [mouseX, mouseY] = _eventManager->getMousePosition();

    Arcade::Entity boardEntity = 0;
    for (const auto& entity : _entityManager->getEntities()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }

    auto boardComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>
        (boardComp);

    auto posComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::POSITION);
    auto boardPos = std::dynamic_pointer_cast<PositionComponent>
        (posComp);

    if (!board || !boardPos) return;
    size_t boardWidth = board->getWidth();
    size_t boardHeight = board->getHeight();

    int cellSize = 100;
    int boardStartX = static_cast<int>(boardPos->x);
    int boardStartY = static_cast<int>(boardPos->y);
    int cellX = (mouseX - boardStartX) / cellSize;
    int cellY = (mouseY - boardStartY) / cellSize;
    if (cellX >= 0 && cellX < static_cast<int>(boardWidth) &&
        cellY >= 0 && cellY < static_cast<int>(boardHeight)) {
        Arcade::Entity cellEntity = board->getCellEntity(cellX, cellY);
        if (cellEntity == 0) return;

        auto cellComp = _componentManager->getComponentByType(cellEntity,
            ComponentType::CELL);
        auto cell = std::dynamic_pointer_cast<Arcade::Minesweeper::Cell>
            (cellComp);

        if (!cell) return;

        auto statsComp = _componentManager->getComponentByType(boardEntity,
            ComponentType::CUSTOM_BASE);
        auto gameStats = std::dynamic_pointer_cast
            <Arcade::Minesweeper::GameStats> (statsComp);

        if (cell->getState() == Arcade::Minesweeper::Cell::HIDDEN) {
            cell->setState(Arcade::Minesweeper::Cell::FLAGGED);

            auto flagSprite = std::make_shared<SpriteComponent>
                ("assets/minesweeper/flag.png");
            _componentManager->registerComponent(cellEntity, flagSprite);

            if (gameStats)
                gameStats->addFlag();
        } else if (cell->getState() == Arcade::Minesweeper::Cell::FLAGGED) {
            cell->setState(Arcade::Minesweeper::Cell::HIDDEN);

            auto hiddenSprite = std::make_shared<SpriteComponent>
                ("assets/minesweeper/hidden.png");
            _componentManager->registerComponent(cellEntity, hiddenSprite);

            if (gameStats)
                gameStats->removeFlag();
        }
    }
}

void EventSubSystem::handleKeyR() {
    _firstClick = true;
    Arcade::Entity boardEntity = -1;
    for (const auto& entity : _entityManager->getEntities()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }

    auto boardComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>
        (boardComp);

    if (!board) return;
    board->setGameOver(false);
    board->setGameWon(false);

    auto posComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::POSITION);
    auto boardPos = std::dynamic_pointer_cast<PositionComponent>(posComp);

    if (!boardPos) return;

    float boardX = boardPos->x;
    float boardY = boardPos->y;
    size_t width = board->getWidth();
    size_t height = board->getHeight();
    size_t mineCount = board->getMineCount();
    int cellSize = 100;

    auto statsComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::CUSTOM_BASE);
    auto gameStats = std::dynamic_pointer_cast<Arcade::Minesweeper::GameStats>
        (statsComp);
    if (gameStats)
        gameStats->reset();

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            Arcade::Entity cellEntity = board->getCellEntity(x, y);
            if (cellEntity == 0) continue;

            auto components = _componentManager->getEntityComponents
                (cellEntity);
            for (const auto& component : components) {
                _componentManager->unregisterComponent(cellEntity,
                    typeid(*component).name());
            }

            _entityManager->destroyEntity(cellEntity);
        }
    }

    Arcade::Minesweeper::MinesweeperFactory factory(_entityManager,
        _componentManager);
    factory.initializeGame(boardEntity, boardX, boardY, cellSize);
}

void EventSubSystem::update() {
    // std::cout << "EventSubSystem update" << std::endl;
}
}  // namespace Arcade
