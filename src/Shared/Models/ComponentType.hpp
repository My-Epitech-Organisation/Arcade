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
    SPRITE,
    BOMB,
    CELL,
    BOARD,
    TEXT,
    CUSTOM_BASE = 1000
};

#endif  // SRC_SHARED_MODELS_COMPONENTTYPE_HPP_
