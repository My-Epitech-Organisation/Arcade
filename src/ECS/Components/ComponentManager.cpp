// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** ComponentManager
*/

#include "ECS/Components/ComponentManager.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <vector>
namespace Arcade {

void ComponentManager::registerComponent(std::shared_ptr<IEntity> entity,
std::shared_ptr<IComponent> component) {
    if (!component || !entity) return;

    std::shared_ptr<IEntity> entityId = entity;
    ComponentType type = component->getType();
    _componentsByType[entityId][type] = component;

    _typeNameMap[typeid(*component).name()] = type;
}

void ComponentManager::unregisterComponent(std::shared_ptr<IEntity> entity,
const std::string& componentName) {
    if (!entity) return;
    std::shared_ptr<IEntity> entityId = entity;
    if (_componentsByType.find(entityId) != _componentsByType.end()) {
        auto& components = _componentsByType[entityId];
        auto it = _typeNameMap.find(componentName);
        if (it != _typeNameMap.end()) {
            ComponentType type = it->second;
            components.erase(type);
        }
    }
}

std::vector<std::shared_ptr<IComponent>> ComponentManager::getAllComponents() {
    std::vector<std::shared_ptr<IComponent>> result;
    for (const auto& [entity, componentMap] : _componentsByType) {
        for (const auto& [type, component] : componentMap) {
            result.push_back(component);
        }
    }
    return result;
}

std::shared_ptr<IComponent>
ComponentManager::getComponentByType(std::shared_ptr<IEntity> entity,
ComponentType type) {
    if (!entity) return nullptr;
    std::shared_ptr<IEntity> entityId = entity;
    if (_componentsByType.find(entityId) == _componentsByType.end())
        return nullptr;

    auto& entityComponents = _componentsByType[entityId];
    if (entityComponents.find(type) == entityComponents.end())
        return nullptr;

    return entityComponents[type];
}

std::vector<std::shared_ptr<IComponent>>
ComponentManager::getAllComponentsByType(ComponentType type) {
    std::vector<std::shared_ptr<IComponent>> result;

    for (const auto& [entity, componentMap] : _componentsByType) {
        auto it = componentMap.find(type);
        if (it != componentMap.end()) {
            result.push_back(it->second);
        }
    }

    return result;
}

std::vector<std::shared_ptr<IComponent>> ComponentManager::getEntityComponents(
std::shared_ptr<IEntity> entity) {
    std::vector<std::shared_ptr<IComponent>> result;
    if (!entity) return result;
    std::shared_ptr<IEntity> entityId = entity;
    if (_componentsByType.find(entityId) != _componentsByType.end()) {
        for (const auto& [type, component] : _componentsByType[entityId]) {
            result.push_back(component);
        }
    }

    return result;
}

void ComponentManager::clearComponents() {
    _componentsByType.clear();
    _typeNameMap.clear();
}

bool
ComponentManager::hasComponent(std::shared_ptr<IEntity> entity,
ComponentType type) const {
    return _componentsByType.find(entity) != _componentsByType.end() &&
        _componentsByType.at(entity).find(type) !=
        _componentsByType.at(entity).end();
}

}  // namespace Arcade
