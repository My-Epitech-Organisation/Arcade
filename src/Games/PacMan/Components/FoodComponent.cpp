// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Food Component implementation
*/

#include "Games/PacMan/Components/FoodComponent.hpp"

namespace Arcade {
namespace PacMan {

FoodComponent::FoodComponent(FoodType type)
: _foodType(type), _eaten(false), _gridX(0), _gridY(0) {
}

int FoodComponent::getPoints() const {
    // Use direct return values instead of switch for this simple case
    return (_foodType == FoodType::NORMAL_DOT) ? 10 : 
           (_foodType == FoodType::POWER_PILL) ? 50 : 0;
}

}  // namespace PacMan
}  // namespace Arcade
