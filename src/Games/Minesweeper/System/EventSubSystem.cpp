/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Event Subsystem for Minesweeper
*/

#include "Games/Minesweeper/System/EventSubSystem.hpp"
#include "Shared/EventManager/KeyEvent/KeyEvent.hpp"
#include "Shared/EventManager/KeyEvent/MouseEvent.hpp"
#include "Shared/Models/EventType.hpp"
#include "Shared/Models/KeysType.hpp"
#include "Shared/Models/MouseButtonType.hpp"
#include "Games/Minesweeper/Components/Board.hpp"
#include "Games/Minesweeper/Components/Cell.hpp"
#include "Games/Minesweeper/Components/Bomb.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Shared/Models/EventType.hpp"
#include <iostream>

namespace Arcade {
EventSubSystem::EventSubSystem(
    std::shared_ptr<Arcade::IComponentManager> componentManager,
    std::shared_ptr<Arcade::IEntityManager> entityManager,
    std::shared_ptr<Arcade::IEventManager> eventManager)
    : _componentManager(componentManager),
      _entityManager(entityManager),
      _eventManager(eventManager) {
    subscribeToEvents();
}

void EventSubSystem::subscribeToEvents() {
    Arcade::MouseEvent leftClick(Arcade::MouseButton::LEFT,
        Arcade::EventType::MOUSE_BUTTON_PRESSED, 0, 0);
    _eventManager->subscribe(leftClick, [this]() {
        handleLeftClick();
    });

    // Subscribe to right mouse click for flagging cells
    Arcade::MouseEvent rightClick(Arcade::MouseButton::RIGHT,
        Arcade::EventType::MOUSE_BUTTON_PRESSED, 0, 0);
    _eventManager->subscribe(rightClick, [this]() {
        handleRightClick();
    });

    // Subscribe to R key for restarting the game
    Arcade::KeyEvent rKey(Arcade::Keys::R, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(rKey, [this]() {
        handleKeyR();
    });
}

void EventSubSystem::handleLeftClick() {
    auto [mouseX, mouseY] = _eventManager->getMousePosition();
    // Find the board entity
    Arcade::Entity boardEntity = 0;
    for (const auto& entity : _entityManager->getEntities()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }
    if (boardEntity == 0) return;
    
    // Get board component and position
    auto boardComp = _componentManager->getComponentByType(boardEntity, ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>(boardComp);
    
    auto posComp = _componentManager->getComponentByType(boardEntity, ComponentType::POSITION);
    auto boardPos = std::dynamic_pointer_cast<PositionComponent>(posComp);
    
    if (!board || !boardPos) return;
    size_t boardWidth = board->getWidth();
    size_t boardHeight = board->getHeight();
    // Calculate board position using the PositionComponent
    int cellSize = 20; // Could be a property of the board
    int boardStartX = static_cast<int>(boardPos->x);
    int boardStartY = static_cast<int>(boardPos->y);
    // Convert mouse position to cell coordnates
    int cellX = (mouseX - boardStartX) / cellSize;
    int cellY = (mouseY - boardStartY) / cellSize;
    // Ensure click is within bounds
    if (cellX >= 0 && cellX < static_cast<int>(boardWidth) &&
        cellY >= 0 && cellY < static_cast<int>(boardHeight)) {
        // Get the cell entity directly from the board
        Arcade::Entity cellEntity = board->getCellEntity(cellX, cellY);
        if (cellEntity == 0) return;
        
        // Get the cell component
        auto cellComp = _componentManager->getComponentByType(cellEntity, ComponentType::CELL);
        auto cell = std::dynamic_pointer_cast<Arcade::Minesweeper::Cell>(cellComp);
        
        if (!cell || cell->getState() == Arcade::Minesweeper::Cell::FLAGGED) return;
        // Reveal the cell
        cell->setState(Arcade::Minesweeper::Cell::REVEALED);
        
        // Check if this is a bomb cell
        auto bombComp = _componentManager->getComponentByType(cellEntity, ComponentType::BOMB);
        auto bombComponent = std::dynamic_pointer_cast<Arcade::Minesweeper::BombComponent>(bombComp);
        
        if (bombComponent) {
            // Reveal the bomb
            bombComponent->reveal();
            // Update sprite to the revealed bomb sprite
            // The sprite should automatically update through the BombComponent's getActiveSprite method
            // Set game over state
            board->setGameOver(true);
        } else {
            // For non-bomb cells, update to the appropriate number sprite based on adjacent mines
            std::string numberSpritePath = "assets/minesweeper/number_" +
                std::to_string(cell->getAdjacentMines()) + ".png";
            // Create a new sprite component for the revealed number
            auto revealedSprite = std::make_shared<SpriteComponent>(numberSpritePath);
            // Remove old sprite and add new one
            // Note: This assumes you have a way to identify and remove the old sprite
            // You might need a different approach depending on your component system
            _componentManager->registerComponent(cellEntity, revealedSprite);
            // If cell has no adjacent mines, recursively reveal neighbors
            if (cell->getAdjacentMines() == 0) {
                // Implement flood fill algorithm to reveal neighboring cells
                // This would be a separate method like revealAdjacentCells(board, cellX, cellY)
            }
        }
    }
}

void EventSubSystem::handleRightClick() {
    auto [mouseX, mouseY] = _eventManager->getMousePosition();
    // Find the board entity
    Arcade::Entity boardEntity = 0;
    for (const auto& entity : _entityManager->getEntities()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }
    if (boardEntity == 0) return;
    
    // Get board component and position
    auto boardComp = _componentManager->getComponentByType(boardEntity, ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>(boardComp);
    
    auto posComp = _componentManager->getComponentByType(boardEntity, ComponentType::POSITION);
    auto boardPos = std::dynamic_pointer_cast<PositionComponent>(posComp);
    
    if (!board || !boardPos) return;
    size_t boardWidth = board->getWidth();
    size_t boardHeight = board->getHeight();
    // Calculate board position using the PositionComponent
    int cellSize = 20;
    int boardStartX = static_cast<int>(boardPos->x);
    int boardStartY = static_cast<int>(boardPos->y);
    int cellX = (mouseX - boardStartX) / cellSize;
    int cellY = (mouseY - boardStartY) / cellSize;
    if (cellX >= 0 && cellX < static_cast<int>(boardWidth) &&
        cellY >= 0 && cellY < static_cast<int>(boardHeight)) {
        // Get the cell entity directly from the board
        Arcade::Entity cellEntity = board->getCellEntity(cellX, cellY);
        if (cellEntity == 0) return;
        
        // Get the cell component
        auto cellComp = _componentManager->getComponentByType(cellEntity, ComponentType::CELL);
        auto cell = std::dynamic_pointer_cast<Arcade::Minesweeper::Cell>(cellComp);
        
        if (!cell) return;
        // Toggle flag state
        if (cell->getState() == Arcade::Minesweeper::Cell::HIDDEN) {
            cell->setState(Arcade::Minesweeper::Cell::FLAGGED);
            // Update sprte to flag
            auto flagSprite = std::make_shared<SpriteComponent>("assets/minesweeper/flag.png");
            _componentManager->registerComponent(cellEntity, flagSprite);
        } else if (cell->getState() == Arcade::Minesweeper::Cell::FLAGGED) {
            cell->setState(Arcade::Minesweeper::Cell::HIDDEN);
            // Revert to hidden sprite
            auto hiddenSprite = std::make_shared<SpriteComponent>("assets/minesweeper/hidden.png");
            _componentManager->registerComponent(cellEntity, hiddenSprite);
        }
    }
}

void EventSubSystem::handleKeyR() {
    // Find the board entity
    Arcade::Entity boardEntity = 0;
    for (const auto& entity : _entityManager->getEntities()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }
    if (boardEntity == 0) return;
    
    // Reset the board state
    auto boardComp = _componentManager->getComponentByType(boardEntity, ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>(boardComp);
    
    if (!board) return;
    board->setGameOver(false);
    board->setGameWon(false);
    // Reset all cells using the board's cell entity references
    for (size_t y = 0; y < board->getHeight(); y++) {
        for (size_t x = 0; x < board->getWidth(); x++) {
            Arcade::Entity cellEntity = board->getCellEntity(x, y);
            if (cellEntity == 0) continue;
            
            // Reset cell state
            auto cellComp = _componentManager->getComponentByType(cellEntity, ComponentType::CELL);
            auto cell = std::dynamic_pointer_cast<Arcade::Minesweeper::Cell>(cellComp);
            
            if (cell) {
                cell->setState(Arcade::Minesweeper::Cell::HIDDEN);
            }
            
            // Reset bomb component if present
            auto bombComp = _componentManager->getComponentByType(cellEntity, ComponentType::BOMB);
            auto bomb = std::dynamic_pointer_cast<Arcade::Minesweeper::BombComponent>(bombComp);
            
            if (bomb) {
                // Reset revealed state in the BombComponent
                // This assumes you have a method to reset the bomb
                // You might need to add a reset() method to BombComponent
                // Alternative: replace the component with a new one
                auto hiddenSprite = std::make_shared<SpriteComponent>("assets/minesweeper/hidden.png");
                auto revealedSprite = std::make_shared<SpriteComponent>("assets/minesweeper/mine.png");
                auto newBomb = std::make_shared<Arcade::Minesweeper::BombComponent>(hiddenSprite, revealedSprite);
                _componentManager->registerComponent(cellEntity, newBomb);
            }
            // Reset sprite to hidden
            auto hiddenSprite = std::make_shared<SpriteComponent>("assets/minesweeper/hidden.png");
            _componentManager->registerComponent(cellEntity, hiddenSprite);
        }
    }
    // Re-randomize mine positions if needed
    // This would be a separate method in your game factory or board initializer
}

void EventSubSystem::update() {
    std::cout << "EventSubSystem update" << std::endl;
}
}  // namespace Arcade