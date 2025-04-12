// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Component Type enum
*/

#ifndef SRC_SHARED_MODELS_COMPONENTTYPE_HPP_
    #define SRC_SHARED_MODELS_COMPONENTTYPE_HPP_

enum class ComponentType {
    POSITION,
    VELOCITY,
    BOMB,
    CELL,
    BOARD,
    TEXT,
    DRAWABLE,
    COLLIDER,
    ANIMATION,
    CONSUMABLE,
    INPUT,
    TEXTURE,
    BUTTON,
    SNAKE,
    SNAKE_HEAD,
    SNAKE_SEGMENT,
    SHIP,
    ENEMIES,
    WALL,
    FIREBALL,
    SHIP_MISSILES,
    CUBES,
    CUSTOM_BASE = 1000
};

#endif  // SRC_SHARED_MODELS_COMPONENTTYPE_HPP_
