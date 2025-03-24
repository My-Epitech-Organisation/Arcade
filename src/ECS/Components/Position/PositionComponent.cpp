// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Position Component
*/
#include "Position/PositionComponent.hpp"
#include "Shared/Models/ComponentType.hpp"

ComponentType PositionComponent::getType() const {
    return ComponentType::POSITION;
}
