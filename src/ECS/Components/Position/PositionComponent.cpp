// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Position Component
*/
#include "ECS/Components/Position/PositionComponent.hpp"
#include "Shared/Models/ComponentType.hpp"
#include "Shared/Interface/IArcadeModule.hpp"
#include "Shared/Models/ModuleInfos.hpp"

ComponentType PositionComponent::getType() const {
    return ComponentType::POSITION;
}


