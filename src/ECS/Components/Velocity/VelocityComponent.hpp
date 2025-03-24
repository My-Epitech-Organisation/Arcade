// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Velocity Component header
*/

#ifndef SRC_ECS_COMPONENTS_VELOCITY_VELOCITYCOMPONENT_HPP_
    #define SRC_ECS_COMPONENTS_VELOCITY_VELOCITYCOMPONENT_HPP_
    #include <string>
    #include "Shared/Interface/ECS/IComponent.hpp"
    #include "Shared/Models/ComponentType.hpp"

class VelocityComponent : public Arcade::IComponent {
 public:
    VelocityComponent(float dx, float dy) : dx(dx), dy(dy) {}
    ComponentType getType() const override;
    float dx, dy;
};

#endif  // SRC_ECS_COMPONENTS_VELOCITY_VELOCITYCOMPONENT_HPP_
