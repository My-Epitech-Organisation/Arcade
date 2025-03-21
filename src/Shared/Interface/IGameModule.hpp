// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** IGameModule interface
*/
/**
 * @file IGameModule.hpp
 * @brief Interface for game modules in the Arcade project.
 *
 * This interface defines the necessary methods for implementing
 * a game module in the Arcade project.
 */

#ifndef SRC_SHARED_INTERFACE_IGAMEMODULE_HPP_
    #define SRC_SHARED_INTERFACE_IGAMEMODULE_HPP_
    #include <memory>
    #include <string>
    #include <vector>
    #include <unordered_map>
    #include "Shared/Models/EntityComponentSystem/EntityManager.hpp"
    #include "Interface/IEventManager.hpp"

namespace Arcade {
/**
 * @class IGameModule
 * @brief Interface for game logic modules.
 *
 * The IGameModule interface provides a set of pure virtual functions
 * that must be implemented by any game module in the Arcade project.
 * It includes methods for starting, updating, rendering, and handling input.
 */
class IGameModule {
 public:
    /**
     * @brief Virtual destructor for the interface.
     */
    virtual ~IGameModule() = default;
    /**
     * @brief Starts the game module.
     *
     * This method is responsible for initializing the game state
     * and preparing all necessary resources for the game session.
     */
    virtual void init() = 0;

    /**
     * @brief Updates the game state.
     *
     * This method should process game logic, update entities,
     * and handle internal state transitions.
     */
    virtual void update() = 0;

    /**
     * @brief Renders the game elements.
     *
     * This method should be responsible for preparing the game's
     * visual elements before they are displayed.
     */
    virtual void render() = 0;

    /**
     * @brief Retrieves the game entities.
     *
     * This method should return the current entities involved in the game
     * to be processed or displayed.
     * @return A map of entity names and their corresponding ECS components.
     */
    virtual std::vector<Entity> getElements() const = 0;
    /**
     * @brief Handles player input.
     *
     * This method should process user input and apply the corresponding
     * actions to the game logic.
     */
    virtual void handleInput(const IEventManager &event) = 0;

    /**
     * @brief Retrieves the game's name.
     *
     * This method should return the name of the game module.
     * @return The name of the game module.
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Retrieves the game's score.
     *
     * This method should return the current score of the game session.
     * @return The current score of the game session.
     */
    virtual int getScore() const = 0;
};
}  // namespace Arcade
#endif  // SRC_SHARED_INTERFACE_IGAMEMODULE_HPP_
