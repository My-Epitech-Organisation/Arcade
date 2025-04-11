// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** EventManager
*/

#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <utility>
#include <iostream>
#include "Shared/Models/MouseButtonType.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"
#include "EventManager/KeyEvent/KeyEvent.hpp"
#include "EventManager/KeyEvent/MouseEvent.hpp"
#include "EventManager/KeyEvent/RawInputState.hpp"
#include "Shared/Interface/Display/IDisplayModule.hpp"
#include "Shared/Models/KeysType.hpp"
#include "Shared/Exceptions/Exceptions.hpp"

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
class EventManager : public IEventManager {
 public:
    // Hash structures for event subscription maps
    struct PairHash {
        std::size_t operator()(const std::pair<EventType, Keys>& p)
            const noexcept {
            return std::hash<int>()(static_cast<int>(p.first)) ^
                std::hash<int>()(static_cast<int>(p.second));
        }
    };

    struct MousePairHash {
      std::size_t operator()(const std::pair<EventType, MouseButton>& p)
      const noexcept {
         return std::hash<int>()(static_cast<int>(p.first)) ^
            std::hash<int>()(static_cast<int>(p.second));
      }
    };

   /**
    * @brief Constructs an EventManager object.
    */
    EventManager();

   /**
    * @brief Default destructor for EventManager.
    */
    ~EventManager() override = default;

   /**
    * @brief Subscribe to a keyboard event with a callback function.
    *
    * @param eventType The keyboard event to subscribe to.
    * @param callback The function to call when the event occurs.
    * @throws InputException if the event type is invalid
    */
    void subscribe(const IEvent& eventType,
        const Callback callback) override {
        auto found = std::pair<EventType, Keys>
            (eventType.getType(), eventType.getKey());
        if (dynamic_cast<const MouseEvent*>(&eventType) != nullptr) {
            auto mouseEvent = dynamic_cast<const MouseEvent*>(&eventType);
            auto mouseFound = std::pair<EventType, MouseButton>
                (mouseEvent->getType(), mouseEvent->getButton());
            _mouseSubscribers[mouseFound].push_back(callback);
            return;
        } else if (dynamic_cast<const KeyEvent*>(&eventType) != nullptr) {
            auto keyEvent = dynamic_cast<const KeyEvent*>(&eventType);
            auto keyFound = std::pair<EventType, Keys>
                (eventType.getType(), eventType.getKey());
            _subscribers[keyFound].push_back(callback);
            return;
        } else {
            throw InputException("Invalid event type for subscription");
        }
    }

   /**
    * @brief Publish a keyboard event to all subscribers.
    *
    * @param eventType The keyboard event to publish.
    * @throws InputException if there's an error during event publishing
    */
    void publish(const IEvent& eventType) override;

   /**
    * @brief Updates the input state based on raw input data.
    *
    * @param state The raw input state containing the current state of keys and mouse buttons.
    */
    void updateInputState(const RawInputState& state) override;

   /**
    * @brief Sets the pressed state of a specific key.
    *
    * @param key The key to update.
    * @param pressed True if the key is pressed, false otherwise.
    */
    void setKeyPressed(Keys key, bool pressed);

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
    void setMousePosition(std::size_t x, std::size_t y) override;

   /**
    * @brief Retrieves the current position of the mouse.
    *
    * @return A pair containing the x and y coordinates of the mouse position.
    */
    std::pair<std::size_t, std::size_t> getMousePosition() const override;

    void unsubscribeAll() override;

 private:
   /**
    * @brief Maps keyboard event types to their subscriber callbacks.
    */
    std::unordered_map<std::pair<EventType, Keys>,
        std::vector<Callback>, PairHash> _subscribers;

   /**
    * @brief Maps mouse event types to their subscriber callbacks.
    */
    std::unordered_map<std::pair<EventType, MouseButton>,
        std::vector<Callback>, MousePairHash> _mouseSubscribers;

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

   /**
    * @brief Reference to the display module.
    */
    std::shared_ptr<IDisplayModule> _displayModule;

   /**
    * @brief Updates the state of keyboard keys from raw input.
    * @throws InputException if there's an error during key state update
    */
    void updateKeyState(const RawInputState& state);

   /**
    * @brief Updates the state of mouse buttons from raw input.
    * @throws InputException if there's an error during mouse button update
    */
    void updateMouseButtonState(const RawInputState& state);

   /**
    * @brief Updates the mouse position from raw input.
    * @throws InputException if there's an error during mouse position update
    */
    void updateMousePosition(const RawInputState& state);
    void safeInvokeCallbacks(const std::vector<Callback>& callbacks) {
        for (const auto& callback : callbacks) {
            if (callback) {
                callback();
            }
        }
    }
};

}  // namespace Arcade
