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
#include <vector>
#include "ECS/Entity/EntityManager.hpp"

Entity EntityManager::createEntity(std::string name) {
    Entity newEntity = _nextEntityId++;
    _activeEntities[newEntity] = name;
    return newEntity;
}

void EntityManager::destroyEntity(Entity entity) {
    auto it = _activeEntities.find(entity);

    if (it != _activeEntities.end())
        _activeEntities.erase(it);
}

const std::unordered_map<Entity, std::string>&
    EntityManager::getEntities() const {
    return _activeEntities;
}
