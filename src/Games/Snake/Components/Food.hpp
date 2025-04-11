// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Food Component
*/

#ifndef SRC_GAMES_SNAKE_COMPONENTS_FOOD_HPP_
#define SRC_GAMES_SNAKE_COMPONENTS_FOOD_HPP_

#include <memory>
#include "Shared/Interface/ECS/IComponent.hpp"

namespace Arcade {

class FoodComponent : public IComponent {
 public:
    FoodComponent() : eaten(false) {}
    ~FoodComponent() = default;

    ComponentType getType() const override {
        return ComponentType::CUSTOM_BASE;
    }

    bool eaten;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_COMPONENTS_FOOD_HPP_
