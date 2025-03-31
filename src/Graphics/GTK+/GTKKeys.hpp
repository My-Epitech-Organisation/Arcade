// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** GTK+ Key Mapping
*/

#ifndef SRC_GRAPHICS_GTK__GTKKEYS_HPP_
    #define SRC_GRAPHICS_GTK__GTKKEYS_HPP_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <unordered_map>
#include "Models/KeysType.hpp"
#include "Models/MouseButtonType.hpp"

namespace Arcade {
class GTKKeyMap {
 private:
    static const std::unordered_map<Keys, guint> _keyMap;
    static const std::unordered_map<MouseButton, guint> _mouseButtonMap;

 public:
    static guint getGTKKey(Keys key) {
        auto it = _keyMap.find(key);
        return (it != _keyMap.end()) ? it->second : 0;
    }

    static guint getGTKButton(MouseButton button) {
        auto it = _mouseButtonMap.find(button);
        return (it != _mouseButtonMap.end()) ? it->second : 0;
    }
};

const std::unordered_map<Keys, guint> GTKKeyMap::_keyMap = {
    {Keys::UP, GDK_KEY_Up},
    {Keys::DOWN, GDK_KEY_Down},
    {Keys::LEFT, GDK_KEY_Left},
    {Keys::RIGHT, GDK_KEY_Right},
    {Keys::ENTER, GDK_KEY_Return},
    {Keys::SPACE, GDK_KEY_space},
    {Keys::ESC, GDK_KEY_Escape},
    {Keys::A, GDK_KEY_a},
    {Keys::B, GDK_KEY_b},
    {Keys::C, GDK_KEY_c},
    {Keys::D, GDK_KEY_d},
    {Keys::E, GDK_KEY_e},
    {Keys::F, GDK_KEY_f},
    {Keys::G, GDK_KEY_g},
    {Keys::H, GDK_KEY_h},
    {Keys::I, GDK_KEY_i},
    {Keys::J, GDK_KEY_j},
    {Keys::K, GDK_KEY_k},
    {Keys::L, GDK_KEY_l},
    {Keys::M, GDK_KEY_m},
    {Keys::N, GDK_KEY_n},
    {Keys::O, GDK_KEY_o},
    {Keys::P, GDK_KEY_p},
    {Keys::Q, GDK_KEY_q},
    {Keys::R, GDK_KEY_r},
    {Keys::S, GDK_KEY_s},
    {Keys::T, GDK_KEY_t},
    {Keys::U, GDK_KEY_u},
    {Keys::V, GDK_KEY_v},
    {Keys::W, GDK_KEY_w},
    {Keys::X, GDK_KEY_x},
    {Keys::Y, GDK_KEY_y},
    {Keys::Z, GDK_KEY_z},
    {Keys::NUM0, GDK_KEY_0},
    {Keys::NUM1, GDK_KEY_1},
    {Keys::NUM2, GDK_KEY_2},
    {Keys::NUM3, GDK_KEY_3},
    {Keys::NUM4, GDK_KEY_4},
    {Keys::NUM5, GDK_KEY_5},
    {Keys::NUM6, GDK_KEY_6},
    {Keys::NUM7, GDK_KEY_7},
    {Keys::NUM8, GDK_KEY_8},
    {Keys::NUM9, GDK_KEY_9},
    // French AZERTY keyboard specific mappings
    {Keys::NUM1, GDK_KEY_ampersand},
    {Keys::NUM2, GDK_KEY_eacute},
    {Keys::NUM3, GDK_KEY_quotedbl},
    {Keys::NUM4, GDK_KEY_apostrophe},
    {Keys::NUM5, GDK_KEY_parenleft},
    {Keys::NUM6, GDK_KEY_minus},
    {Keys::NUM6, GDK_KEY_egrave},
    {Keys::NUM7, GDK_KEY_underscore},
    {Keys::NUM7, GDK_KEY_grave},
    {Keys::NUM8, GDK_KEY_ccedilla},
    {Keys::NUM9, GDK_KEY_agrave},
    {Keys::NUM0, GDK_KEY_parenright}
};

const std::unordered_map<MouseButton, guint> GTKKeyMap::_mouseButtonMap = {
    {MouseButton::LEFT, 1},
    {MouseButton::MIDDLE, 2},
    {MouseButton::RIGHT, 3},
    {MouseButton::X1, 4},
    {MouseButton::X2, 5}
};

}  // namespace Arcade

#endif  // SRC_GRAPHICS_GTK__GTKKEYS_HPP_
