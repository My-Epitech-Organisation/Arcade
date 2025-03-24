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
    #include "Interface/ECS/IComponent.hpp"
    #include "Models/EntityType.hpp"

namespace Arcade {
class IComponentManager {
 public:
    virtual ~IComponentManager() = default;
    virtual void registerComponent(Entity entity, IComponent* component) = 0;
    virtual IComponent* getComponent(Entity entity,
        const std::string& componentName) = 0;
    virtual void removeComponent(Entity entity,
        const std::string& componentName) = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ECS_ICOMPONENTMANAGER_HPP_
