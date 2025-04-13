// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Food Component implementation
*/

#include <iostream>
#include "Games/PacMan/Components/FoodComponent.hpp"

namespace Arcade {
namespace PacMan {

FoodComponent::FoodComponent(FoodType type)
: _foodType(type), _eaten(false), _gridX(0), _gridY(0) {
    // Add debug output when creating a power pill
    if (type == FoodType::POWER_PILL) {
        std::cout << "Created a POWER_PILL food component" << std::endl;
    }
}

int FoodComponent::getPoints() const {
    // Replace conditional operators with a clearer switch statement
    switch (_foodType) {
        case FoodType::NORMAL_DOT:
            return 10;
        case FoodType::POWER_PILL:
            // Debug output to verify this is called
            std::cout << "Power pill points requested: 50" << std::endl;
            return 50;
        default:
            return 0;
    }
}

}  // namespace PacMan
}  // namespace Arcade
