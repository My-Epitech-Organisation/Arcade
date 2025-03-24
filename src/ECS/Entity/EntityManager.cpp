// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Entity Manager
*/

#include <algorithm>
#include <vector>
#include "ECS/Entity/EntityManager.hpp"

Entity EntityManager::createEntity() {
    return _nextEntityId++;
}

void EntityManager::destroyEntity(Entity entity) {
    auto it = std::find(_activeEntities.begin(), _activeEntities.end(), entity);

    if (it != _activeEntities.end())
        _activeEntities.erase(it);
}

const std::vector<Entity>& EntityManager::getEntities() const {
    return _activeEntities;
}
