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
#include <map>
#include <vector>
#include <utility>
#include <random>
#include "Games/Minesweeper/System/EventSubSystem.hpp"
#include "Shared/EventManager/KeyEvent/KeyEvent.hpp"
#include "Shared/EventManager/KeyEvent/MouseEvent.hpp"
#include "Shared/Interface/Core/IEvent.hpp"
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
std::shared_ptr<Arcade::IEventManager> eventManager,
const std::map<std::string, DrawableComponent>& _drawableAssets)
: _componentManager(componentManager),
_entityManager(entityManager),
_eventManager(eventManager),
_drawableAssets(_drawableAssets),
_firstClick(true) {
    subscribeToEvents();
}

EventSubSystem::~EventSubSystem() {
    if (_eventManager)
        _eventManager->unsubscribeAll();
}

void EventSubSystem::subscribeToEvents() {
    Arcade::MouseEvent leftClick(Arcade::MouseButton::LEFT,
        Arcade::EventType::MOUSE_BUTTON_PRESSED, 0, 0);
    Arcade::MouseEvent rightClick(Arcade::MouseButton::RIGHT,
        Arcade::EventType::MOUSE_BUTTON_PRESSED, 0, 0);
    Arcade::KeyEvent rKey(Arcade::Keys::R, Arcade::EventType::KEY_PRESSED);
    _eventManager->unsubscribeAll(rKey);
    _eventManager->unsubscribeAll(leftClick);
    _eventManager->unsubscribeAll(rightClick);
    _eventManager->subscribe(leftClick, [this](const IEvent& event) {
        (void)event;
        handleLeftClick();
    });
    _eventManager->subscribe(rightClick, [this](const IEvent& event) {
        (void)event;
        handleRightClick();
    });
    _eventManager->subscribe(rKey, [this](const IEvent& event) {
        (void)event;
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

    std::shared_ptr<Arcade::IEntity> boardEntity = 0;
    for (const auto& entity : _entityManager->getEntitiesMap()) {
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

                    std::shared_ptr<Arcade::IEntity> neighborEntity =
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
                            std::make_shared<Arcade::DrawableComponent>();
                        neighborRevealedSprite->setAsTexture
                            (neighborNumberSpritePath, 100.0f, 100.0f);
                        neighborRevealedSprite->setPosition
                            (static_cast<float>(neighborX * 100),
                                static_cast<float>(neighborY * 100));
                        neighborRevealedSprite->setVisibility(true);
                        _componentManager->registerComponent(neighborEntity,
                            neighborRevealedSprite);
                    } else {
                        auto emptySprite
                            = std::make_shared<Arcade::DrawableComponent>();
                        emptySprite->setAsTexture
                            ("assets/minesweeper/revealed.png", 100.0f, 100.0f);
                        emptySprite->setPosition(
                            static_cast<float>(neighborX * 100),
                                static_cast<float>(neighborY * 100));
                        emptySprite->setVisibility(true);
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

                            auto hiddenAsset = _drawableAssets.find
                                ("cell.revealed");
                            auto mineAsset = _drawableAssets.find("cell.mine");
                            if (hiddenAsset != _drawableAssets.end()
                            && mineAsset != _drawableAssets.end()) {
                                auto hiddenDrawable
                                    = std::make_shared
                                    <Arcade::DrawableComponent>
                                    (hiddenAsset->second);
                                auto revealedDrawable
                                    = std::make_shared
                                    <Arcade::DrawableComponent>
                                    (mineAsset->second);
                                hiddenDrawable->setPosition
                                    (static_cast<float>(sx * 100),
                                        static_cast<float>(sy * 100));
                                revealedDrawable->setPosition
                                    (static_cast<float>(sx * 100),
                                        static_cast<float>(sy * 100));
                                auto bombComponent
                                    = std::make_shared
                                    <Minesweeper::BombComponent>(
                                    hiddenDrawable, revealedDrawable);
                                _componentManager->registerComponent
                                    (safeEntity, bombComponent);
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
    // Find the board entity
    auto [mouseX, mouseY] = _eventManager->getMousePosition();
    std::shared_ptr<Arcade::IEntity> boardEntity = nullptr;
    for (const auto& entity : _entityManager->getEntitiesMap()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }

    // Check if board is in game over state
    auto boardComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>
        (boardComp);
    if (!board) return;
    if (board->isGameOver()) {
        handleKeyR();
        return;
    }
    auto posComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::POSITION);
    auto boardPos = std::dynamic_pointer_cast<PositionComponent>(posComp);

    if (!boardPos) return;
    size_t boardWidth = board->getWidth();
    size_t boardHeight = board->getHeight();

    int cellSize = 100;
    int boardStartX = static_cast<int>(boardPos->x);
    int boardStartY = static_cast<int>(boardPos->y);

    int cellX = (mouseX - boardStartX) / cellSize;
    int cellY = (mouseY - boardStartY) / cellSize;

    if (cellX >= 0 && cellX < static_cast<int>(boardWidth) &&
        cellY >= 0 && cellY < static_cast<int>(boardHeight)) {
        std::shared_ptr<Arcade::IEntity> cellEntity
            = board->getCellEntity(cellX, cellY);
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
        auto existingDrawable = _componentManager->getComponentByType
            (cellEntity,
            ComponentType::DRAWABLE);
        if (existingDrawable) {
            _componentManager->unregisterComponent(cellEntity,
                typeid(*existingDrawable).name());
        }

        auto bombComp = _componentManager->getComponentByType(cellEntity,
            ComponentType::BOMB);
        auto bombComponent =
            std::dynamic_pointer_cast<Arcade::Minesweeper::BombComponent>
                (bombComp);

        if (bombComponent) {
            auto bombSpriteAsset = _drawableAssets.find("cell.mine");
            if (bombSpriteAsset != _drawableAssets.end()) {
                auto bombSprite = std::make_shared<Arcade::DrawableComponent>
                    (bombSpriteAsset->second);
                bombSprite->setVisibility(true);
                bombSprite->setPosition(static_cast<float>(cellX * 100),
                    static_cast<float>(cellY * 100));
                _componentManager->registerComponent(cellEntity, bombSprite);
                bombComponent->reveal();
                board->setGameOver(true);
            }
        } else {
            auto statsComp = _componentManager->getComponentByType(boardEntity,
                ComponentType::CUSTOM_BASE);
            auto gameStats = std::dynamic_pointer_cast
                <Arcade::Minesweeper::GameStats> (statsComp);
            if (gameStats)
                gameStats->revealSafeCell();
            if (cell->getAdjacentMines() > 0) {
                int mineCount = cell->getAdjacentMines();
                std::string assetKey = "cell.numbers["
                    + std::to_string(mineCount - 1) + "]";
                auto numberSpriteAsset = _drawableAssets.find(assetKey);
                if (numberSpriteAsset != _drawableAssets.end()) {
                    auto revealedSprite = std::make_shared
                        <Arcade::DrawableComponent>(numberSpriteAsset->second);
                    revealedSprite->setVisibility(true);
                    revealedSprite->setPosition(static_cast<float>(cellX * 100),
                        static_cast<float>(cellY * 100));
                    _componentManager->registerComponent
                        (cellEntity, revealedSprite);
                }
            } else {
                auto emptySpriteAsset = _drawableAssets.find("cell.revealed");
                if (emptySpriteAsset != _drawableAssets.end()) {
                    auto emptySprite = std::make_shared
                        <Arcade::DrawableComponent>(emptySpriteAsset->second);
                    emptySprite->setVisibility(true);
                    emptySprite->setPosition(static_cast<float>(cellX * 100),
                        static_cast<float>(cellY * 100));
                    _componentManager->registerComponent
                        (cellEntity, emptySprite);
                }
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

    std::shared_ptr<Arcade::IEntity> boardEntity = 0;
    for (const auto& entity : _entityManager->getEntitiesMap()) {
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
        std::shared_ptr<Arcade::IEntity> cellEntity
            = board->getCellEntity(cellX, cellY);
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
        auto existingDrawable = _componentManager->getComponentByType
            (cellEntity,
            ComponentType::DRAWABLE);
        if (existingDrawable) {
            _componentManager->unregisterComponent(cellEntity,
                typeid(*existingDrawable).name());
        }

        if (cell->getState() == Arcade::Minesweeper::Cell::HIDDEN) {
            cell->setState(Arcade::Minesweeper::Cell::FLAGGED);
            auto flagSpriteAsset = _drawableAssets.find("cell.flag");
            if (flagSpriteAsset != _drawableAssets.end()) {
                auto flagSprite = std::make_shared
                    <Arcade::DrawableComponent>(flagSpriteAsset->second);
                flagSprite->setVisibility(true);
                flagSprite->setPosition(static_cast<float>(cellX * 100),
                    static_cast<float>(cellY * 100));
                _componentManager->registerComponent(cellEntity, flagSprite);
                if (gameStats)
                    gameStats->addFlag();
            }
        } else if (cell->getState() == Arcade::Minesweeper::Cell::FLAGGED) {
            cell->setState(Arcade::Minesweeper::Cell::HIDDEN);
            auto hiddenSpriteAsset = _drawableAssets.find("cell.hidden");
            if (hiddenSpriteAsset != _drawableAssets.end()) {
                auto hiddenSprite = std::make_shared
                    <Arcade::DrawableComponent>(hiddenSpriteAsset->second);
                hiddenSprite->setVisibility(true);
                hiddenSprite->setPosition(static_cast<float>(cellX * 100),
                    static_cast<float>(cellY * 100));
                _componentManager->registerComponent(cellEntity, hiddenSprite);
                if (gameStats)
                    gameStats->removeFlag();
            }
        }
    }
}

void EventSubSystem::handleKeyR() {
    _firstClick = true;
    std::shared_ptr<Arcade::IEntity> boardEntity = nullptr;
    for (const auto& entity : _entityManager->getEntitiesMap()) {
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
            std::shared_ptr<Arcade::IEntity> cellEntity
                = board->getCellEntity(x, y);
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

    // Re-subscribe to events after reset
    subscribeToEvents();
}

void EventSubSystem::update() {
    // Check if we need to resubscribe events
    static bool wasGameOver = false;
    std::shared_ptr<Arcade::IEntity> boardEntity = nullptr;
    for (const auto& entity : _entityManager->getEntitiesMap()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }
    if (boardEntity) {
        auto boardComp = _componentManager->getComponentByType(boardEntity,
            ComponentType::BOARD);
        auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>
            (boardComp);
        if (board) {
            bool isGameOver = board->isGameOver();
            if (wasGameOver != isGameOver) {
                wasGameOver = isGameOver;
                if (isGameOver) {
                    Arcade::KeyEvent rKey(Arcade::Keys::R,
                        Arcade::EventType::KEY_PRESSED);
                        _eventManager->subscribe(rKey,
                        [this](const IEvent& event) {
                        (void)event;
                        handleKeyR();
                    });
                } else {
                    // Re-subscribe all events when game is active
                    subscribeToEvents();
                }
            }
        }
    }
}
}  // namespace Arcade
