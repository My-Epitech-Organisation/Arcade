// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade    #define SRC_GAMES_MINESWEEPER_COMPONENTS_BOARD_HPP_
** File description:
** Minesweeper game module implementation
*/
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <algorithm>
#include <utility>
#include <string>
#include "Games/Minesweeper/Minesweeper.hpp"
#include "Shared/Models/KeysType.hpp"
#include "Shared/Models/EventType.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"
#include "Shared/Interface/ECS/IEntity.hpp"
#include "Games/Minesweeper/System/GameLogic.hpp"
#include "Games/Minesweeper/System/RenderSystem.hpp"
#include "Games/Minesweeper/System/EventSubSystem.hpp"
#include "Games/Minesweeper/Components/Board.hpp"
#include "Games/Minesweeper/Components/Cell.hpp"
#include "Games/Minesweeper/Components/Bomb.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Games/Minesweeper/MinesweeperFactory.hpp"
#include "Shared/JSONParser/JSONParser.hpp"
#include "Shared/Models/ModuleInfos.hpp"

namespace Arcade {

MinesweeperGame::~MinesweeperGame() {
    _systems.clear();
    _eventSystem.reset();

    if (_entityManager && _componentManager) {
        auto entities = _entityManager->getEntitiesMap();
        for (const auto& entity : entities) {
            auto components = _componentManager->getEntityComponents(
                entity.first);
            for (const auto& component : components) {
                _componentManager->unregisterComponent(entity.first,
                    typeid(*component).name());
            }

            _entityManager->destroyEntity(entity.first);
        }
    }

    _componentManager.reset();
    _entityManager.reset();
    _eventManager.reset();
}

void MinesweeperGame::init(std::shared_ptr<IEventManager> eventManager,
std::shared_ptr<IComponentManager> componentManager,
std::shared_ptr<IEntityManager> entityManager,
std::shared_ptr<IScoreProvider> scoreProvider) {
    _gameOver = false;
    _gameWon = false;
    _eventManager = eventManager;
    _componentManager = componentManager;
    _entityManager = entityManager;
    _scoreProvider = scoreProvider;

    loadDrawableAssets();
    createBoard();
    _eventSystem = std::make_shared<EventSubSystem>(
        _componentManager, _entityManager, _eventManager, _drawableAssets);
    _systems.push_back(std::make_shared<GameLogic>(_componentManager,
        _entityManager));
    _systems.push_back(std::make_shared<RenderSystem>(_componentManager,
        _entityManager, std::make_pair(10u, 10u)));
    _systems.push_back(std::make_shared<UISystem>(_componentManager,
        _entityManager));

    _systems.push_back(_eventSystem);
}

void MinesweeperGame::loadDrawableAssets() {
    try {
        JSONParser parser;
        _drawableAssets = parser.convertAssetsToGraphicalElements(
            "./config/minesweeper.json");
    } catch (const std::exception& e) {
        std::cerr << "Failed to load Minesweeper assets: "
                  << e.what() << std::endl;
    }
}

std::shared_ptr<IDrawableComponent> MinesweeperGame::getDrawableAsset(
const std::string& key) const {
    auto it = _drawableAssets.find(key);
    if (it != _drawableAssets.end()) {
        auto component = std::make_shared<DrawableComponent>(it->second);
        return component;
    }
    return nullptr;
}

void MinesweeperGame::update(float deltaTime) {
    (void)deltaTime;
    std::shared_ptr<IEntity> boardEntity = nullptr;
    for (const auto &entities : _entityManager->getEntitiesMap()) {
        if (entities.second == "Board") {
            boardEntity = entities.first;
            break;
        }
    }
    if (!boardEntity) return;  // Safety check
    
    auto comp = _componentManager->getComponentByType(boardEntity,
        ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>
        (comp);
    auto statsComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::CUSTOM_BASE);
    auto gameStats = std::dynamic_pointer_cast
        <Arcade::Minesweeper::GameStats>(statsComp);
    
    if (board && gameStats) {
        // If game is not over, update timers and check win/lose conditions
        if (!board->isGameOver()) {
            gameStats->updateTime();

            if (gameStats->isTimeUp()) {
                std::cout << "Game over: Time's up!" << std::endl;
                board->setGameOver(true);
                board->setGameWon(false);
                _gameOver = true;
                _gameWon = false;
            }
            
            // Check for victory
            _gameWon = checkVictory(boardEntity);
            if (_gameWon) {
                std::cout << "Game won!" << std::endl;
                int timeBonus = gameStats->getTimeRemaining() * 10;
                gameStats->addTimeBonus(timeBonus);
                gameStats->updateHighestScore();

                board->setGameWon(true);
                board->setGameOver(true);
                _gameOver = true;
            }
        } else {
            // If game is already over
            _gameOver = true;
            _gameWon = board->isGameWon();
            gameStats->updateHighestScore();
        }
    }

    // Always update all systems
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
    _scoreProvider->setScore(_gameOver ? 0 : getScore());
    _gameOver = true;
    _componentManager.reset();
    _entityManager.reset();
    _systems.clear();
    _eventSystem.reset();
}

extern "C" {
    IArcadeModule* entryPoint(void) {
        try {
            return new MinesweeperGame();
        } catch (const std::exception& e) {
            std::cerr << "Error creating MinesweeperGame instance: "
                << e.what() << std::endl;
            return nullptr;
        }
    }

    void destroy(IGameModule* instance) {
        delete instance;
    }

    Arcade::ModuleInfos module_infos() {
        return {"Minesweeper", "IDK",
            "./assets/textures/module_pictures/sfml.png",
            "./lib/arcade_minesweeper.so", Arcade::ModuleType::GAME};
    }
}

void MinesweeperGame::createBoard() {
    float screenWidth = 800.0f;
    float screenHeight = 600.0f;
    size_t width = 8;
    size_t height = 6;
    float cellSize = 100.0f;
    float boardWidth = width * cellSize;
    float boardHeight = height * cellSize;

    float boardX = (screenWidth - boardWidth) / 2;
    float boardY = (screenHeight - boardHeight) / 2;
    size_t totalCells = width * height;
    size_t mineCount = static_cast<size_t>(totalCells * 0.15);

    Minesweeper::MinesweeperFactory factory(_entityManager, _componentManager);

    std::shared_ptr<IEntity> boardEntity = factory.createBoard(boardX, boardY,
        width, height, mineCount);

    factory.initializeGame(boardEntity, boardX, boardY, cellSize);
}

bool MinesweeperGame::checkVictory(std::shared_ptr<IEntity> boardEntity) {
    auto comp = _componentManager->getComponentByType(boardEntity,
        ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>(comp);
    if (!board) return false;
    
    size_t width = board->getWidth();
    size_t height = board->getHeight();
    size_t totalCells = width * height;
    size_t revealedCells = 0;
    size_t mineCells = 0;
    
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            std::shared_ptr<IEntity> cellEntity = board->getCellEntity(x, y);
            auto comp2 = _componentManager->getComponentByType(cellEntity,
                ComponentType::CELL);
            auto cell = std::dynamic_pointer_cast<Arcade::Minesweeper::Cell>
                (comp2);
            if (!cell) continue;
            
            if (cell->hasMine()) {
                mineCells++;
            } else if (cell->isRevealed()) {
                revealedCells++;
            }
        }
    }
    
    // Victory condition: all non-mine cells revealed
    return revealedCells == (totalCells - mineCells);
}

int MinesweeperGame::getScore() const {
    auto temp = _entityManager->getEntitiesMap();
    std::shared_ptr<Arcade::IEntity> board = nullptr;
    for (auto& temp2 : temp) {
        if (temp2.second == "Board") {
            board = temp2.first;
            break;
        }
    }
    if (!board) {
        return 0;
    }
    auto stat = _componentManager->getComponentByType(board,
        ComponentType::CUSTOM_BASE);
    auto gameStats = std::dynamic_pointer_cast
        <Arcade::Minesweeper::GameStats>(stat);
    if (gameStats)
        return gameStats->getHighestScore();
    return 0;
}

}  // namespace Arcade
