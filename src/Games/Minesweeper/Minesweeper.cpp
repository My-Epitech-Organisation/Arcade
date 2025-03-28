// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Minesweeper game module implementation
*/
#include <iostream>
#include "Games/Minesweeper/Minesweeper.hpp"
#include "Shared/Models/KeysType.hpp"
#include "Shared/Models/EventType.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"

namespace Arcade {

    MinesweeperEntity::MinesweeperEntity(int id, int x, int y, char symbol)
    : _id(id), _x(x), _y(y), _symbol(symbol) {}

int MinesweeperEntity::getId() const {
    return _id;
}

int MinesweeperEntity::getX() const {
    return _x;
}

int MinesweeperEntity::getY() const {
    return _y;
}

char MinesweeperEntity::getSymbol() const {
    return _symbol;
}

MinesweeperGame::MinesweeperGame(std::shared_ptr<Arcade::IEventManager> eventManager)
    : _eventManager(eventManager), _gameOver(false), _gameWon(false),
      _cursorX(0), _cursorY(0), _width(15), _height(15),
      _mineCount(30), _entityIdCounter(0), _inputCooldown(0.0f) {
    // Constructor implementation
}

MinesweeperGame::~MinesweeperGame() {
    stop();
}

void MinesweeperGame::init() {
    _gameOver = false;
    _gameWon = false;
    _cursorX = 0;
    _cursorY = 0;
    _entityIdCounter = 0;
    _entities.clear();
    _inputCooldown = 0.0f;

    createBoard();
    updateEntities();

    if (_eventManager) {
        KeyEvent upevent(Keys::UP, EventType::KEY_PRESSED);
        _eventManager->subscribe(upevent, [this]() {
            moveCursor(0, -1);
            return true;
        });

        KeyEvent downevent(Keys::DOWN, EventType::KEY_PRESSED);
        _eventManager->subscribe(downevent, [this]() {
            moveCursor(0, 1);
            return true;
        });
        
        KeyEvent leftevent(Keys::LEFT, EventType::KEY_PRESSED);
        _eventManager->subscribe(leftevent, [this]() {
            moveCursor(-1, 0);
            return true;
        });
        
        KeyEvent rightevent(Keys::RIGHT, EventType::KEY_PRESSED);
        _eventManager->subscribe(rightevent, [this]() {
            moveCursor(1, 0);
            return true;
        });
        
        KeyEvent spaceevent(Keys::SPACE, EventType::KEY_PRESSED);
        _eventManager->subscribe(spaceevent, [this]() {
            revealCell();
            return true;
        });
        
        KeyEvent fevent(Keys::F, EventType::KEY_PRESSED);
        _eventManager->subscribe(fevent, [this]() {
            flagCell();
            return true;
        });

        KeyEvent qevent(Keys::Q, EventType::KEY_PRESSED);
        _eventManager->subscribe(qevent, [this]() {
            _gameOver = true;
            return true;
        });

        KeyEvent revent(Keys::R, EventType::KEY_PRESSED);
        _eventManager->subscribe(revent, [this]() {
            stop();
            init();
            return true;
        });
    }
}

void MinesweeperGame::update(float deltaTime) {
    if (_inputCooldown > 0) {
        _inputCooldown -= deltaTime;
    }
    
    // We don't need to check for key presses here anymore as they're handled by event callbacks
    
    if (_board && _board->isGameOver()) {
        _gameOver = true;
    }

    if (_board && _board->hasWon()) {
        _gameWon = true;
    }

    // Always refresh entities
    updateEntities();
}

void MinesweeperGame::stop() {
    _entities.clear();
    _board.reset();
}

std::vector<std::shared_ptr<IEntity>> MinesweeperGame::getEntities() const {
    return _entities;
}

bool MinesweeperGame::isGameOver() const {
    return _gameOver;
}

bool MinesweeperGame::hasWon() const {
    return _gameWon;
}

void MinesweeperGame::createBoard() {
    _board = std::make_unique<Minesweeper::Board>(_width, _height, _mineCount);
    _board->initialize();
}

void MinesweeperGame::updateEntities() {
    _entities.clear();

    // Create entities for each cell on the board
    if (_board) {
        for (size_t y = 0; y < _board->getHeight(); ++y) {
            for (size_t x = 0; x < _board->getWidth(); ++x) {
                const Minesweeper::Cell& cell = _board->getCell(x, y);
                char symbol = cell.getDisplayChar();
                
                auto entity = std::make_shared<MinesweeperEntity>(
                    generateEntityId(), 
                    static_cast<int>(x), 
                    static_cast<int>(y), 
                    symbol);
                
                _entities.push_back(entity);
            }
        }
    }

    // Create cursor entity (shown with different character to distinguish it)
    // Use a higher Z value to ensure the cursor is rendered on top
    auto cursorEntity = std::make_shared<MinesweeperEntity>(
        generateEntityId(), _cursorX, _cursorY, '@');
    
    _entities.push_back(cursorEntity);

    // Add game status entities if needed
    if (_gameOver) {
        auto gameOverEntity = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 1, _height + 1, 'G');
        _entities.push_back(gameOverEntity);
        
        auto gameOverEntity2 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 2, _height + 1, 'A');
        _entities.push_back(gameOverEntity2);
        
