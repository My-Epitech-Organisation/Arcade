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
    #include <memory>
    #include <string>
    #include "Interface/ECS/IEntity.hpp"

namespace Arcade {
class IEntityManager {
 public:
    virtual ~IEntityManager() = default;
    virtual std::shared_ptr<IEntity> createEntity(std::string name) = 0;
    virtual void destroyEntity(std::shared_ptr<IEntity> entity) = 0;
    virtual const std::vector<std::shared_ptr<IEntity>>
        getEntitiesVector() const = 0;
    virtual const std::unordered_map<std::shared_ptr<IEntity>, std::string>
        getEntitiesMap() const = 0;
    virtual void resetEntities() = 0;
    virtual void addEntity(std::shared_ptr<IEntity> entity) = 0;
    virtual std::shared_ptr<IEntity> getEntity(int id) = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ECS_IENTITYMANAGER_HPP_
