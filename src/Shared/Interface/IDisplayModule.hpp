// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Display Module Interface
*/
/**
 * @file IDisplayModule.hpp
 * @brief Interface for graphical display modules in the Arcade project.
 *
 * The IDisplayModule interface provides methods for initializing a display module,
 * handling user input, rendering graphical elements, and retrieving the module's name.
 */

#ifndef SRC_SHARED_INTERFACE_IDISPLAYMODULE_HPP_
    #define SRC_SHARED_INTERFACE_IDISPLAYMODULE_HPP_
    #include <string>
    #include <vector>
    #include "Shared/Models/EntityComponentSystem/EntityManager.hpp"
    #include "Interface/IEventManager.hpp"

namespace Arcade {

/**
 * @class IDisplayModule
 * @brief Interface for graphical display modules.
 *
 * The IDisplayModule interface defines the required methods that each display module
 * must implement. This includes initializing the module, handling input,
 * rendering game entities, and retrieving the module's name.
 */
class IDisplayModule {
 public:
    /**
     * @brief Virtual destructor for the interface.
     *
     * Ensures that derived classes clean up their resources upon destruction.
     */
    virtual ~IDisplayModule() = default;

    /**
     * @brief Initializes the display module.
     *
     * This method is responsible for setting up the display module,
     * loading required resources, and preparing the graphical environment.
     */
    virtual void init() = 0;

    /**
     * @brief Retrieves the input manager.
     *
     * This method returns an instance of the `IEventManager` to handle user input events.
     *
     * @return An `IEventManager` instance responsible for managing user inputs.
     */
    virtual IEventManager getInput(void) = 0;

    /**
     * @brief Draws a set of game entities.
     *
     * This method takes a list of entities and processes their rendering on the display.
     *
     * @param entities A vector of `Entity` objects representing game entities to be drawn.
     */
    virtual void drawElement(const std::vector<Entity> &entities) = 0;

    /**
     * @brief Renders the display module.
     *
     * This method finalizes all drawing operations and updates the display with the latest
     * graphical elements.
     */
    virtual void render() = 0;

    /**
     * @brief Retrieves the name of the display module.
     *
     * @return A string representing the name of the display module.
     */
    virtual std::string getName() const = 0;
};

}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_IDISPLAYMODULE_HPP_