        auto gameOverEntity3 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 3, _height + 1, 'M');
        _entities.push_back(gameOverEntity3);
        
        auto gameOverEntity4 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 4, _height + 1, 'E');
        _entities.push_back(gameOverEntity4);
        
        auto gameOverEntity5 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 5, _height + 1, ' ');
        _entities.push_back(gameOverEntity5);
        
        auto gameOverEntity6 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 6, _height + 1, 'O');
        _entities.push_back(gameOverEntity6);
        
        auto gameOverEntity7 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 7, _height + 1, 'V');
        _entities.push_back(gameOverEntity7);
        
        auto gameOverEntity8 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 8, _height + 1, 'E');
        _entities.push_back(gameOverEntity8);
        
        auto gameOverEntity9 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 9, _height + 1, 'R');
        _entities.push_back(gameOverEntity9);
    } else if (_gameWon) {
        auto winEntity = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 1, _height + 1, 'Y');
        _entities.push_back(winEntity);
        
        auto winEntity2 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 2, _height + 1, 'O');
        _entities.push_back(winEntity2);
        
        auto winEntity3 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 3, _height + 1, 'U');
        _entities.push_back(winEntity3);
        
        auto winEntity4 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 4, _height + 1, ' ');
        _entities.push_back(winEntity4);
        
        auto winEntity5 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 5, _height + 1, 'W');
        _entities.push_back(winEntity5);
        
        auto winEntity6 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 6, _height + 1, 'I');
        _entities.push_back(winEntity6);
        
        auto winEntity7 = std::make_shared<MinesweeperEntity>(
            generateEntityId(), 7, _height + 1, 'N');
        _entities.push_back(winEntity7);
    }
}

void MinesweeperGame::moveCursor(int dx, int dy) {
    if (_inputCooldown <= 0) {
        int newX = _cursorX + dx;
        int newY = _cursorY + dy;

        if (newX >= 0 && static_cast<size_t>(newX) < _board->getWidth() &&
            newY >= 0 && static_cast<size_t>(newY) < _board->getHeight()) {
            _cursorX = newX;
            _cursorY = newY;
            _inputCooldown = 0.15f; // Cooldown to prevent too rapid movement
        }
    }
}

void MinesweeperGame::revealCell() {
    if (_inputCooldown <= 0 && !_gameOver && !_gameWon) {
        _board->revealCell(_cursorX, _cursorY);
        _inputCooldown = 0.2f; // Cooldown to prevent accidental double-clicks
    }
}

void MinesweeperGame::flagCell() {
    if (_inputCooldown <= 0 && !_gameOver && !_gameWon) {
        _board->flagCell(_cursorX, _cursorY);
        _inputCooldown = 0.2f;
    }
}

int MinesweeperGame::generateEntityId() {
    return _entityIdCounter++;
}

extern "C" {
    Arcade::IGameModule* entryPoint(std::shared_ptr<Arcade::IEventManager> eventManager) {
        return new Arcade::MinesweeperGame(eventManager);
    }

    void destroy(Arcade::IGameModule* instance) {
        delete instance;
    }
}

}  // namespace Arcade
