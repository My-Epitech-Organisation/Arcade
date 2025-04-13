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
    #include <unordered_map>
    #include <vector>
    #include <memory>
    #include <string>
    #include "Shared/Interface/ECS/IEntity.hpp"
    #include "Shared/Interface/ECS/IEntityManager.hpp"

namespace Arcade {
/**
 * @class EntityManager
 * @brief Manages entities in an ECS.
 *
 * The EntityManager class handles the creation and deletion of entities,
 * ensuring each entity has a unique identifier and maintaining a list of active entities.
 */
class EntityManager : public IEntityManager {
 public:
    /**
     * @brief Creates a new entity.
     *
     * Generates a new unique entity identifier and adds it to the list of active entities.
     *
     * @return The newly created entity identifier.
     */
    std::shared_ptr<IEntity> createEntity(std::string name) override;

    /**
     * @brief Destroys an existing entity.
     *
     * Removes the specified entity from the list of active entities.
     *
     * @param entity The entity identifier to be destroyed.
     */
    void destroyEntity(std::shared_ptr<IEntity> entity) override;

    /**
     * @brief Retrieves the list of active entities.
     *
     * @return A constant reference to a vector containing all active entity identifiers.
     */
    const std::vector<std::shared_ptr<IEntity>> getEntitiesVector()
        const override;
    const std::unordered_map<std::shared_ptr<IEntity>, std::string>
        getEntitiesMap() const override;
    void resetEntities() override;
    void addEntity(std::shared_ptr<IEntity> entity) override;
    std::shared_ptr<IEntity> getEntity(int id) override;

 private:
    std::shared_ptr<IEntity> _nextEntityId = 0;
    std::unordered_map<std::shared_ptr<IEntity>, std::string>
        _activeEntities;  /// Stores currently active entities.
};
}  // namespace Arcade
#endif  // SRC_ECS_ENTITY_ENTITYMANAGER_HPP_
