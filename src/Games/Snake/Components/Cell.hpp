// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake Cell component
*/

#ifndef SRC_GAMES_SNAKE_COMPONENTS_CELL_HPP_
#define SRC_GAMES_SNAKE_COMPONENTS_CELL_HPP_

namespace Arcade {

struct Cell {
    enum CellType {
        EMPTY,
        SNAKE_HEAD,
        SNAKE_BODY,
        SNAKE_TAIL,
        FOOD
    };

    CellType type;
    int x;
    int y;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_COMPONENTS_CELL_HPP_