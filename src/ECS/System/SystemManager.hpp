// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** System Manager
*/
/**
 * @file SystemManager.hpp
 * @brief Manages multiple systems in an Entity Component System (ECS).
 *
 * The SystemManager class handles the addition and updating of systems that
 * implement the ISystem interface.
 */

#ifndef SRC_ECS_SYSTEM_SYSTEMMANAGER_HPP_
    #define SRC_ECS_SYSTEM_SYSTEMMANAGER_HPP_
    #include <vector>
    #include <memory>
    #include "Shared/Interface/ECS/ISystemManager.hpp"

namespace Arcade {
/**
 * @class SystemManager
 * @brief Manages and updates game systems.
 *
 * The SystemManager class is responsible for managing multiple game systems
 * that implement the Arcade::ISystem interface. It provides methods to add
 * systems and update them collectively.
 */
class SystemManager : public Arcade::ISystemManager {
 public:
    /**
     * @brief Adds a system to the manager.
     *
     * This method stores a pointer to a system that implements the ISystem interface.
     *
     * @param system A pointer to an ISystem instance to be managed.
     */
    void registerSystem(std::shared_ptr<Arcade::ISystem> system) override;

    void unregisterSystem(std::shared_ptr<Arcade::ISystem> system) override;
    /**
     * @brief Updates all registered systems.
     *
     * This method iterates through the stored systems and calls their update function.
     */
    void updateSystems(float deltaTime) override;
    /**
     * @brief Clears all registered systems.
     *
     * This method removes all systems from the manager.
     */
    void clearSystems() override;
    /**
     * @brief Retrieves the list of registered systems.
     *
     * @return A constant reference to a vector containing pointers to all managed systems.
     */
    const std::vector<std::shared_ptr<Arcade::ISystem>>&
    getSystems() const override {
        return _systems;
    };

 private:
    std::vector<std::shared_ptr<Arcade::ISystem>> _systems;
};
}  // namespace Arcade
#endif  // SRC_ECS_SYSTEM_SYSTEMMANAGER_HPP_
