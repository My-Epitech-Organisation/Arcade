// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** SystemManager Interface
*/

#ifndef SRC_SHARED_INTERFACE_ECS_ISYSTEMMANAGER_HPP_
    #define SRC_SHARED_INTERFACE_ECS_ISYSTEMMANAGER_HPP_
    #include "ECS/ISystem.hpp"

namespace Arcade {
class ISystemManager {
 public:
    virtual ~ISystemManager() = default;
    virtual void registerSystem(Arcade::ISystem* system) = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ECS_ISYSTEMMANAGER_HPP_
