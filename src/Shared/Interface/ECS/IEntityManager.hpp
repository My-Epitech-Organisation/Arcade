// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** EntityManager Interface
*/

#ifndef SRC_SHARED_INTERFACE_ECS_IENTITYMANAGER_HPP_
    #define SRC_SHARED_INTERFACE_ECS_IENTITYMANAGER_HPP_
    #include <vector>
    #include <unordered_map>
    #include <string>
    #include "Models/EntityType.hpp"

namespace Arcade {
class IEntityManager {
 public:
    virtual ~IEntityManager() = default;
    virtual Entity createEntity(std::string) = 0;
    virtual void destroyEntity(Entity entity) = 0;
    virtual const std::unordered_map<Entity, std::string>&
        getEntities() const = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ECS_IENTITYMANAGER_HPP_
