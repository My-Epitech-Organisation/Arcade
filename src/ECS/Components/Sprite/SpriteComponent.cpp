// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Sprite Component
*/
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Shared/Models/ComponentType.hpp"
#include "Shared/Models/ModuleInfos.hpp"

ComponentType SpriteComponent::getType() const {
    return ComponentType::TEXTURE;
}
