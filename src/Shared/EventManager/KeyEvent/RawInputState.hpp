// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Raw input state
*/

#ifndef SRC_SHARED_EVENTMANAGER_KEYEVENT_RAWINPUTSTATE_HPP_
    #define SRC_SHARED_EVENTMANAGER_KEYEVENT_RAWINPUTSTATE_HPP_

#include <unordered_map>
#include "../../Models/KeysType.hpp"
#include "../../Models/MouseButtonType.hpp"

/**
 * @file RawInputState.hpp
 * @brief Defines the RawInputState structure for managing input states.
 */

/**
 * @struct RawInputState
 * @brief Represents the state of raw input devices such as keyboard and mouse.
 *
 * This structure is used to store the current state of keys and mouse buttons,
 * as well as the position of the mouse cursor. It is designed to facilitate
 * input handling in the Arcade project.
 *
 * @var RawInputState::keyStates
 * A map that associates each key (of type Arcade::Keys) with its current state.
 * The state is represented as a boolean value, where `true` indicates that the
 * key is pressed, and `false` indicates that it is not pressed.
 *
 * @var RawInputState::mouseButtons
 * A map that associates each mouse button (of type Arcade::MouseButton) with
 * its current state. The state is represented as a boolean value, where `true`
 * indicates that the button is pressed, and `false` indicates that it is not
 * pressed.
 *
 * @var RawInputState::mouseX
 * The current X-coordinate of the mouse cursor, represented as an integer.
 *
 * @var RawInputState::mouseY
 * The current Y-coordinate of the mouse cursor, represented as an integer.
 */
struct RawInputState {
    std::unordered_map<Arcade::Keys, bool> keyStates;
    std::unordered_map<Arcade::MouseButton, bool> mouseButtons;
    int mouseX = 0;
    int mouseY = 0;
};


#endif  // SRC_SHARED_EVENTMANAGER_KEYEVENT_RAWINPUTSTATE_HPP_
