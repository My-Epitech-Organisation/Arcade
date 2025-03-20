// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Component Manager
*/

// Copyright 2025 <Epitech>
/**
 * @file ComponentManager.hpp
 * @brief Manages components for entities in an Entity Component System (ECS).
 *
 * The ComponentManager class is a template-based system that allows
 * entities to have associated components, enabling efficient storage
 * and retrieval within an ECS architecture.
 */

#ifndef SRC_SHARED_MODELS_ENTITYCOMPONENTSYSTEM_COMPONENTMANAGER_HPP_
    #define SRC_SHARED_MODELS_ENTITYCOMPONENTSYSTEM_COMPONENTMANAGER_HPP_
    #include <unordered_map>
    #include "Models/EntityComponentSystem/EntityManager.hpp"

/**
 * @class ComponentManager
 * @brief Manages components of a specific type for entities.
 *
 * The ComponentManager class is responsible for storing, retrieving,
 * and removing components associated with entities in an ECS.
 * It uses an unordered map to efficiently store components.
 *
 * @tparam T The type of component managed by this class.
 */
template <typename T>
class ComponentManager {
 private:
    std::unordered_map<Entity, T> components;
        /// Maps entities to their corresponding components.

 public:
    /**
     * @brief Adds a component to an entity.
     *
     * Associates the specified component with the given entity.
     *
     * @param entity The entity to which the component is assigned.
     * @param component The component to be added.
     */
    void addComponent(Entity entity, T component);

    /**
     * @brief Removes a component from an entity.
     *
     * Deletes the component associated with the specified entity.
     *
     * @param entity The entity whose component should be removed.
     */
    void removeComponent(Entity entity);

    /**
     * @brief Retrieves a pointer to an entity's component.
     *
     * Returns a pointer to the component if it exists, or nullptr if the entity
     * does not have the requested component.
     *
     * @param entity The entity whose component is being retrieved.
     * @return A pointer to the component, or nullptr if not found.
     */
    T* getComponent(Entity entity);

    /**
     * @brief Retrieves all components.
     *
     * Provides access to the internal unordered map storing all components
     * mapped to their respective entities.
     *
     * @return A reference to the unordered map of entity-component pairs.
     */
    std::unordered_map<Entity, T>& getAll();
};

#endif  // SRC_SHARED_MODELS_ENTITYCOMPONENTSYSTEM_COMPONENTMANAGER_HPP_

