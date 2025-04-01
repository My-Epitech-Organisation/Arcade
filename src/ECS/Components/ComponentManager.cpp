#include "ECS/Components/ComponentManager.hpp"
#include <iostream>

namespace Arcade {

void ComponentManager::registerComponent(Entity entity,
                                       std::shared_ptr<IComponent> component) {
    if (!component) return;
    
    // Store component by its type
    ComponentType type = component->getType();
    _componentsByType[entity][type] = component;
    
    // Also store by typeid for template support
    _typeNameMap[typeid(*component).name()] = type;
}

void ComponentManager::unregisterComponent(Entity entity,
                                         const std::string& componentName) {
    // Implementation depends on how you identify components by name
    // This is a simple approach - remove all components for the entity
    if (_componentsByType.find(entity) != _componentsByType.end()) {
        _componentsByType.erase(entity);
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
    if (_componentsByType.find(entity) == _componentsByType.end()) {
        return nullptr;
    }

    // Try to find component type from type name
    if (_typeNameMap.find(typeName) == _typeNameMap.end()) {
        return nullptr;
    }

    ComponentType type = _typeNameMap[typeName];
    auto& entityComponents = _componentsByType[entity];
    
    if (entityComponents.find(type) == entityComponents.end()) {
        return nullptr;
    }
    
    return entityComponents[type];
}

// Implement the remaining methods:

std::shared_ptr<IComponent> ComponentManager::getComponentByType(Entity entity, ComponentType type) {
    // Check if entity exists
    if (_componentsByType.find(entity) == _componentsByType.end()) {
        return nullptr;
    }
    
    // Check if entity has component of this type
    auto& entityComponents = _componentsByType[entity];
    if (entityComponents.find(type) == entityComponents.end()) {
        return nullptr;
    }
    
    return entityComponents[type];
}

std::vector<std::shared_ptr<IComponent>> ComponentManager::getAllComponentsByType(ComponentType type) {
    std::vector<std::shared_ptr<IComponent>> result;
    
    for (const auto& [entity, componentMap] : _componentsByType) {
        auto it = componentMap.find(type);
        if (it != componentMap.end()) {
            result.push_back(it->second);
        }
    }
    
    return result;
}

std::vector<std::shared_ptr<IComponent>> ComponentManager::getEntityComponents(Entity entity) {
    std::vector<std::shared_ptr<IComponent>> result;
    
    if (_componentsByType.find(entity) != _componentsByType.end()) {
        for (const auto& [type, component] : _componentsByType[entity]) {
            result.push_back(component);
        }
    }
    
    return result;
}

} // namespace Arcade