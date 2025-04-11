// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake Component
*/

#ifndef SRC_GAMES_SNAKE_COMPONENTS_SNAKE_HPP_
#define SRC_GAMES_SNAKE_COMPONENTS_SNAKE_HPP_

#include <vector>
#include <memory>
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Interface/ECS/IEntity.hpp"
#include "Games/Snake/Types.hpp"

namespace Arcade {

class SnakeComponent : public IComponent {
 public:
    SnakeComponent() : direction(Direction::RIGHT), shouldGrow(false) {}
    ~SnakeComponent() = default;

    ComponentType getType() const override {
        return ComponentType::CUSTOM_BASE;
    }

    std::vector<Entity> segments;

    Direction direction;

    bool shouldGrow;

    float speedMultiplier = 1.0f;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_COMPONENTS_SNAKE_HPP_
