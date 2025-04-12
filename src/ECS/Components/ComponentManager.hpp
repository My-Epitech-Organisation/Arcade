// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** ComponentManager
*/

#ifndef SRC_ECS_COMPONENTS_COMPONENTMANAGER_HPP_
    #define SRC_ECS_COMPONENTS_COMPONENTMANAGER_HPP_
    #include <unordered_map>
    #include <memory>
    #include <vector>
    #include <string>
    #include "Shared/Interface/ECS/IEntity.hpp"
    #include "Shared/Interface/ECS/IComponentManager.hpp"
    #include "Shared/Interface/ECS/IComponent.hpp"
    #include "Shared/Models/ComponentType.hpp"

namespace Arcade {

class ComponentManager : public IComponentManager {
 private:
    std::unordered_map<std::shared_ptr<IEntity>,
        std::unordered_map<ComponentType,
        std::shared_ptr<IComponent>>> _componentsByType;
    std::unordered_map<std::string, ComponentType> _typeNameMap;

 public:
    ComponentManager() = default;
    ~ComponentManager() = default;

    void registerComponent(std::shared_ptr<IEntity> entity,
        std::shared_ptr<IComponent> component) override;
    std::shared_ptr<IComponent>
        getComponentByType(std::shared_ptr<IEntity> entity,
        ComponentType type) override;
    std::vector<std::shared_ptr<IComponent>> getAllComponentsByType
        (ComponentType type) override;
    bool hasComponent(std::shared_ptr<IEntity> entity,
        ComponentType type) const override;
    void unregisterComponent(std::shared_ptr<IEntity> entity, const std::string&
        componentName) override;
    std::vector<std::shared_ptr<IComponent>>
        getEntityComponents(std::shared_ptr<IEntity>
        entity) override;
    std::vector<std::shared_ptr<IComponent>> getAllComponents() override;
    void clearComponents() override;
};

}  // namespace Arcade

#endif  // SRC_ECS_COMPONENTS_COMPONENTMANAGER_HPP_
