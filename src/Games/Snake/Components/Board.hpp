// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake Board component
*/

#ifndef SRC_GAMES_SNAKE_COMPONENTS_BOARD_HPP_
#define SRC_GAMES_SNAKE_COMPONENTS_BOARD_HPP_

namespace Arcade {

struct Board {
    Board() : width(20), height(20), cyclical(false) {}
    Board(int w, int h, bool cycl = false) : width(w), height(h), cyclical(cycl) {}

    int width;
    int height;
    bool cyclical;  // If true, snake can wrap around the board edges
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_COMPONENTS_BOARD_HPP_