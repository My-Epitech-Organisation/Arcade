// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake component
*/

#ifndef SRC_GAMES_SNAKE_COMPONENTS_SNAKE_HPP_
#define SRC_GAMES_SNAKE_COMPONENTS_SNAKE_HPP_

#include <vector>
#include <utility>

namespace Arcade {

struct Snake {
    enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    
    // Sprite types for different parts of the snake
    static const size_t HEAD_UP = 100;
    static const size_t HEAD_DOWN = 101;
    static const size_t HEAD_LEFT = 102;
    static const size_t HEAD_RIGHT = 103;
    static const size_t BODY_HORIZONTAL = 104;
    static const size_t BODY_VERTICAL = 105;
    static const size_t BODY_TOPLEFT = 106;
    static const size_t BODY_TOPRIGHT = 107;
    static const size_t BODY_BOTTOMLEFT = 108;
    static const size_t BODY_BOTTOMRIGHT = 109;
    static const size_t TAIL_UP = 110;
    static const size_t TAIL_DOWN = 111;
    static const size_t TAIL_LEFT = 112;
    static const size_t TAIL_RIGHT = 113;

    // The snake is represented as a list of segments (coordinates)
    // First segment is the head, last segment is the tail
    std::vector<std::pair<int, int>> segments;
    
    Direction direction;
    bool growNextUpdate;  // True if the snake should grow on the next update
    float movementCooldown;  // Time until next movement (seconds)
    float moveSpeed;  // Movement speed (cells per second)
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_COMPONENTS_SNAKE_HPP_