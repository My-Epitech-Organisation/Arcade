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
#include "Models/KeysType.hpp"
#include "Models/MouseButtonType.hpp"

struct RawInputState {
    std::unordered_map<Arcade::Keys, bool> keyStates;
    std::unordered_map<Arcade::MouseButton, bool> mouseButtons;
    int mouseX = 0;
    int mouseY = 0;
};


#endif  // SRC_SHARED_EVENTMANAGER_KEYEVENT_RAWINPUTSTATE_HPP_
