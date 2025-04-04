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

void ComponentManager::registerComponent(Entity entity,
std::shared_ptr<IComponent> component) {
    if (!component) return;

    ComponentType type = component->getType();
    _componentsByType[entity][type] = component;

    _typeNameMap[typeid(*component).name()] = type;
}

void ComponentManager::unregisterComponent(Entity entity,
const std::string& componentName) {
    if (_componentsByType.find(entity) != _componentsByType.end()) {
        auto& components = _componentsByType[entity];
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

std::shared_ptr<IComponent> ComponentManager::getComponentBase(Entity entity,
const std::string& typeName) {
    if (_componentsByType.find(entity) == _componentsByType.end())
        return nullptr;

    if (_typeNameMap.find(typeName) == _typeNameMap.end())
        return nullptr;

    ComponentType type = _typeNameMap[typeName];
    auto& entityComponents = _componentsByType[entity];

    if (entityComponents.find(type) == entityComponents.end())
        return nullptr;

    return entityComponents[type];
}

std::shared_ptr<IComponent> ComponentManager::getComponentByType(Entity entity,
ComponentType type) {
    if (_componentsByType.find(entity) == _componentsByType.end())
        return nullptr;

    auto& entityComponents = _componentsByType[entity];
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
Entity entity) {
    std::vector<std::shared_ptr<IComponent>> result;

    if (_componentsByType.find(entity) != _componentsByType.end()) {
        for (const auto& [type, component] : _componentsByType[entity]) {
            result.push_back(component);
        }
    }

    return result;
}

}  // namespace Arcade
