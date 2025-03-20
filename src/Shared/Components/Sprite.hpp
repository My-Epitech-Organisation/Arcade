// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Position Component
*/
/**
 * @file SpriteComponent.hpp
 * @brief Defines the SpriteComponent structure for graphical representation in an ECS.
 *
 * The SpriteComponent is used in an Entity Component System (ECS) to associate
 * an entity with a graphical sprite, identified by a file path.
 */

#ifndef SRC_SHARED_COMPONENTS_SPRITE_HPP_
#define SRC_SHARED_COMPONENTS_SPRITE_HPP_
#include <utility>
#include <string>

/**
 * @struct SpriteComponent
 * @brief Represents a graphical sprite associated with an entity.
 *
 * The SpriteComponent stores the file path to an image that is used as
 * the graphical representation of an entity.
 */
struct SpriteComponent {
    std::string path;  /// File path to the sprite image.
    /**
     * @brief Constructs a SpriteComponent with a specified image path.
     *
     * @param path The file path to the sprite image.
     */
    explicit SpriteComponent(std::string path) : path(std::move(path)) {}
};

#endif  // SRC_SHARED_COMPONENTS_SPRITE_HPP_
