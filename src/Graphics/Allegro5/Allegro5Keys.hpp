// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Allegro5 key mapping
*/

#ifndef SRC_GRAPHICS_ALLEGRO5_ALLEGRO5KEYS_HPP_
    #define SRC_GRAPHICS_ALLEGRO5_ALLEGRO5KEYS_HPP_
    #include <allegro5/allegro.h>
    #include <unordered_map>
    #include "Models/KeysType.hpp"
    #include "Models/MouseButtonType.hpp"

namespace Arcade {

class Allegro5KeyMap {
 private:
    static const std::unordered_map<Keys, int> _keyMap;
    static const std::unordered_map<MouseButton, int> _mouseButtonMap;

 public:
    static int getAllegroKey(Keys key) {
        auto it = _keyMap.find(key);
        return (it != _keyMap.end()) ? it->second : ALLEGRO_KEY_UNKNOWN;
    }

    static int getAllegroButton(MouseButton button) {
        auto it = _mouseButtonMap.find(button);
        return (it != _mouseButtonMap.end()) ? it->second : 0;
    }
};

// Define the static mapping
const std::unordered_map<Keys, int> Allegro5KeyMap::_keyMap = {
    {Keys::UP, ALLEGRO_KEY_UP},
    {Keys::DOWN, ALLEGRO_KEY_DOWN},
    {Keys::LEFT, ALLEGRO_KEY_LEFT},
    {Keys::RIGHT, ALLEGRO_KEY_RIGHT},
    {Keys::ENTER, ALLEGRO_KEY_ENTER},
    {Keys::ESC, ALLEGRO_KEY_ESCAPE},
    {Keys::NUM1, ALLEGRO_KEY_1},
    {Keys::NUM2, ALLEGRO_KEY_2},
    {Keys::NUM3, ALLEGRO_KEY_3},
    {Keys::NUM4, ALLEGRO_KEY_4},
    {Keys::A, ALLEGRO_KEY_A},
    {Keys::B, ALLEGRO_KEY_B},
    {Keys::C, ALLEGRO_KEY_C},
    {Keys::D, ALLEGRO_KEY_D},
    {Keys::E, ALLEGRO_KEY_E},
    {Keys::F, ALLEGRO_KEY_F},
    {Keys::G, ALLEGRO_KEY_G},
    {Keys::H, ALLEGRO_KEY_H},
    {Keys::I, ALLEGRO_KEY_I},
    {Keys::J, ALLEGRO_KEY_J},
    {Keys::K, ALLEGRO_KEY_K},
    {Keys::L, ALLEGRO_KEY_L},
    {Keys::M, ALLEGRO_KEY_M},
    {Keys::N, ALLEGRO_KEY_N},
    {Keys::O, ALLEGRO_KEY_O},
    {Keys::P, ALLEGRO_KEY_P},
    {Keys::Q, ALLEGRO_KEY_Q},
    {Keys::R, ALLEGRO_KEY_R},
    {Keys::S, ALLEGRO_KEY_S},
    {Keys::T, ALLEGRO_KEY_T},
    {Keys::U, ALLEGRO_KEY_U},
    {Keys::V, ALLEGRO_KEY_V},
    {Keys::W, ALLEGRO_KEY_W},
    {Keys::X, ALLEGRO_KEY_X},
    {Keys::Y, ALLEGRO_KEY_Y},
    {Keys::Z, ALLEGRO_KEY_Z}
};

const std::unordered_map<MouseButton, int> Allegro5KeyMap::_mouseButtonMap = {
    {MouseButton::LEFT, 1},
    {MouseButton::MIDDLE, 3},
    {MouseButton::RIGHT, 2},
    {MouseButton::X1, 4},
    {MouseButton::X2, 5}
};

}  // namespace Arcade

#endif  // SRC_GRAPHICS_ALLEGRO5_ALLEGRO5KEYS_HPP_
