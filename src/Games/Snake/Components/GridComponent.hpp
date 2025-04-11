// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Grid Component for Snake
*/

#ifndef SRC_GAMES_SNAKE_COMPONENTS_GRID_GRIDCOMPONENT_HPP_
#define SRC_GAMES_SNAKE_COMPONENTS_GRID_GRIDCOMPONENT_HPP_

#include <vector>
#include <string>
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Models/ComponentType.hpp"
#include "Shared/Models/EntityType.hpp"

namespace Arcade {

enum class SnakeCellType {
    EMPTY,
    WALL,
    FOOD,
    SNAKE_HEAD,
    SNAKE_BODY,
    SNAKE_TAIL
};

class GridComponent : public Arcade::IComponent {
 public:
    GridComponent(size_t width, size_t height);
    ~GridComponent() = default;

    ComponentType getType() const override {
        return static_cast<ComponentType>(1000);
    }

    void setCellType(size_t x, size_t y, SnakeCellType type);
    SnakeCellType getCellType(size_t x, size_t y) const;
    void setEntityAtCell(size_t x, size_t y, Arcade::Entity entity);
    Arcade::Entity getEntityAtCell(size_t x, size_t y) const;
    size_t getWidth() const { return _width; }
    size_t getHeight() const { return _height; }
    void setName(const std::string& name) { _name = name; }
    const std::string& getName() const { return _name; }
    void setGameOver(bool state) { _gameOver = state; }
    bool isGameOver() const { return _gameOver; }
    void setGameWon(bool state) { _gameWon = state; }
    bool isGameWon() const { return _gameWon; }
    void setCellSize(float cellSize) { _cellSize = cellSize; }
    float getCellSize() const { return _cellSize; }
    void initializeEmptyGrid();
    bool isCellValid(size_t x, size_t y) const;
    void clearCell(size_t x, size_t y);

 private:
    std::string _name;
    size_t _width;
    size_t _height;
    bool _gameOver;
    bool _gameWon;
    std::vector<std::vector<SnakeCellType>> _grid;
    std::vector<std::vector<Arcade::Entity>> _entityGrid;
    float _cellSize;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_COMPONENTS_GRID_GRIDCOMPONENT_HPP_