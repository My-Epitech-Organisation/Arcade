// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** SystemManager Interface
*/

#ifndef SRC_SHARED_INTERFACE_ECS_ISYSTEMMANAGER_HPP_
    #define SRC_SHARED_INTERFACE_ECS_ISYSTEMMANAGER_HPP_
    #include <memory>
    #include "Interface/ECS/ISystem.hpp"

namespace Arcade {
class ISystemManager {
 public:
    virtual ~ISystemManager() = default;
    virtual void registerSystem(std::shared_ptr<Arcade::ISystem> system) = 0;
    virtual void unregisterSystem(std::shared_ptr<Arcade::ISystem> system) = 0;
    virtual void updateSystems() = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ECS_ISYSTEMMANAGER_HPP_
