// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Food Component implementation
*/

#include <iostream>
#include "Games/Snake/Components/FoodComponent.hpp"

namespace Arcade {
namespace Snake {

FoodComponent::FoodComponent(FoodType type)
: _foodType(type), _gridX(0), _gridY(0) {
}

int FoodComponent::getValue() const {
    // Bonus food is worth 5 points, regular food is worth 1 point
    return _foodType == FoodType::BONUS ? 5 : 1;
}

}  // namespace Snake
}  // namespace Arcade
