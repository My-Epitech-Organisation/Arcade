// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Sprite Component header
*/

#ifndef SRC_ECS_COMPONENTS_SPRITE_SPRITECOMPONENT_HPP_
    #define SRC_ECS_COMPONENTS_SPRITE_SPRITECOMPONENT_HPP_
    #include <string>
    #include "Shared/Interface/ECS/IComponent.hpp"
    #include "Shared/Models/ComponentType.hpp"

class SpriteComponent : public Arcade::IComponent {
 public:
    explicit SpriteComponent(const std::string &spritePath) :
        spritePath(spritePath) {}
    ComponentType getType() const override;
    std::string spritePath;
};

#endif  // SRC_ECS_COMPONENTS_SPRITE_SPRITECOMPONENT_HPP_
