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

void EventSubSystem::handleLeftClick() {
    auto [mouseX, mouseY] = _eventManager->getMousePosition();
    Arcade::Entity boardEntity = 0;
    for (const auto& entity : _entityManager->getEntities()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }
    if (boardEntity == 0) return;

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

    int cellSize = 20;
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

        if (!cell || cell->getState() ==
            Arcade::Minesweeper::Cell::FLAGGED) return;

        cell->setState(Arcade::Minesweeper::Cell::REVEALED);

        auto bombComp = _componentManager->getComponentByType(cellEntity,
            ComponentType::BOMB);
        auto bombComponent =
            std::dynamic_pointer_cast<Arcade::Minesweeper::BombComponent>
                (bombComp);

        if (bombComponent) {
            bombComponent->reveal();

            board->setGameOver(true);
        } else {
            std::string numberSpritePath = "assets/minesweeper/number_" +
                std::to_string(cell->getAdjacentMines()) + ".png";

            auto revealedSprite = std::make_shared<SpriteComponent>
                (numberSpritePath);

            _componentManager->registerComponent(cellEntity, revealedSprite);

            if (cell->getAdjacentMines() == 0) {}
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
    if (boardEntity == 0) return;

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

    int cellSize = 20;
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

        if (cell->getState() == Arcade::Minesweeper::Cell::HIDDEN) {
            cell->setState(Arcade::Minesweeper::Cell::FLAGGED);

            auto flagSprite = std::make_shared<SpriteComponent>
                ("assets/minesweeper/flag.png");
            _componentManager->registerComponent(cellEntity, flagSprite);
        } else if (cell->getState() == Arcade::Minesweeper::Cell::FLAGGED) {
            cell->setState(Arcade::Minesweeper::Cell::HIDDEN);

            auto hiddenSprite = std::make_shared<SpriteComponent>
                ("assets/minesweeper/hidden.png");
            _componentManager->registerComponent(cellEntity, hiddenSprite);
        }
    }
}

void EventSubSystem::handleKeyR() {
    Arcade::Entity boardEntity = 0;
    for (const auto& entity : _entityManager->getEntities()) {
        if (entity.second == "Board") {
            boardEntity = entity.first;
            break;
        }
    }
    if (boardEntity == 0) return;

    auto boardComp = _componentManager->getComponentByType(boardEntity,
        ComponentType::BOARD);
    auto board = std::dynamic_pointer_cast<Arcade::Minesweeper::Board>
        (boardComp);

    if (!board) return;
    board->setGameOver(false);
    board->setGameWon(false);

    for (size_t y = 0; y < board->getHeight(); y++) {
        for (size_t x = 0; x < board->getWidth(); x++) {
            Arcade::Entity cellEntity = board->getCellEntity(x, y);
            if (cellEntity == 0) continue;

            auto cellComp = _componentManager->getComponentByType(cellEntity,
                ComponentType::CELL);
            auto cell = std::dynamic_pointer_cast<Arcade::Minesweeper::Cell>
                (cellComp);

            if (cell) {
                cell->setState(Arcade::Minesweeper::Cell::HIDDEN);
            }

            auto bombComp = _componentManager->getComponentByType(cellEntity,
                ComponentType::BOMB);
            auto bomb = std::dynamic_pointer_cast
                <Arcade::Minesweeper::BombComponent>(bombComp);

            if (bomb) {
                auto hiddenSprite = std::make_shared<SpriteComponent>
                    ("assets/minesweeper/hidden.png");
                auto revealedSprite = std::make_shared<SpriteComponent>
                    ("assets/minesweeper/mine.png");
                auto newBomb =
                    std::make_shared<Arcade::Minesweeper::BombComponent>
                        (hiddenSprite, revealedSprite);
                _componentManager->registerComponent(cellEntity, newBomb);
            }
            auto hiddenSprite = std::make_shared<SpriteComponent>
                ("assets/minesweeper/hidden.png");
            _componentManager->registerComponent(cellEntity, hiddenSprite);
        }
    }
}

void EventSubSystem::update() {
    std::cout << "EventSubSystem update" << std::endl;
}
}  // namespace Arcade
