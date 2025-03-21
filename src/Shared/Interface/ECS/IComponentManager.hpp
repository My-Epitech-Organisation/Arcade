// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** ComponentManager Interface
*/

#ifndef SRC_SHARED_INTERFACE_ECS_ICOMPONENTMANAGER_HPP_
    #define SRC_SHARED_INTERFACE_ECS_ICOMPONENTMANAGER_HPP_
    #include "ECS/IComponent.hpp"

namespace Arcade {
class IComponentManager {
 public:
    virtual ~IComponentManager() = default;
    virtual void registerComponent(Arcade::IComponent* component) = 0;
};
}

#endif  // SRC_SHARED_INTERFACE_ECS_ICOMPONENTMANAGER_HPP_
