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

ComponentType PositionComponent::getType() const {
    return ComponentType::POSITION;
}

extern "C" {
    __attribute__((constructor))
    const char *init_position(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_ncurses(void) {
    }

    Arcade::IArcadeModule* entryPoint(void) {
        return new PositionComponent(0, 0);
    }
}