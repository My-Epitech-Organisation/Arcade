// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** EventType enumeration
*/
/**
 * @file EventType.hpp
 * @brief Defines event types for the Arcade project.
 *
 * This file contains the EventType enumeration used to identify different types
 * of events in the Arcade system.
 */

#ifndef SRC_SHARED_MODELS_EVENTTYPE_HPP_
    #define SRC_SHARED_MODELS_EVENTTYPE_HPP_

namespace Arcade {

/**
* @enum EventType
* @brief Enumeration of supported event types in the Arcade system.
*/
enum class EventType {
    KEY_PRESSED,
    KEY_RELEASED,
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,
    MOUSE_MOVED,
    WINDOW_CLOSED,
    WINDOW_RESIZED,
    GAME_PAUSE,
    GAME_RESUME,
    GAME_EXIT,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SELECT,
    BACK,
    NEXT_GRAPHIC_LIB,
    PREV_GRAPHIC_LIB,
    NEXT_GAME,
    PREV_GAME,
    CUSTOM_EVENT
};

}  // namespace Arcade

#endif  // SRC_SHARED_MODELS_EVENTTYPE_HPP_
