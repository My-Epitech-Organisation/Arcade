// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Velocity Component implementation
*/

#include "ECS/Components/Velocity/VelocityComponent.hpp"
#include "Shared/Models/ComponentType.hpp"

ComponentType VelocityComponent::getType() const {
    return ComponentType::VELOCITY;
}

extern "C" {
    __attribute__((constructor))
    const char *init_velocity(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_ncurses(void) {
    }

    Arcade::IArcadeModule* entryPoint(void) {
        return new VelocityComponent(0, 0);
    }
}
