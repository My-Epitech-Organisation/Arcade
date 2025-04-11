// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Food component
*/

#ifndef SRC_GAMES_SNAKE_COMPONENTS_FOOD_HPP_
#define SRC_GAMES_SNAKE_COMPONENTS_FOOD_HPP_

namespace Arcade {

struct Food {
    enum FoodType {
        APPLE,
        BONUS  // For potential bonus food items
    };
    
    static const size_t APPLE = 200;
    static const size_t BONUS = 201;
    
    int x;
    int y;
    FoodType type;
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_COMPONENTS_FOOD_HPP_