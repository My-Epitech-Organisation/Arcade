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
    #include <memory>
    #include <utility>
    #include "Interface/ECS/IEntityManager.hpp"
    #include "Interface/Core/IEventManager.hpp"
    #include "Models/ColorType.hpp"
    #include "Interface/IArcadeModule.hpp"
    #include "../ECS/Components/Drawable/DrawableComponent.hpp"
    #include "Interface/Core/IWindowModule.hpp"

namespace Arcade {

/**
 * @class IDisplayModule
 * @brief Interface for graphical display modules.
 *
 * The IDisplayModule interface defines the required methods that each display module
 * must implement. This includes initializing the module, handling input,
 * rendering game entities, and retrieving the module's name.
 */
class IDisplayModule : public IArcadeModule {
 public:
    virtual ~IDisplayModule() = default;
    virtual void init(const Arcade::IWindowModule& windowParam) = 0;
    virtual void stop() = 0;
    virtual void clearScreen() = 0;
    virtual void refreshScreen() = 0;
    virtual void drawDrawable(const DrawableComponent& drawable) = 0;
    virtual void pollEvents() = 0;
    virtual bool isOpen() const = 0;
    virtual bool isKeyPressed(int keyCode) = 0;
    virtual const std::string &getName() const = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual bool isMouseButtonPressed(int button) const = 0;
    virtual std::pair<size_t, size_t> getMousePosition() const = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_DISPLAY_IDISPLAYMODULE_HPP_
