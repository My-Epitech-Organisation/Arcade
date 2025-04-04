// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Sprite Component
*/
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Shared/Models/ComponentType.hpp"

ComponentType SpriteComponent::getType() const {
    return ComponentType::SPRITE;
}

extern "C" {
    __attribute__((constructor))
    const char *init_sprite(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_ncurses(void) {
    }

    Arcade::IArcadeModule* entryPoint(void) {
        return new SpriteComponent("");
    }
}
