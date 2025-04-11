// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Minesweeper Board class
*/

#ifndef SRC_GAMES_MINESWEEPER_COMPONENTS_BOARD_HPP_
    #define SRC_GAMES_MINESWEEPER_COMPONENTS_BOARD_HPP_
    #include <vector>
    #include <string>
    #include <unordered_map>
    #include <memory>
    #include "Games/Minesweeper/Components/Cell.hpp"
    #include "Shared/Interface/ECS/IComponent.hpp"
    #include "Shared/Interface/ECS/IEntity.hpp"
    #include "ECS/Components/Position/PositionComponent.hpp"

namespace Arcade {
namespace Minesweeper {

class Board : public Arcade::IComponent {
 public:
    Board(size_t width, size_t height, size_t mineCount);
    ~Board() = default;

    void addCellEntity(size_t x, size_t y,
        std::shared_ptr<Arcade::IEntity> cellEntity);
    std::shared_ptr<Arcade::IEntity> getCellEntity(size_t x, size_t y) const;
    void setWidth(size_t width) { _width = width; }
    void setHeight(size_t height) { _height = height; }
    void setMineCount(size_t mineCount) { _mineCount = mineCount; }
    size_t getMineCount() const { return _mineCount; }
    size_t getWidth() const { return _width; }
    size_t getHeight() const { return _height; }
    void setGameOver(bool state) { _gameOver = state; }
    bool isGameOver() const { return _gameOver; }
    void setGameWon(bool state) { _gameWon = state; }
    bool isGameWon() const { return _gameWon; }
    void setInitialized(bool state) { _gameInitialized = state; }
    bool isInitialized() const { return _gameInitialized; }
    void setName(const std::string& name) { this->name = name; }
    const std::string& getName() const { return name; }

    ComponentType getType() const override {
        return ComponentType::BOARD;
    }

 private:
    std::string name;
    size_t _width;
    size_t _height;
    size_t _mineCount;
    bool _gameOver;
    bool _gameWon;
    bool _gameInitialized;
    std::vector<std::vector<std::shared_ptr<Arcade::IEntity>>> _cellEntities;
};

}  // namespace Minesweeper
}  // namespace Arcade

#endif  // SRC_GAMES_MINESWEEPER_COMPONENTS_BOARD_HPP_
