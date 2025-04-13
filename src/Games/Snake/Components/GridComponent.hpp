// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Grid Component header for Snake game
*/

#ifndef SRC_GAMES_SNAKE_COMPONENTS_GRIDCOMPONENT_HPP_
#define SRC_GAMES_SNAKE_COMPONENTS_GRIDCOMPONENT_HPP_

#include <string>
#include <vector>
#include <utility>
#include <random>
#include "Shared/Interface/ECS/IComponent.hpp"

namespace Arcade {
namespace Snake {

/**
 * @enum CellType
 * @brief Types of cells in the Snake grid
 */
enum class CellType {
    EMPTY,       // Empty cell
    SNAKE_HEAD,  // Snake head
    SNAKE_BODY,  // Snake body segment
    FOOD,        // Food
    WALL         // Wall
};

/**
 * @class GridComponent
 * @brief Component representing the game grid for Snake
 */
class GridComponent : public Arcade::IComponent {
 public:
    /**
     * @brief Constructor
     * @param width Grid width
     * @param height Grid height
     */
    GridComponent(size_t width, size_t height);

    /**
     * @brief Destructor
     */
    ~GridComponent() = default;

    /**
     * @brief Gets component type
     * @return Component type
     */
    ComponentType getType() const override {
        return static_cast<ComponentType>(1000);  // Custom component type
    }

    /**
     * @brief Gets grid width
     * @return Width
     */
    size_t getWidth() const { return _width; }

    /**
     * @brief Gets grid height
     * @return Height
     */
    size_t getHeight() const { return _height; }

    /**
     * @brief Gets cell size
     * @return Cell size
     */
    float getCellSize() const { return _cellSize; }

    /**
     * @brief Sets cell size
     * @param size Cell size
     */
    void setCellSize(float size) { _cellSize = size; }

    /**
     * @brief Checks if grid is cyclical (wraps around edges)
     * @return True if grid is cyclical
     */
    bool isCyclical() const { return _cyclical; }

    /**
     * @brief Sets whether grid is cyclical
     * @param cyclical Whether grid is cyclical
     */
    void setCyclical(bool cyclical) { _cyclical = cyclical; }

    /**
     * @brief Gets cell type at position
     * @param x X position
     * @param y Y position
     * @return Cell type
     */
    CellType getCellType(size_t x, size_t y) const;

    /**
     * @brief Sets cell type at position
     * @param x X position
     * @param y Y position
     * @param type Cell type
     */
    void setCellType(size_t x, size_t y, CellType type);

    /**
     * @brief Sets the grid name
     * @param name Name
     */
    void setName(const std::string& name) { _name = name; }

    /**
     * @brief Gets the grid name
     * @return Name
     */
    const std::string& getName() const { return _name; }

    /**
     * @brief Finds an entity at a cell position
     * @param x X position
     * @param y Y position
     * @return Entity ID
     */
    Arcade::Entity getEntityAtCell(size_t x, size_t y) const;

    /**
     * @brief Sets an entity at a cell position
     * @param x X position
     * @param y Y position
     * @param entity Entity ID
     */
    void setEntityAtCell(size_t x, size_t y, Arcade::Entity entity);

    /**
     * @brief Checks if game is over
     * @return True if game is over
     */
    bool isGameOver() const { return _gameOver; }

    /**
     * @brief Sets game over state
     * @param gameOver Game over state
     */
    void setGameOver(bool gameOver) { _gameOver = gameOver; }

    /**
     * @brief Checks if player has won
     * @return True if player has won
     */
    bool isGameWon() const { return _gameWon; }

    /**
     * @brief Sets game won state
     * @param gameWon Game won state
     */
    void setGameWon(bool gameWon) { _gameWon = gameWon; }

    /**
     * @brief Places food at a random empty position
     * @return Position of placed food (x, y)
     */
    std::pair<size_t, size_t> placeFood();

    /**
     * @brief Resets the grid
     */
    void resetGrid();

 private:
    std::string _name;
    size_t _width;
    size_t _height;
    float _cellSize;
    bool _gameOver;
    bool _gameWon;
    bool _cyclical = true;
    std::vector<std::vector<CellType>> _grid;
    std::vector<std::vector<Arcade::Entity>> _entities;
    std::mt19937 _rng;
};

}  // namespace Snake
}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_COMPONENTS_GRIDCOMPONENT_HPP_
