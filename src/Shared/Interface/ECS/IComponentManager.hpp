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
    #include "Interface/ECS/IComponent.hpp"
    #include "Interface/ECS/IEntity.hpp"

namespace Arcade {
class IComponentManager {
 public:
    virtual ~IComponentManager() = default;
    virtual void registerComponent(std::shared_ptr<IEntity> entity,
        std::shared_ptr<IComponent> component) = 0;
    virtual IComponent* getComponent(std::shared_ptr<IEntity> entity,
        const std::string& componentName) = 0;
    virtual void unregisterComponent(std::shared_ptr<IEntity> entity,
        const std::string& componentName) = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ECS_ICOMPONENTMANAGER_HPP_
