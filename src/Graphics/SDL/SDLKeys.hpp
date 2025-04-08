// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** SDL key mapping
*/

#ifndef SRC_GRAPHICS_SDL_SDLKEYS_HPP_
    #define SRC_GRAPHICS_SDL_SDLKEYS_HPP_
    #include <SDL2/SDL_keycode.h>
    #include <unordered_map>
    #include "Models/KeysType.hpp"
    #include "Models/MouseButtonType.hpp"

namespace Arcade {

class SDLKeyMap {
 private:
    static const std::unordered_map<Keys, SDL_Keycode> _keyMap;
    static const std::unordered_map<MouseButton, Uint8> _mouseButtonMap;

 public:
    static SDL_Keycode getSDLKey(Keys key) {
        auto it = _keyMap.find(key);
        return (it != _keyMap.end()) ? it->second : SDLK_UNKNOWN;
    }

    static Uint8 getSDLButton(MouseButton button) {
        auto it = _mouseButtonMap.find(button);
        return (it != _mouseButtonMap.end()) ? it->second : 0;
    }

    static SDL_MouseButtonEvent getSDLMouseButton(MouseButton key) {
        auto it = _mouseButtonMap.find(key);
        SDL_MouseButtonEvent defaultEvent = {};
        return (it != _mouseButtonMap.end()) ?
            SDL_MouseButtonEvent{it->second} : defaultEvent;
    }
};

// Define the static mapping
const std::unordered_map<Keys, SDL_Keycode> SDLKeyMap::_keyMap = {
    {Keys::UP, SDLK_UP},
    {Keys::DOWN, SDLK_DOWN},
    {Keys::LEFT, SDLK_LEFT},
    {Keys::RIGHT, SDLK_RIGHT},
    {Keys::ENTER, SDLK_RETURN},
    {Keys::ESC, SDLK_ESCAPE},
    {Keys::BACKSPACE, SDLK_BACKSPACE},  // Add this mapping
    {Keys::NUM0, SDLK_0},
    {Keys::NUM1, SDLK_1},
    {Keys::NUM2, SDLK_2},
    {Keys::NUM3, SDLK_3},
    {Keys::NUM4, SDLK_4},
    {Keys::NUM5, SDLK_5},  // Make sure this is mapped
    {Keys::A, SDLK_a},
    {Keys::B, SDLK_b},
    {Keys::C, SDLK_c},
    {Keys::D, SDLK_d},
    {Keys::E, SDLK_e},
    {Keys::F, SDLK_f},
    {Keys::G, SDLK_g},
    {Keys::H, SDLK_h},
    {Keys::I, SDLK_i},
    {Keys::J, SDLK_j},
    {Keys::K, SDLK_k},
    {Keys::L, SDLK_l},
    {Keys::M, SDLK_m},
    {Keys::N, SDLK_n},
    {Keys::O, SDLK_o},
    {Keys::P, SDLK_p},
    {Keys::Q, SDLK_q},
    {Keys::R, SDLK_r},
    {Keys::S, SDLK_s},
    {Keys::T, SDLK_t},
    {Keys::U, SDLK_u},
    {Keys::V, SDLK_v},
    {Keys::W, SDLK_w},
    {Keys::X, SDLK_x},
    {Keys::Y, SDLK_y},
    {Keys::Z, SDLK_z}
};

const std::unordered_map<MouseButton, Uint8> SDLKeyMap::_mouseButtonMap = {
    {MouseButton::LEFT, SDL_BUTTON_LEFT},
    {MouseButton::MIDDLE, SDL_BUTTON_MIDDLE},
    {MouseButton::RIGHT, SDL_BUTTON_RIGHT},
    {MouseButton::X1, SDL_BUTTON_X1},
    {MouseButton::X2, SDL_BUTTON_X2}
};

}  // namespace Arcade

#endif  // SRC_GRAPHICS_SDL_SDLKEYS_HPP_
