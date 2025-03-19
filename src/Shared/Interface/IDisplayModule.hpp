// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** IDisplayModule interface
*/
/**
 * @file IDisplayModule.hpp
 * @brief Interface for display modules in the Arcade project.
 *
 * This interface defines the necessary methods for implementing
 * a display module in the Arcade project.
 */
#ifndef SRC_SHARED_INTERFACE_IDISPLAYMODULE_HPP_
    #define SRC_SHARED_INTERFACE_IDISPLAYMODULE_HPP_
    #include <memory>
    #include <string>
    #include "../Models/InputModel.hpp"

namespace Arcade {
/**
 * @class IDisplayModule
 * @brief Interface for graphical display modules.
 *
 * The IDisplayModule interface provides a set of pure virtual functions
 * that must be implemented by any graphical display module in the Arcade project.
 * It includes methods for initialization, rendering, input handling,
 * and refreshing the display.
 */
class IDisplayModule {
 public:
    /**
     * @brief Virtual destructor for the interface.
     */
    virtual ~IDisplayModule() = default;
    /**
     * @brief Initializes the display module.
     *
     * This method is responsible for setting up the display module,
     * loading necessary resources, and preparing the graphical environment.
     */
    virtual void init() = 0;
    /**
     * @brief Renders the current frame.
     *
     * This method should draw all necessary elements onto the screen.
     */
    virtual void render() = 0;
    /**
     * @brief Retrieves the latest user input.
     *
     * This method captures and returns the latest input received from the user.
     * @return An InputModel representing the user's input.
     */
    virtual InputModel::Input getInput() = 0;
    /**
     * @brief Sets the entities to be displayed.
     *
     * This method updates the graphical entities that should be rendered on the screen.
     */
    virtual void drawElement() = 0;
    virtual void stop() = 0;

    virtual const std::string& getName() const = 0;
};
}  // namespace Arcade
#endif  // SRC_SHARED_INTERFACE_IDISPLAYMODULE_HPP_
