// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Minesweeper game module implementation
*/
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <algorithm>
#include "Games/Minesweeper/Minesweeper.hpp"
#include "Shared/Models/KeysType.hpp"
#include "Shared/Models/EventType.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"
#include "Games/Minesweeper/System/GameLogic.hpp"
#include "Games/Minesweeper/System/RenderSystem.hpp"
#include "Games/Minesweeper/System/EventSubSystem.hpp"
#include "Games/Minesweeper/Components/Board.hpp"
#include "Games/Minesweeper/Components/Cell.hpp"
#include "Games/Minesweeper/Components/Bomb.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Games/Minesweeper/MinesweeperFactory.hpp"

namespace Arcade {

void MinesweeperGame::init(std::shared_ptr<IEventManager> eventManager,
    std::shared_ptr<IComponentManager> componentManager,
    std::shared_ptr<IEntityManager> entityManager) {
    _gameOver = false;
    _gameWon = false;
    _eventManager = eventManager;
    _componentManager = componentManager;
    _entityManager = entityManager;

    createBoard();
    // Initialize the event system first
    _eventSystem = std::make_shared<EventSubSystem>(
        _componentManager, _entityManager, _eventManager);
    // Add other systems
    _systems.push_back(std::make_shared<GameLogic>(_componentManager, _entityManager));
    _systems.push_back(std::make_shared<RenderSystem>(_componentManager, _entityManager,
                                                        std::make_pair(10u, 10u))); // Add board dimensions
    _systems.push_back(_eventSystem);
}

void MinesweeperGame::update() {
    // Check for victory condition
    Arcade::Entity boardEntity = 0;
    for (const auto &entities: _entityManager->getEntities()) {
        if (entities.second == "Board") {
            boardEntity = entities.first;
            break;
        }
    }
    if (boardEntity != 0) {
        auto comp = _componentManager->getComponentByType(boardEntity, ComponentType::BOARD);
        auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>(comp);
        if (board && !board->isGameOver()) {
            _gameWon = checkVictory(boardEntity);
            if (_gameWon) {
                board->setGameWon(true);
            }
        }
        // Update game over status
        if (board && board->isGameOver()) {
            _gameOver = true;
        }
    }
    // Update all systems
    for (const auto& system : _systems) {
        system->update();
    }
}

bool MinesweeperGame::isGameOver() const {
    return _gameOver;
}

bool MinesweeperGame::hasWon() const {
    return _gameWon;
}

void MinesweeperGame::stop() {
    _gameOver = true;
}

extern "C" {
    // Inside the extern "C" block
    IArcadeModule* entryPoint(void) {
        try {
            return new MinesweeperGame();
        } catch (const std::exception& e) {
            std::cerr << "Error creating MinesweeperGame instance: " << e.what() << std::endl;
            return nullptr;
        }
    }
    void destroy(IGameModule* instance) {
        delete instance;
    }
}

void MinesweeperGame::createBoard() {
    // Board dimensions and properties
    size_t width = 10;
    size_t height = 10;
    size_t mineCount = 10;
    float cellSize = 20.0f;
    // Board position (centered on screen with some offset)
    float screenWidth = 800.0f;  // Assume default screen size
    float screenHeight = 600.0f;
    float boardWidth = width * cellSize;
    float boardHeight = height * cellSize;
    float boardX = (screenWidth - boardWidth) / 2;
    float boardY = (screenHeight - boardHeight) / 2;
    // Use our factory to create the board and all cells
    Minesweeper::MinesweeperFactory factory(_entityManager, _componentManager);
    // Create board entity with position
    Arcade::Entity boardEntity = factory.createBoard(boardX, boardY, width, height, mineCount);
    // Initialize the game board with cells
    factory.initializeGame(boardEntity, boardX, boardY, cellSize);
}

bool MinesweeperGame::checkVictory(Arcade::Entity boardEntity) {
    auto comp = _componentManager->getComponentByType(boardEntity, ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>(comp);
    if (!board) return false;
    size_t width = board->getWidth();
    size_t height = board->getHeight();
    // Check if all non-mine cells are revealed
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            Arcade::Entity cellEntity = board->getCellEntity(x, y);
            auto comp2 = _componentManager->getComponentByType(cellEntity, ComponentType::CELL);
            auto cell = std::dynamic_pointer_cast<Arcade::Minesweeper::Cell>(comp2);
            if (!cell) continue;
            // If a non-mine cell is not revealed, game is not won yet
            if (!cell->isMine() && cell->getState() != Arcade::Minesweeper::Cell::REVEALED) {
                return false;
            }
        }
    }
    // All non-mine cells are revealed, victory!
    return true;
}

std::string MinesweeperGame::getSpecialCompSprite(size_t id) const {
    // Check for BombComponent
    auto component = _componentManager->getComponentByType(id, ComponentType::BOMB);
    auto bombComponent = std::dynamic_pointer_cast<Arcade::Minesweeper::BombComponent>(component);
    if (bombComponent) {
        // Return the appropriate sprite path based on bomb state
        if (bombComponent->isRevealed()) {
            return "assets/minesweeper/mine.png";  // Revealed bomb sprite
        } else {
            return "assets/minesweeper/hidden.png";  // Hidden bomb sprite
        }
    }
    auto comp2 = _componentManager->getComponentByType(id, ComponentType::CELL);
    auto cellComponent = std::dynamic_pointer_cast<Arcade::Minesweeper::Cell>(comp2);
    if (cellComponent) {
        // Return appropriate cell sprite based on state and adjacent mines
        if (cellComponent->getState() == Arcade::Minesweeper::Cell::FLAGGED) {
            return "assets/minesweeper/flag.png";
        } else if (cellComponent->getState() == Arcade::Minesweeper::Cell::REVEALED && !cellComponent->isMine()) {
            // For revealed non-mine cells, show number of adjacent mines
            int adjacentMines = cellComponent->getAdjacentMines();
            if (adjacentMines > 0) {
                return "assets/minesweeper/number_" + std::to_string(adjacentMines) + ".png";
            } else {
                return "assets/minesweeper/empty.png";  // No adjacent mines
            }
        } else if (cellComponent->getState() == Arcade::Minesweeper::Cell::HIDDEN) {
            return "assets/minesweeper/hidden.png";
        }
    }
    // If no special component is found or no special rendering is needed, return empty string
    return "";
}
}  // namespace Arcade