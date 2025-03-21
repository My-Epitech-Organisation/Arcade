// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Position Component header
*/

#ifndef SRC_ECS_COMPONENTS_POSITIONCOMPONENT_HPP_
    #define SRC_ECS_COMPONENTS_POSITIONCOMPONENT_HPP_
    #include <string>
    #include "Shared/Interface/IComponent.hpp"
    #include "Shared/Models/ComponentType.hpp"

class PositionComponent : public IComponent {
 public:
    PositionComponent(float x, float y) : x(x), y(y) {}
    ComponentType getType() const override;
    float x, y;
};

#endif  // SRC_ECS_COMPONENTS_POSITIONCOMPONENT_HPP_
