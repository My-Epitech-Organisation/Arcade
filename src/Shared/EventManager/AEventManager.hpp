// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** AEventManager abstract class
*/

// Copyright 2025 <Epitech>
/**
 * @file AEventManager.hpp
 * @brief Abstract class for event management in the Arcade project.
 *
 * The AEventManager class provides a base implementation for event handling,
 * including keyboard event states and mouse position tracking.
 */

#ifndef SRC_SHARED_EVENTMANAGER_AEVENTMANAGER_HPP_
    #define SRC_SHARED_EVENTMANAGER_AEVENTMANAGER_HPP_
    #include <vector>
    #include <string>
    #include <utility>
    #include <unordered_map>
    #include "Interface/Core/IEventManager.hpp"
    #include "Interface/Core/IEvent.hpp"
    #include "Models/EventType.hpp"

/**
 * @class AEventManager
 * @brief Abstract base class for event management.
 *
 * The AEventManager class provides an implementation of the IEventManager interface
 * with basic event state tracking, including key events and mouse position.
 * It must be extended by concrete event managers that implement `pollEvents()`.
 */
class AEventManager : public Arcade::IEventManager {
 public:
    /**
    * @brief Constructs an AEventManager object.
    *
    * Initializes key event states and mouse position.
    */
    AEventManager();

    /**
    * @brief Virtual destructor.
    */
    virtual ~AEventManager() = default;

    /**
    * @brief Subscribe to an event with a callback function.
    *
    * @param eventType The type of the event to subscribe to.
    * @param callback The function to call when the event occurs.
    */
    void subscribe(const Arcade::IEvent& eventType,
      const Callback callback) override;

   /**
   * @brief Publish an event to all subscribers.
   *
   * @param eventType The type of the event to publish.
   */
    void publish(const Arcade::IEvent& eventType) override;

    /**
    * @brief Polls for new events.
    *
    * This method is pure virtual and must be implemented by derived classes.
    */
    virtual void pollEvents() = 0;

 protected:
    /**
    * @brief Sets the X-coordinate of the mouse.
    *
    * @param x The new X position of the mouse cursor.
    */
    void setMouseX(std::size_t x);

    /**
    * @brief Sets the Y-coordinate of the mouse.
    *
    * @param y The new Y position of the mouse cursor.
    */
    void setMouseY(std::size_t y);

 private:
    std::vector<std::pair<int, bool>> _keyEvents;
      /// Stores key event states as (key, state) pairs.
    std::size_t _mouseX;  /// The current X position of the mouse.
    std::size_t _mouseY;  /// The current Y position of the mouse.
    std::unordered_map<Arcade::EventType, std::vector<Callback>> _subscribers;
         /// Maps event names to subscriber callback functions.
};

#endif  // SRC_SHARED_EVENTMANAGER_AEVENTMANAGER_HPP_
