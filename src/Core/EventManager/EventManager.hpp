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

namespace Arcade {

class EventManager : public AEventManager {
 public:
    EventManager();
    ~EventManager() override = default;
    void updateInputState(const RawInputState& state);
    void setKeyPressed(Keys key, bool pressed) override;
    bool isKeyPressed(Keys key) const override;
    void setMousePosition(std::size_t x, std::size_t y);
    std::pair<std::size_t, std::size_t> getMousePosition() const;

 private:
    std::unordered_map<Keys, bool> _keyStates;
    std::unordered_map<MouseButton, bool> _mouseButtonStates;
    std::size_t _mouseX;
    std::size_t _mouseY;
};

}  // namespace Arcade

#endif  // SRC_CORE_EVENTMANAGER_EVENTMANAGER_HPP_
