// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake factory for creating Snake game instances
*/

#ifndef SRC_GAMES_SNAKE_SNAKEFACTORY_HPP_
#define SRC_GAMES_SNAKE_SNAKEFACTORY_HPP_

#include "Games/Snake/Snake.hpp"

extern "C" {
    Arcade::IGameModule *entryPoint();
    void destroy(Arcade::IGameModule *module);
    Arcade::ModuleInfo module_infos();
}

#endif  // SRC_GAMES_SNAKE_SNAKEFACTORY_HPP_
