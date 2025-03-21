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

#pragma once
#include <cstddef>

/// Key definitions for user input handling.
#define KEY_UP 0
#define KEY_DOWN 1
#define KEY_LEFT 2
#define KEY_RIGHT 3
#define KEY_A 4
#define KEY_B 5
#define KEY_C 6
#define KEY_D 7
#define KEY_E 8
#define KEY_F 9
#define KEY_G 10
#define KEY_H 11
#define KEY_I 12
#define KEY_J 13
#define KEY_K 14
#define KEY_L 15
#define KEY_M 16
#define KEY_N 17
#define KEY_O 18
#define KEY_P 19
#define KEY_Q 20
#define KEY_R 21
#define KEY_S 22
#define KEY_T 23
#define KEY_U 24
#define KEY_V 25
#define KEY_W 26
#define KEY_X 27
#define KEY_Y 28
#define KEY_Z 29
#define KEY_0 30
#define KEY_1 31
#define KEY_2 32
#define KEY_3 33
#define KEY_4 34
#define KEY_5 35
#define KEY_6 36
#define KEY_7 37
#define KEY_8 38
#define KEY_9 39
#define KEY_SPACE 40
#define KEY_ENTER 41
#define KEY_ESCAPE 42
#define KEY_BACKSPACE 43
#define KEY_TAB 44
#define KEY_SHIFT 45
#define KEY_CTRL 46
#define KEY_ALT 47
#define KEY_SUPER 48
#define LEFT_CLICK 49
#define RIGHT_CLICK 50
#define MIDDLE_CLICK 51
#define WINDOW_CLOSE 52

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
     * @brief Virtual destructor for IEventManager.
     */
    virtual ~IEventManager() = default;

    /**
     * @brief Checks if a specific event is active.
     *
     * This method allows querying the state of a particular key or event.
     *
     * @param key The key/event identifier.
     * @return True if the event is active, false otherwise.
     */
    virtual bool getEventState(int key) = 0;

    /**
     * @brief Sets the state of a specific event.
     *
     * This method enables or disables a given key or event state.
     *
     * @param key The key/event identifier.
     * @param state The new state of the event (true = active, false = inactive).
     */
    virtual void setEventState(int key, bool state) = 0;

    /**
     * @brief Retrieves the current X-coordinate of the mouse.
     *
     * @return The X-position of the mouse cursor.
     */
    virtual std::size_t getMouseX() const = 0;

    /**
     * @brief Retrieves the current Y-coordinate of the mouse.
     *
     * @return The Y-position of the mouse cursor.
     */
    virtual std::size_t getMouseY() const = 0;

    /**
     * @brief Polls for new events.
     *
     * This method updates the event states by checking for new input from the user.
     */
    virtual void pollEvents() = 0;
};

}  // namespace Arcade
