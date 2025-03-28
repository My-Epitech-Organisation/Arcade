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
#include <map>
#include "Models/KeysType.hpp"

class GTKKeys {
 public:
    // Convert from Arcade key to GTK+ key code
    static guint convertKeyToGTK(Arcade::Key key) {
        static const std::map<Arcade::Key, guint> keyMap = {
            {Arcade::Key::UP, GDK_KEY_Up},
            {Arcade::Key::DOWN, GDK_KEY_Down},
            {Arcade::Key::LEFT, GDK_KEY_Left},
            {Arcade::Key::RIGHT, GDK_KEY_Right},
            {Arcade::Key::ENTER, GDK_KEY_Return},
            {Arcade::Key::SPACE, GDK_KEY_space},
            {Arcade::Key::ESCAPE, GDK_KEY_Escape},
            {Arcade::Key::A, GDK_KEY_a},
            {Arcade::Key::B, GDK_KEY_b},
            {Arcade::Key::C, GDK_KEY_c},
            {Arcade::Key::D, GDK_KEY_d},
            {Arcade::Key::E, GDK_KEY_e},
            {Arcade::Key::F, GDK_KEY_f},
            {Arcade::Key::G, GDK_KEY_g},
            {Arcade::Key::H, GDK_KEY_h},
            {Arcade::Key::I, GDK_KEY_i},
            {Arcade::Key::J, GDK_KEY_j},
            {Arcade::Key::K, GDK_KEY_k},
            {Arcade::Key::L, GDK_KEY_l},
            {Arcade::Key::M, GDK_KEY_m},
            {Arcade::Key::N, GDK_KEY_n},
            {Arcade::Key::O, GDK_KEY_o},
            {Arcade::Key::P, GDK_KEY_p},
            {Arcade::Key::Q, GDK_KEY_q},
            {Arcade::Key::R, GDK_KEY_r},
            {Arcade::Key::S, GDK_KEY_s},
            {Arcade::Key::T, GDK_KEY_t},
            {Arcade::Key::U, GDK_KEY_u},
            {Arcade::Key::V, GDK_KEY_v},
            {Arcade::Key::W, GDK_KEY_w},
            {Arcade::Key::X, GDK_KEY_x},
            {Arcade::Key::Y, GDK_KEY_y},
            {Arcade::Key::Z, GDK_KEY_z},
            {Arcade::Key::NUM0, GDK_KEY_0},
            {Arcade::Key::NUM1, GDK_KEY_1},
            {Arcade::Key::NUM2, GDK_KEY_2},
            {Arcade::Key::NUM3, GDK_KEY_3},
            {Arcade::Key::NUM4, GDK_KEY_4},
            {Arcade::Key::NUM5, GDK_KEY_5},
            {Arcade::Key::NUM6, GDK_KEY_6},
            {Arcade::Key::NUM7, GDK_KEY_7},
            {Arcade::Key::NUM8, GDK_KEY_8},
            {Arcade::Key::NUM9, GDK_KEY_9}
        };
        auto it = keyMap.find(key);
        if (it != keyMap.end())
            return it->second;
        return 0;  // No mapping found
    }
    static Arcade::Key convertKeyFromGTK(guint keyval) {
        switch (keyval) {
            case GDK_KEY_Up: return Arcade::Key::UP;
            case GDK_KEY_Down: return Arcade::Key::DOWN;
            case GDK_KEY_Left: return Arcade::Key::LEFT;
            case GDK_KEY_Right: return Arcade::Key::RIGHT;
            case GDK_KEY_Return: return Arcade::Key::ENTER;
            case GDK_KEY_space: return Arcade::Key::SPACE;
            case GDK_KEY_Escape: return Arcade::Key::ESCAPE;
            case GDK_KEY_a: case GDK_KEY_A: return Arcade::Key::A;
            case GDK_KEY_b: case GDK_KEY_B: return Arcade::Key::B;
            case GDK_KEY_c: case GDK_KEY_C: return Arcade::Key::C;
            case GDK_KEY_d: case GDK_KEY_D: return Arcade::Key::D;
            case GDK_KEY_e: case GDK_KEY_E: return Arcade::Key::E;
            case GDK_KEY_f: case GDK_KEY_F: return Arcade::Key::F;
            case GDK_KEY_g: case GDK_KEY_G: return Arcade::Key::G;
            case GDK_KEY_h: case GDK_KEY_H: return Arcade::Key::H;
            case GDK_KEY_i: case GDK_KEY_I: return Arcade::Key::I;
            case GDK_KEY_j: case GDK_KEY_J: return Arcade::Key::J;
            case GDK_KEY_k: case GDK_KEY_K: return Arcade::Key::K;
            case GDK_KEY_l: case GDK_KEY_L: return Arcade::Key::L;
            case GDK_KEY_m: case GDK_KEY_M: return Arcade::Key::M;
            case GDK_KEY_n: case GDK_KEY_N: return Arcade::Key::N;
            case GDK_KEY_o: case GDK_KEY_O: return Arcade::Key::O;
            case GDK_KEY_p: case GDK_KEY_P: return Arcade::Key::P;
            case GDK_KEY_q: case GDK_KEY_Q: return Arcade::Key::Q;
            case GDK_KEY_r: case GDK_KEY_R: return Arcade::Key::R;
            case GDK_KEY_s: case GDK_KEY_S: return Arcade::Key::S;
            case GDK_KEY_t: case GDK_KEY_T: return Arcade::Key::T;
            case GDK_KEY_u: case GDK_KEY_U: return Arcade::Key::U;
            case GDK_KEY_v: case GDK_KEY_V: return Arcade::Key::V;
            case GDK_KEY_w: case GDK_KEY_W: return Arcade::Key::W;
            case GDK_KEY_x: case GDK_KEY_X: return Arcade::Key::X;
            case GDK_KEY_y: case GDK_KEY_Y: return Arcade::Key::Y;
            case GDK_KEY_z: case GDK_KEY_Z: return Arcade::Key::Z;
            case GDK_KEY_0: return Arcade::Key::NUM0;
            case GDK_KEY_1: return Arcade::Key::NUM1;
            case GDK_KEY_2: return Arcade::Key::NUM2;
            case GDK_KEY_3: return Arcade::Key::NUM3;
            case GDK_KEY_4: return Arcade::Key::NUM4;
            case GDK_KEY_5: return Arcade::Key::NUM5;
            case GDK_KEY_6: return Arcade::Key::NUM6;
            case GDK_KEY_7: return Arcade::Key::NUM7;
            case GDK_KEY_8: return Arcade::Key::NUM8;
            case GDK_KEY_9: return Arcade::Key::NUM9;
            default: return Arcade::Key::UNKNOWN;
        }
    }
};

#endif  // SRC_GRAPHICS_GTK__GTKKEYS_HPP_
