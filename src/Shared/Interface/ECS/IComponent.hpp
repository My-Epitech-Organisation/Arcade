// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Component Interface
*/

#ifndef SRC_SHARED_INTERFACE_ECS_ICOMPONENT_HPP_
    #define SRC_SHARED_INTERFACE_ECS_ICOMPONENT_HPP_
    #include "Shared/Models/ComponentType.hpp"
    #include "Shared/Interface/IArcadeModule.hpp"
    #include "Shared/Interface/ECS/IEntity.hpp"
    #include "Shared/Interface/ECS/IEntity.hpp"

namespace Arcade {
class IComponent : public IArcadeModule {
 public:
    virtual ~IComponent() = default;
    virtual ComponentType getType() const = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ECS_ICOMPONENT_HPP_
