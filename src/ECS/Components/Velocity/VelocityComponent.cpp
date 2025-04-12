// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Velocity Component implementation
*/

#include "ECS/Components/Velocity/VelocityComponent.hpp"
#include "Shared/Models/ComponentType.hpp"
#include "Shared/Models/ModuleInfos.hpp"

ComponentType VelocityComponent::getType() const {
    return ComponentType::VELOCITY;
}

