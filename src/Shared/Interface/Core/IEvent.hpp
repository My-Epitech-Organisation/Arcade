// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** IEvent interface
*/
/**
 * @file IEvent.hpp
 * @brief Interface for events in the Arcade project.
 *
 * The IEvent interface defines a common base for all events in the Arcade system,
 * providing methods to retrieve the event type and other information.
 */

#ifndef SRC_SHARED_INTERFACE_CORE_IEVENT_HPP_
    #define SRC_SHARED_INTERFACE_CORE_IEVENT_HPP_
    #include <string>
    #include "Models/EventType.hpp"

namespace Arcade {

/**
* @class IEvent
* @brief Interface for all event types.
*
* The IEvent interface is the base class for all events in the Arcade system.
*/
class IEvent {
 public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IEvent() = default;

    /**
     * @brief Get the type of the event.
     * @return The EventType of this event.
     */
    virtual EventType getType() const = 0;

    /**
     * @brief Check if the event is of a specific type.
     * @param type The event type to check against.
     * @return True if the event matches the specified type, false otherwise.
     */
    virtual bool isType(EventType type) const = 0;
};

}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_CORE_IEVENT_HPP_
