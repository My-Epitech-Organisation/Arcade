// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Entity Manager
*/

#include <algorithm>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Entity/Entity.hpp"
#include "Entity/EntityManager.hpp"

namespace Arcade {

std::shared_ptr<IEntity> EntityManager::createEntity(std::string name) {
    int id = 0;
    if (_nextEntityId) {
        id = _nextEntityId->getId() + 1;
    }
    std::shared_ptr<IEntity> newEntity = std::make_shared<Entity>(id, name);
    _nextEntityId = newEntity;
    _activeEntities[newEntity] = name;
    return newEntity;
}

void EntityManager::destroyEntity(std::shared_ptr<IEntity> entity) {
    auto it = _activeEntities.find(entity);

    if (it != _activeEntities.end())
        _activeEntities.erase(it);
}

const std::vector<std::shared_ptr<IEntity>> EntityManager::getEntitiesVector() const {
    try {
        std::cout << "EntityManager::getEntitiesVector: _activeEntities size = " << _activeEntities.size() << std::endl;
        // Pre-allocate memory to avoid reallocation
        std::vector<std::shared_ptr<IEntity>> entities;
        entities.reserve(_activeEntities.size());
        
        // Add entities to vector
        for (const auto& pair : _activeEntities) {
            const auto& entity = pair.first;
            if (entity) {
                entities.push_back(entity);
            }
        }
        return entities;
    } catch (const std::bad_alloc&) {
        // Fallback to a smaller allocation if the full reservation fails
        std::cerr << "Warning: Memory allocation issue in getEntitiesVector, using smaller capacity" << std::endl;
        std::vector<std::shared_ptr<IEntity>> entities;
        
        // Process entities in smaller batches if needed
        for (const auto& pair : _activeEntities) {
            const auto& entity = pair.first;
            if (entity) {
                try {
                    entities.push_back(entity);
                } catch (const std::bad_alloc&) {
                    std::cerr << "Critical: Unable to add entity to vector" << std::endl;
                    // Return what we have so far
                    break;
                }
            }
        }
        return entities;
    }
}

const std::unordered_map<std::shared_ptr<IEntity>, std::string>
EntityManager::getEntitiesMap() const {
    return _activeEntities;
}

void EntityManager::resetEntities() {
    _activeEntities.clear();
    _nextEntityId.reset();
}

std::shared_ptr<IEntity> EntityManager::getEntity(int id) {
    for (const auto& [entity, _] : _activeEntities) {
        if (entity->getId() == id)
            return entity;
    }
    return nullptr;
}

void EntityManager::addEntity(std::shared_ptr<IEntity> entity) {
    if (entity) {
        _activeEntities[entity] = entity->getName();
    }
}

}  // namespace Arcade
