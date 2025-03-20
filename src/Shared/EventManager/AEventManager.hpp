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

#pragma once

#include <vector>
#include <utility>
#include "Interface/IEventManager.hpp"

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
    * @brief Retrieves the state of a specific key event.
    *
    * @param key The key/event identifier.
    * @return True if the key is currently active, false otherwise.
    */
    bool getEventState(int key) override;

    /**
    * @brief Sets the state of a specific key event.
    *
    * @param key The key/event identifier.
    * @param state The new state of the key (true = pressed, false = released).
    */
    void setEventState(int key, bool state) override;

    /**
    * @brief Retrieves the current X-coordinate of the mouse.
    *
    * @return The current X position of the mouse cursor.
    */
    std::size_t getMouseX() const override;

    /**
    * @brief Retrieves the current Y-coordinate of the mouse.
    *
    * @return The current Y position of the mouse cursor.
    */
    std::size_t getMouseY() const override;

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
};
