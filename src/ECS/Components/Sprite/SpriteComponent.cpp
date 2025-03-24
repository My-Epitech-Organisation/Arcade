// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Sprite Component
*/
#include "Sprite/SpriteComponent.hpp"
#include "Shared/Models/ComponentType.hpp"

ComponentType SpriteComponent::getType() const {
    return ComponentType::SPRITE;
}
