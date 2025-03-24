// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Entity manager
*/
// Copyright 2025 <Epitech>
/**
 * @file EntityManager.hpp
 * @brief Manages entity creation and deletion in an Entity Component System (ECS).
 *
 * The EntityManager class is responsible for generating unique entity identifiers
 * and tracking active entities.
 */

#ifndef SRC_ECS_ENTITY_ENTITYMANAGER_HPP_
    #define SRC_ECS_ENTITY_ENTITYMANAGER_HPP_
    #include <iostream>
    #include <vector>
    #include "Shared/Interface/ECS/IEntity.hpp"
    #include "Shared/Interface/ECS/IEntityManager.hpp"

using Entity = std::size_t;  /// Alias for entity identifiers.
/**
 * @class EntityManager
 * @brief Manages entities in an ECS.
 *
 * The EntityManager class handles the creation and deletion of entities,
 * ensuring each entity has a unique identifier and maintaining a list of active entities.
 */
class EntityManager : public Arcade::IEntityManager {
 public:
    /**
     * @brief Creates a new entity.
     *
     * Generates a new unique entity identifier and adds it to the list of active entities.
     *
     * @return The newly created entity identifier.
     */
    Entity createEntity();

    /**
     * @brief Destroys an existing entity.
     *
     * Removes the specified entity from the list of active entities.
     *
     * @param entity The entity identifier to be destroyed.
     */
    void destroyEntity(Entity entity);

    /**
     * @brief Retrieves the list of active entities.
     *
     * @return A constant reference to a vector containing all active entity identifiers.
     */
    const std::vector<Entity>& getEntities() const;

 private:
    Entity _nextEntityId = 0;  /// Tracks the next available entity identifier.
    std::vector<Entity> _activeEntities;  /// Stores currently active entities.
};

#endif  // SRC_ECS_ENTITY_ENTITYMANAGER_HPP_
