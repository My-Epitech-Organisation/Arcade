#ifndef SRC_ECS_COMPONENTS_COMPONENTMANAGER_HPP_
#define SRC_ECS_COMPONENTS_COMPONENTMANAGER_HPP_

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include "Shared/Models/EntityType.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Models/ComponentType.hpp"

namespace Arcade {

class ComponentManager : public IComponentManager {
private:
    std::unordered_map<Entity, std::unordered_map<ComponentType, std::shared_ptr<IComponent>>> _componentsByType;
    std::unordered_map<std::string, ComponentType> _typeNameMap;

public:
    ComponentManager() = default;
    ~ComponentManager() = default;

    void registerComponent(Entity entity, std::shared_ptr<IComponent> component) override;
    std::shared_ptr<IComponent> getComponentByType(Entity entity, ComponentType type) override;
    std::shared_ptr<IComponent> getComponentBase(Entity entity, const std::string& typeName) override;
    std::vector<std::shared_ptr<IComponent>> getAllComponentsByType(ComponentType type) override;
    void unregisterComponent(Entity entity, const std::string& componentName) override;
    std::vector<std::shared_ptr<IComponent>> getEntityComponents(Entity entity) override;
    std::vector<std::shared_ptr<IComponent>> getAllComponents() override;
};

} // namespace Arcade

#endif  // SRC_ECS_COMPONENTS_COMPONENTMANAGER_HPP_