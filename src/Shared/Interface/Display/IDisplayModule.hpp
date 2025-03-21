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

#ifndef SRC_SHARED_INTERFACE_DISPLAY_IDISPLAYMODULE_HPP_
    #define SRC_SHARED_INTERFACE_DISPLAY_IDISPLAYMODULE_HPP_
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
    virtual ~IDisplayModule() = default;
    virtual void init(float width, float height) = 0;
    virtual void stop() = 0;
    virtual void clearScreen() = 0;
    virtual void refreshScreen() = 0;
    virtual void drawEntity(int x, int y, char symbol) = 0;
    virtual void drawTexture(int x, int y,
        const std::string &textureId) = 0;
    virtual void drawText(int x, int y, const std::string &text) = 0;
    virtual void pollEvents() = 0;
    virtual bool isOpen() const = 0;
    virtual const std::string &getName() const = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_DISPLAY_IDISPLAYMODULE_HPP_
