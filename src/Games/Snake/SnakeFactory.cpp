// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake factory implementation for creating Snake game instances
*/

#include "Games/Snake/SnakeFactory.hpp"
#include "Shared/Interface/IArcadeModule.hpp"

Arcade::IGameModule *entryPoint() {
    return new Arcade::SnakeGame();
}

void destroy(Arcade::IGameModule *module) {
    delete module;
}

Arcade::ModuleInfo module_infos() {
    Arcade::ModuleInfo infos;
    infos.name = "Snake";
    infos.type = Arcade::ModuleType::GAME;
    return infos;
}
