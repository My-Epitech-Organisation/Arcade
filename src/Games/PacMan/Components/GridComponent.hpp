// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Grid Component for Pacman
*/

#ifndef SRC_GAMES_PACMAN_COMPONENTS_GRIDCOMPONENT_HPP_
#define SRC_GAMES_PACMAN_COMPONENTS_GRIDCOMPONENT_HPP_

#include <vector>
#include <string>
#include <unordered_map>
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Models/ComponentType.hpp"
#include "Shared/Models/EntityType.hpp"

namespace Arcade {
namespace PacMan {

enum class CellType {
    EMPTY,
    WALL,
    FOOD,
    POWER_PILL,
    PACMAN_SPAWN,
    GHOST_SPAWN
};

class GridComponent : public Arcade::IComponent {
public:
    GridComponent(size_t width, size_t height);
    ~GridComponent() = default;

    ComponentType getType() const override {
        return static_cast<ComponentType>(1000); // Custom component type
    }

    void setCellType(size_t x, size_t y, CellType type);
    CellType getCellType(size_t x, size_t y) const;
    void setEntityAtCell(size_t x, size_t y, Arcade::Entity entity);
    Arcade::Entity getEntityAtCell(size_t x, size_t y) const;
    size_t getWidth() const { return _width; }
    size_t getHeight() const { return _height; }
    void setName(const std::string& name) { _name = name; }
    const std::string& getName() const { return _name; }
    size_t getFoodCount() const { return _foodCount; }
    void setFoodCount(size_t count) { _foodCount = count; }
    void decrementFoodCount() { if (_foodCount > 0) _foodCount--; }
    void setGameOver(bool state) { _gameOver = state; }
    bool isGameOver() const { return _gameOver; }
    void setGameWon(bool state) { _gameWon = state; }
    bool isGameWon() const { return _gameWon; }
    void loadDefaultMap();

private:
    std::string _name;
    size_t _width;
    size_t _height;
    size_t _foodCount;
    bool _gameOver;
    bool _gameWon;
    std::vector<std::vector<CellType>> _grid;
    std::vector<std::vector<Arcade::Entity>> _entityGrid;
};

} // namespace PacMan
} // namespace Arcade

#endif // SRC_GAMES_PACMAN_COMPONENTS_GRIDCOMPONENT_HPP_