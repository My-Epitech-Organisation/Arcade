// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Velocity Component implementation
*/

#include "Velocity/VelocityComponent.hpp"
#include "Shared/Models/ComponentType.hpp"

ComponentType VelocityComponent::getType() const {
    return ComponentType::VELOCITY;
}
