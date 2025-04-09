// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** NCurses key mapping
*/

#ifndef SRC_GRAPHICS_NCURSES_NCURSESKEYS_HPP_
    #define SRC_GRAPHICS_NCURSES_NCURSESKEYS_HPP_
    #include <ncurses.h>
    #include <unordered_map>
    #include "Models/KeysType.hpp"
    #include "Models/MouseButtonType.hpp"

namespace Arcade {

class NCursesKeyMap {
 private:
    static const std::unordered_map<Keys, int> _keyMap;
    static const std::unordered_map<MouseButton, unsigned long> _mouseButtonMap;

 public:
    static int getNCursesKey(Keys key) {
        auto it = _keyMap.find(key);
        return (it != _keyMap.end()) ? it->second : ERR;
    }

    static unsigned long getNCursesButton(MouseButton button) {
        auto it = _mouseButtonMap.find(button);
        return (it != _mouseButtonMap.end()) ? it->second : 0;
    }
};

// Define the static mapping
const std::unordered_map<Keys, int> NCursesKeyMap::_keyMap = {
    {Keys::UP, KEY_UP},
    {Keys::DOWN, KEY_DOWN},
    {Keys::LEFT, KEY_LEFT},
    {Keys::RIGHT, KEY_RIGHT},
    {Keys::ENTER, '\n'},
    {Keys::ESC, 27},
    {Keys::NUM1, '1'},
    {Keys::NUM2, '2'},
    {Keys::NUM3, '3'},
    {Keys::NUM4, '4'},
    {Keys::A, 'a'},
    {Keys::B, 'b'},
    {Keys::C, 'c'},
    {Keys::D, 'd'},
    {Keys::E, 'e'},
    {Keys::F, 'f'},
    {Keys::G, 'g'},
    {Keys::H, 'h'},
    {Keys::I, 'i'},
    {Keys::J, 'j'},
    {Keys::K, 'k'},
    {Keys::L, 'l'},
    {Keys::M, 'm'},
    {Keys::N, 'n'},
    {Keys::O, 'o'},
    {Keys::P, 'p'},
    {Keys::Q, 'q'},
    {Keys::R, 'r'},
    {Keys::S, 's'},
    {Keys::T, 't'},
    {Keys::U, 'u'},
    {Keys::V, 'v'},
    {Keys::W, 'w'},
    {Keys::X, 'x'},
    {Keys::Y, 'y'},
    {Keys::Z, 'z'}
};

const std::unordered_map<MouseButton, unsigned long> NCursesKeyMap::_mouseButtonMap = {
    {MouseButton::LEFT, BUTTON1_PRESSED},
    {MouseButton::MIDDLE, BUTTON2_PRESSED},
    {MouseButton::RIGHT, BUTTON3_PRESSED},
    {MouseButton::X1, BUTTON4_PRESSED},
    {MouseButton::X2, 0}  // NCurses doesn't have a standard X2 button mapping
};

}  // namespace Arcade

#endif  // SRC_GRAPHICS_NCURSES_NCURSESKEYS_HPP_
