// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** System Interface
*/

// Copyright 2025 <Epitech>
/**
 * @file ISystem.hpp
 * @brief Interface for systems in the Entity Component System (ECS) architecture.
 *
 * The ISystem interface defines a standard structure for all systems
 * that operate within the ECS of the Arcade project.
 */

#ifndef SRC_SHARED_INTERFACE_ISYSTEM_HPP_
    #define SRC_SHARED_INTERFACE_ISYSTEM_HPP_

namespace Arcade {
/**
 * @class ISystem
 * @brief Interface for game systems in the ECS.
 *
 * The ISystem interface provides a pure virtual method `update()`,
 * which must be implemented by any system managing game logic or
 * entity behavior.
 *
 * @dot
 * digraph SystemManager {
 *    ISystem -> SystemManager;
 *    SystemManager -> IEventManager;
 *    SystemManager -> EntityManager;
 *    SystemManager -> ComponentManager;
 *    ComponentManager -> PositionComponent;
 *    ComponentManager -> VelocityComponent;
 * }
 * @enddot
 */
class ISystem {
 public:
    /**
     * @brief Virtual destructor.
     *
     * Ensures proper cleanup of derived system instances.
     */
    virtual ~ISystem() = default;

    /**
     * @brief Updates the system.
     *
     * This method is meant to be implemented by derived classes
     * to handle system-specific logic that needs to be executed
     * every update cycle.
     */
    virtual void update() = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ISYSTEM_HPP_
