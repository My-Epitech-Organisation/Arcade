// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** EventManager
*/

#ifndef SRC_CORE_EVENTMANAGER_EVENTMANAGER_HPP_
#define SRC_CORE_EVENTMANAGER_EVENTMANAGER_HPP_

#include <memory>
#include <unordered_map>
#include <utility>
#include "Models/MouseButtonType.hpp"
#include "EventManager/AEventManager.hpp"
#include "EventManager/KeyEvent/KeyEvent.hpp"
#include "EventManager/KeyEvent/RawInputState.hpp"
#include "Interface/Display/IDisplayModule.hpp"
#include "Models/KeysType.hpp"

/**
 * @file EventManager.hpp
 * @brief Declaration of the EventManager class for handling input events in the Arcade project.
 */

namespace Arcade {

/**
 * @class EventManager
 * @brief Manages input events such as keyboard and mouse interactions.
 *
 * The EventManager class is responsible for tracking the state of keys and mouse buttons,
 * as well as the position of the mouse. It provides methods to update and query the state
 * of inputs, enabling interaction within the Arcade application.
 */
class EventManager : public AEventManager {
 public:
   /**
    * @brief Constructs an EventManager object.
    */
    EventManager();

   /**
    * @brief Default destructor for EventManager.
    */
    ~EventManager() override = default;

   /**
    * @brief Updates the input state based on raw input data.
    *
    * @param state The raw input state containing the current state of keys and mouse buttons.
    */
    void updateInputState(const RawInputState& state);

   /**
    * @brief Sets the pressed state of a specific key.
    *
    * @param key The key to update.
    * @param pressed True if the key is pressed, false otherwise.
    */
    void setKeyPressed(Keys key, bool pressed) override;

   /**
    * @brief Checks if a specific key is currently pressed.
    *
    * @param key The key to check.
    * @return True if the key is pressed, false otherwise.
    */
    bool isKeyPressed(Keys key) const override;

   /**
    * @brief Sets the current position of the mouse.
    *
    * @param x The x-coordinate of the mouse position.
    * @param y The y-coordinate of the mouse position.
    */
    void setMousePosition(std::size_t x, std::size_t y);

   /**
    * @brief Retrieves the current position of the mouse.
    *
    * @return A pair containing the x and y coordinates of the mouse position.
    */
    std::pair<std::size_t, std::size_t> getMousePosition() const;

 private:
   /**
    * @brief Stores the pressed state of keys.
    */
    std::unordered_map<Keys, bool> _keyStates;

   /**
    * @brief Stores the pressed state of mouse buttons.
    */
    std::unordered_map<MouseButton, bool> _mouseButtonStates;

   /**
    * @brief The x-coordinate of the mouse position.
    */
    std::size_t _mouseX;

   /**
    * @brief The y-coordinate of the mouse position.
    */
    std::size_t _mouseY;
    void updateKeyState(const RawInputState& state);
    void updateMouseButtonState(const RawInputState& state);
    void updateMousePosition(const RawInputState& state);
};

}  // namespace Arcade

#endif  // SRC_CORE_EVENTMANAGER_EVENTMANAGER_HPP_
