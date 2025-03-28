// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Minesweeper game module
*/

#ifndef SRC_GAMES_MINESWEEPER_MINESWEEPER_HPP_
#define SRC_GAMES_MINESWEEPER_MINESWEEPER_HPP_

#include <memory>
#include <vector>
#include <string>
#include "Games/Minesweeper/Board.hpp"
#include "Shared/Interface/Game/IGameModule.hpp"
#include "Shared/Interface/ECS/IEntity.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"
#include "Shared/Models/EventType.hpp"
#include "Shared/Models/KeysType.hpp"

namespace Arcade {

class MinesweeperEntity : public IEntity {
 private:
   int _id;
   int _x;
   int _y;
   char _symbol;

 public:
   MinesweeperEntity(int id, int x, int y, char symbol);

   int getId() const;
   int getX() const;
   int getY() const;
   char getSymbol() const;
};

class MinesweeperGame : public IGameModule {
 public:
   // Fix the constructor declaration
   explicit MinesweeperGame(std::shared_ptr<Arcade::IEventManager> eventManager = nullptr);
   ~MinesweeperGame() override;

   void init() override;
   void update(float deltaTime) override;
   void stop() override;
   
   std::vector<std::shared_ptr<IEntity>> getEntities() const override;
   bool isGameOver() const override;
   bool hasWon() const override;

 private:
   void createBoard();
   void updateEntities();
   void moveCursor(int dx, int dy);
   void revealCell();
   void flagCell();
   int generateEntityId();

   // Fix the event manager member variable
   std::shared_ptr<Arcade::IEventManager> _eventManager;
   bool _gameOver;
   bool _gameWon;
   int _cursorX;
   int _cursorY;
   size_t _width;
   size_t _height;
   int _mineCount;
   int _entityIdCounter;
   float _inputCooldown;
   std::vector<std::shared_ptr<IEntity>> _entities;
   std::unique_ptr<Minesweeper::Board> _board;
};
}  // namespace Arcade

#endif  // SRC_GAMES_MINESWEEPER_MINESWEEPER_HPP_
