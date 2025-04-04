// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** IEventManager interface
*/
/**
 * @file IEventManager.hpp
 * @brief Interface for managing user input events in the Arcade project.
 *
 * The IEventManager interface defines a set of methods for handling
 * keyboard and mouse events, including retrieving event states and polling for new events.
 */

#ifndef SRC_SHARED_INTERFACE_CORE_IEVENTMANAGER_HPP_
    #define SRC_SHARED_INTERFACE_CORE_IEVENTMANAGER_HPP_
    #include <functional>
    #include <utility>
    #include "Interface/Core/IEvent.hpp"
    #include "../../Models/KeysType.hpp"
    #include "../../Models/MouseButtonType.hpp"
    #include "EventManager/KeyEvent/RawInputState.hpp"
namespace Arcade {

/**
* @class IEventManager
* @brief Interface for event management.
*
* The IEventManager interface provides methods for handling user input events,
* including keyboard and mouse input, event polling, and retrieving the current
* mouse position.
*/
class IEventManager {
 public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IEventManager() = default;

    /**
     * @brief Type definition for event callback functions.
     */
    using Callback = std::function<void()>;

    /**
     * @brief Subscribe to a keyboard event with a callback function.
     *
     * @param eventType The keyboard event to subscribe to.
     * @param callback The function to call when the event occurs.
     */
    virtual void subscribe(const IEvent& eventType,
        const Callback callback) = 0;

    /**
     * @brief Publish a keyboard event to all subscribers.
     *
     * @param eventType The keyboard event to publish.
     */
    virtual void publish(const IEvent& eventType) = 0;


    /**
     * @brief Retrieves the current position of the mouse.
     *
     * @return A pair containing the x and y coordinates of the mouse position.
     */
    virtual std::pair<std::size_t, std::size_t> getMousePosition() const = 0;

    /**
     * @brief Unsubscribe from all events.
     *
     * Removes all registered event callbacks for the caller.
     */
    virtual void unsubscribeAll() = 0;

    /**
     * @brief Check if a specific key is currently pressed.
     *
     * @param key The key code to check.
     * @return true if the key is pressed, false otherwise.
     */
    virtual bool isKeyPressed(Keys key) const = 0;

    /**
     * @brief Set the current mouse position.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    virtual void setMousePosition(std::size_t x, std::size_t y) = 0;

    /**
     * @brief Update the input state from raw input data.
     *
     * @param state The raw input state to process.
     */
    virtual void updateInputState(const RawInputState& state) = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_CORE_IEVENTMANAGER_HPP_
