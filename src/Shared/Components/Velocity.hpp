// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Position Component
*/
/**
 * @file VelocityComponent.hpp
 * @brief Defines the VelocityComponent structure for movement in an ECS.
 *
 * The VelocityComponent is used in an Entity Component System (ECS)
 * to represent an entity's movement speed along the X and Y axes.
 */

#ifndef SRC_SHARED_COMPONENTS_VELOCITY_HPP_
    #define SRC_SHARED_COMPONENTS_VELOCITY_HPP_

/**
 * @struct VelocityComponent
 * @brief Represents an entity's velocity in 2D space.
 *
 * The VelocityComponent structure defines movement speed in the X and Y directions.
 * It is used to update an entity's position within the game world.
 */
struct VelocityComponent {
    int dx;  /// Change in position along the X-axis.
    int dy;  /// Change in position along the Y-axis.

    /**
     * @brief Constructs a VelocityComponent with optional initial values.
     *
     * @param dx The initial velocity along the X-axis (default is 0).
     * @param dy The initial velocity along the Y-axis (default is 0).
     */
    explicit VelocityComponent(int dx = 0, int dy = 0) : dx(dx), dy(dy) {}
};

#endif  // SRC_SHARED_COMPONENTS_VELOCITY_HPP_
