// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Position Component
*/
/**
 * @file PositionComponent.hpp
 * @brief Defines the PositionComponent structure for spatial representation in an ECS.
 *
 * The PositionComponent is used in an Entity Component System (ECS)
 * to track the position of an entity in a 2D coordinate space.
 */

#ifndef SRC_SHARED_COMPONENTS_POSITION_HPP_
    #define SRC_SHARED_COMPONENTS_POSITION_HPP_

/**
 * @struct PositionComponent
 * @brief Represents the position of an entity in a 2D space.
 *
 * The PositionComponent structure stores the X and Y coordinates
 * of an entity within the game world.
 */
struct PositionComponent {
    int x;  /// The X-coordinate of the entity.
    int y;  /// The Y-coordinate of the entity.

    /**
     * @brief Constructs a PositionComponent with optional initial values.
     *
     * @param x The initial X position (default is 0).
     * @param y The initial Y position (default is 0).
     */
    explicit PositionComponent(int x = 0, int y = 0) : x(x), y(y) {}
};

#endif  // SRC_SHARED_COMPONENTS_POSITION_HPP_
