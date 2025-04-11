// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Qt5 key mapping
*/

#ifndef SRC_GRAPHICS_QT5_QT5KEYS_HPP_
    #define SRC_GRAPHICS_QT5_QT5KEYS_HPP_
    #include <Qt>
    #include <unordered_map>
    #include "Models/KeysType.hpp"
    #include "Models/MouseButtonType.hpp"

namespace Arcade {
class Qt5KeyMap {
 private:
    static const std::unordered_map<Keys, int> _keyMap;
    static const std::unordered_map<MouseButton, int> _mouseButtonMap;

 public:
    static int getQtKey(Keys key) {
        auto it = _keyMap.find(key);
        return (it != _keyMap.end()) ? it->second : 0;
    }

    static int getQtButton(MouseButton button) {
        auto it = _mouseButtonMap.find(button);
        return (it != _mouseButtonMap.end()) ? it->second : 0;
    }
};

const std::unordered_map<Keys, int> Qt5KeyMap::_keyMap = {
    {Keys::UP, Qt::Key_Up},
    {Keys::DOWN, Qt::Key_Down},
    {Keys::LEFT, Qt::Key_Left},
    {Keys::RIGHT, Qt::Key_Right},
    {Keys::ENTER, Qt::Key_Return},
    {Keys::ESC, Qt::Key_Escape},
    {Keys::BACKSPACE, Qt::Key_Backspace},
    {Keys::NUM0, Qt::Key_0},
    {Keys::NUM1, Qt::Key_1},
    {Keys::NUM2, Qt::Key_2},
    {Keys::NUM3, Qt::Key_3},
    {Keys::NUM4, Qt::Key_4},
    {Keys::NUM5, Qt::Key_5},
    {Keys::NUM6, Qt::Key_6},
    {Keys::NUM7, Qt::Key_7},
    {Keys::NUM8, Qt::Key_8},
    {Keys::NUM9, Qt::Key_9},
    {Keys::A, Qt::Key_A},
    {Keys::B, Qt::Key_B},
    {Keys::C, Qt::Key_C},
    {Keys::D, Qt::Key_D},
    {Keys::E, Qt::Key_E},
    {Keys::F, Qt::Key_F},
    {Keys::G, Qt::Key_G},
    {Keys::H, Qt::Key_H},
    {Keys::I, Qt::Key_I},
    {Keys::J, Qt::Key_J},
    {Keys::K, Qt::Key_K},
    {Keys::L, Qt::Key_L},
    {Keys::M, Qt::Key_M},
    {Keys::N, Qt::Key_N},
    {Keys::O, Qt::Key_O},
    {Keys::P, Qt::Key_P},
    {Keys::Q, Qt::Key_Q},
    {Keys::R, Qt::Key_R},
    {Keys::S, Qt::Key_S},
    {Keys::T, Qt::Key_T},
    {Keys::U, Qt::Key_U},
    {Keys::V, Qt::Key_V},
    {Keys::W, Qt::Key_W},
    {Keys::X, Qt::Key_X},
    {Keys::Y, Qt::Key_Y},
    {Keys::Z, Qt::Key_Z}
};

const std::unordered_map<MouseButton, int> Qt5KeyMap::_mouseButtonMap = {
    {MouseButton::LEFT, Qt::LeftButton},
    {MouseButton::MIDDLE, Qt::MiddleButton},
    {MouseButton::RIGHT, Qt::RightButton},
    {MouseButton::X1, Qt::XButton1},
    {MouseButton::X2, Qt::XButton2}
};

}  // namespace Arcade

#endif  // SRC_GRAPHICS_QT5_QT5KEYS_HPP_
