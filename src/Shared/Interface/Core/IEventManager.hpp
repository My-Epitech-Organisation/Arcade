// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** IEventManager interface
*/
// Copyright 2025 <Epitech>
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
    #include <string>
    #include "Interface/Core/IEvent.hpp"
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
    virtual ~IEventManager() = default;
    using Callback = std::function<void()>;
    virtual void subscribe(const IEvent& eventType,
        const Callback callback) = 0;
    virtual void publish(const IEvent& eventType) = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_CORE_IEVENTMANAGER_HPP_
