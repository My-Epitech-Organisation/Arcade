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
    #include "Shared/Interface/ECS/ISystemManager.hpp"

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
    void registerSystem(Arcade::ISystem* system);

    void removeSystem(Arcade::ISystem* system);
    /**
     * @brief Updates all registered systems.
     *
     * This method iterates through the stored systems and calls their update function.
     */
    void updateSystems();

 private:
    std::vector<Arcade::ISystem*> _systems;  ///< List of managed systems.
};

#endif  // SRC_ECS_SYSTEM_SYSTEMMANAGER_HPP_
