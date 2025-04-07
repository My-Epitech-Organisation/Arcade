// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Food Component
*/

#ifndef SRC_GAMES_PACMAN_COMPONENTS_FOODCOMPONENT_HPP_
#define SRC_GAMES_PACMAN_COMPONENTS_FOODCOMPONENT_HPP_

#include <string>
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Models/ComponentType.hpp"

namespace Arcade {
namespace PacMan {

enum class FoodType {
    NORMAL_DOT,
    POWER_PILL
};

class FoodComponent : public Arcade::IComponent {
public:
    explicit FoodComponent(FoodType type = FoodType::NORMAL_DOT);
    ~FoodComponent() = default;

    ComponentType getType() const override {
        return static_cast<ComponentType>(1003); // Custom component type
    }

    FoodType getFoodType() const { return _foodType; }
    bool isEaten() const { return _eaten; }
    void setEaten(bool eaten) { _eaten = eaten; }
    int getPoints() const;
    void setName(const std::string& name) { _name = name; }
    const std::string& getName() const { return _name; }
    size_t getGridX() const { return _gridX; }
    size_t getGridY() const { return _gridY; }
    void setGridPosition(size_t x, size_t y) { _gridX = x; _gridY = y; }

private:
    std::string _name;
    FoodType _foodType;
    bool _eaten;
    size_t _gridX;
    size_t _gridY;
};

} // namespace PacMan
} // namespace Arcade

#endif // SRC_GAMES_PACMAN_COMPONENTS_FOODCOMPONENT_HPP_