// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** TextComponent
*/
#include "ECS/Components/Text/TextComponent.hpp"
#include "Shared/Models/ComponentType.hpp"
#include "Shared/Models/ModuleInfos.hpp"

ComponentType TextComponent::getType() const {
    return ComponentType::TEXT;
}
