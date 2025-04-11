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

const std::vector<std::shared_ptr<IEntity>>
EntityManager::getEntitiesVector() const {
    std::vector<std::shared_ptr<IEntity>> entities;
    for (const auto& [entity, _] : _activeEntities) {
        entities.push_back(entity);
    }
    return entities;
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
