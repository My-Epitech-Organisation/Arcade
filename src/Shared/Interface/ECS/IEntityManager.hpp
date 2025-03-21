// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** EntityManager Interface
*/

#ifndef SRC_SHARED_INTERFACE_ECS_IENTITYMANAGER_HPP_
    #define SRC_SHARED_INTERFACE_ECS_IENTITYMANAGER_HPP_
    #include "ECS/IEntity.hpp"

namespace Arcade {
class IEntityManager {
 public:
    virtual ~IEntityManager() = default;
    virtual IEntity* createEntity() = 0;
    virtual void removeEntity(int entityId) = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ECS_IENTITYMANAGER_HPP_
