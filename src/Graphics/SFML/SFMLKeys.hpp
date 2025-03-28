// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** SFML key mapping
*/

#ifndef SRC_GRAPHICS_SFML_SFMLKEYS_HPP_
    #define SRC_GRAPHICS_SFML_SFMLKEYS_HPP_
    #include <unordered_map>
    #include <SFML/Window/Keyboard.hpp>
    #include "Models/KeysType.hpp"
    #include "Models/MouseButtonType.hpp"

namespace Arcade {
class SFMLKeyMap {
 private:
    static const std::unordered_map<Keys, sf::Keyboard::Key> _keyMap;
    static const std::unordered_map<MouseButton, sf::Mouse::Button>
        _mouseButtonMap;

 public:
    static sf::Keyboard::Key getSFMLKey(Keys key) {
        auto it = _keyMap.find(key);
        return (it != _keyMap.end()) ? it->second : sf::Keyboard::Unknown;
    }

    static sf::Mouse::Button getSFMLButton(MouseButton button) {
        auto it = _mouseButtonMap.find(button);
        return (it != _mouseButtonMap.end()) ? it->second
            : sf::Mouse::Button::ButtonCount;
    }
};
const std::unordered_map<Keys, sf::Keyboard::Key> SFMLKeyMap::_keyMap = {
    {Keys::UP, sf::Keyboard::Up},
    {Keys::DOWN, sf::Keyboard::Down},
    {Keys::LEFT, sf::Keyboard::Left},
    {Keys::RIGHT, sf::Keyboard::Right},
    {Keys::ENTER, sf::Keyboard::Return},
    {Keys::ESC, sf::Keyboard::Escape},
    {Keys::NUM1, sf::Keyboard::Num1},
    {Keys::NUM2, sf::Keyboard::Num2},
    {Keys::NUM3, sf::Keyboard::Num3},
    {Keys::NUM4, sf::Keyboard::Num4},
    {Keys::A, sf::Keyboard::A},
    {Keys::B, sf::Keyboard::B},
    {Keys::C, sf::Keyboard::C},
    {Keys::D, sf::Keyboard::D},
    {Keys::E, sf::Keyboard::E},
    {Keys::F, sf::Keyboard::F},
    {Keys::G, sf::Keyboard::G},
    {Keys::H, sf::Keyboard::H},
    {Keys::I, sf::Keyboard::I},
    {Keys::J, sf::Keyboard::J},
    {Keys::K, sf::Keyboard::K},
    {Keys::L, sf::Keyboard::L},
    {Keys::M, sf::Keyboard::M},
    {Keys::N, sf::Keyboard::N},
    {Keys::O, sf::Keyboard::O},
    {Keys::P, sf::Keyboard::P},
    {Keys::Q, sf::Keyboard::Q},
    {Keys::R, sf::Keyboard::R},
    {Keys::S, sf::Keyboard::S},
    {Keys::T, sf::Keyboard::T},
    {Keys::U, sf::Keyboard::U},
    {Keys::V, sf::Keyboard::V},
    {Keys::W, sf::Keyboard::W},
    {Keys::X, sf::Keyboard::X},
    {Keys::Y, sf::Keyboard::Y},
    {Keys::Z, sf::Keyboard::Z}
};

const std::unordered_map<MouseButton, sf::Mouse::Button>
SFMLKeyMap::_mouseButtonMap = {
        {MouseButton::LEFT, sf::Mouse::Button::Left},
        {MouseButton::MIDDLE, sf::Mouse::Button::Middle},
        {MouseButton::RIGHT, sf::Mouse::Button::Right},
        {MouseButton::X1, sf::Mouse::Button::XButton1},
        {MouseButton::X2, sf::Mouse::Button::XButton2}
};

}  // namespace Arcade

#endif  // SRC_GRAPHICS_SFML_SFMLKEYS_HPP_
