// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Food Component header
*/

#ifndef SRC_GAMES_SNAKE_COMPONENTS_FOODCOMPONENT_HPP_
#define SRC_GAMES_SNAKE_COMPONENTS_FOODCOMPONENT_HPP_

#include <string>
#include "Shared/Interface/ECS/IComponent.hpp"

namespace Arcade {
namespace Snake {

/**
 * @enum FoodType
 * @brief Types of food
 */
enum class FoodType {
    REGULAR,  // Regular food worth 1 point
    BONUS     // Bonus food worth more points
};

/**
 * @class FoodComponent
 * @brief Component representing food in the snake game
 */
class FoodComponent : public Arcade::IComponent {
 public:
    /**
     * @brief Constructor
     * @param type Food type
     */
    explicit FoodComponent(FoodType type = FoodType::REGULAR);

    /**
     * @brief Destructor
     */
    ~FoodComponent() = default;

    /**
     * @brief Gets component type
     * @return Component type
     */
    ComponentType getType() const override {
        return static_cast<ComponentType>(1002);  // Custom component type matching GameLogic.cpp
    }

    /**
     * @brief Gets food type
     * @return Food type
     */
    FoodType getFoodType() const { return _foodType; }

    /**
     * @brief Sets food type
     * @param type Food type
     */
    void setFoodType(FoodType type) { _foodType = type; }

    /**
     * @brief Gets food value
     * @return Value in points
     */
    int getValue() const;

    /**
     * @brief Gets point value of the food
     * @return Point value
     */
    int getPointValue() const { return getValue(); }

    /**
     * @brief Updates the remaining time for bonus food
     * @param deltaTime Time elapsed since last update
     * @return True if food should be removed (timeout)
     */
    bool updateTimeRemaining(float deltaTime) {
        if (_foodType != FoodType::BONUS)
            return false;

        _timeRemaining -= deltaTime;
        return _timeRemaining <= 0.0f;
    }

    /**
     * @brief Sets the food name
     * @param name Name
     */
    void setName(const std::string& name) { _name = name; }

    /**
     * @brief Gets the food name
     * @return Name
     */
    const std::string& getName() const { return _name; }

    /**
     * @brief Gets the X position in the grid
     * @return X position
     */
    size_t getGridX() const { return _gridX; }

    /**
     * @brief Gets the Y position in the grid
     * @return Y position
     */
    size_t getGridY() const { return _gridY; }

    /**
     * @brief Sets the grid position
     * @param x X position
     * @param y Y position
     */
    void setGridPosition(size_t x, size_t y) { _gridX = x; _gridY = y; }

 private:
    std::string _name;
    FoodType _foodType;
    size_t _gridX;
    size_t _gridY;
    float _timeRemaining = 10.0f; // Default 10 seconds for bonus food
};

}  // namespace Snake
}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_COMPONENTS_FOODCOMPONENT_HPP_