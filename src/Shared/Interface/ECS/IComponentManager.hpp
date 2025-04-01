// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** ComponentManager Interface
*/

#ifndef SRC_SHARED_INTERFACE_ECS_ICOMPONENTMANAGER_HPP_
    #define SRC_SHARED_INTERFACE_ECS_ICOMPONENTMANAGER_HPP_
    #include <string>
    #include <memory>
    #include <vector>
    #include "Shared/Interface/ECS/IComponent.hpp"
    #include "Shared/Interface/ECS/IEntity.hpp"
    #include "Shared/Models/ComponentType.hpp"

namespace Arcade {
class IComponentManager {
 public:
    virtual ~IComponentManager() = default;
    // Register a component
    virtual void registerComponent(Entity entity,
        std::shared_ptr<IComponent> component) = 0;
    // Get component by entity and component type
    virtual std::shared_ptr<IComponent> getComponentByType(Entity entity,
        ComponentType type) = 0;
    // Get component by entity and type name (for backward compatibility)
    virtual std::shared_ptr<IComponent> getComponentBase(Entity entity,
        const std::string& typeName) = 0;
    // Get all components of a specific type
    virtual std::vector<std::shared_ptr<IComponent>> getAllComponentsByType(
        ComponentType type) = 0;
    // Unregister a component
    virtual void unregisterComponent(Entity entity,
        const std::string& componentName) = 0;
    // Get all components for an entity
    virtual std::vector<std::shared_ptr<IComponent>> getEntityComponents(Entity entity) = 0;
    // Get all components
    virtual std::vector<std::shared_ptr<IComponent>> getAllComponents() = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ECS_ICOMPONENTMANAGER_HPP_