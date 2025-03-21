// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** MovementSystem
*/
/**
 * @file MovementSystem.hpp
 * @brief System for handling entity movement in an ECS architecture.
 *
 * The MovementSystem class is responsible for updating entity positions based
 * on their velocity components within an Entity Component System (ECS).
 */

#ifndef SRC_SHARED_SYSTEM_MOVEMENTSYSTEM_HPP_
    #define SRC_SHARED_SYSTEM_MOVEMENTSYSTEM_HPP_
    #include "Models/EntityComponentSystem/ComponentManager.hpp"
    #include "Components/Position.hpp"
    #include "Components/Velocity.hpp"

/**
 * @class MovementSystem
 * @brief System responsible for updating entity movement.
 *
 * The MovementSystem class processes all entities that have both
 * Position and Velocity components, updating their positions based
 * on their velocity values.
 */
class MovementSystem {
 public:
    /**
     * @brief Constructs a MovementSystem with component managers.
     *
     * The system requires pointers to the component managers for Position and Velocity.
     *
     * @param pm Pointer to the PositionComponent manager.
     * @param vm Pointer to the VelocityComponent manager.
     */
    MovementSystem(ComponentManager<PositionComponent>* pm,
                ComponentManager<VelocityComponent>* vm)
        : positionManager(pm), velocityManager(vm) {}

    /**
     * @brief Updates entity positions based on velocity.
     *
     * This function iterates through all entities that have both Position
     * and Velocity components and updates their positions accordingly.
     */
    void update();

 private:
    ComponentManager<PositionComponent>* positionManager;
        /// Pointer to the PositionComponent manager.
    ComponentManager<VelocityComponent>* velocityManager;
        /// Pointer to the VelocityComponent manager.
};

#endif  // SRC_SHARED_SYSTEM_MOVEMENTSYSTEM_HPP_
