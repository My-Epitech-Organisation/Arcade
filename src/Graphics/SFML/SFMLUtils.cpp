// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SFMLUtils
*/
#include <string>
#include <memory>
#include <iostream>
#include "SFML/SFML.hpp"
#include "SFML/SFMLKeys.hpp"


void SFML::createWindow(int width, int height) {
    _window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(width, height),
        "Arcade",
        sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);

    if (!_window)
        throw std::runtime_error("SFML: Failed to create window");

    _window->setFramerateLimit(60);
}

sf::Font* SFML::loadFont(const std::string& fontPath) {
    if (_fonts.find(fontPath) == _fonts.end()) {
        _fonts[fontPath] = std::make_unique<sf::Font>();
        if (!_fonts[fontPath]->loadFromFile(fontPath)) {
            std::cerr << "Failed to load font: " << fontPath << std::endl;
            return nullptr;
        }
    }
    return _fonts[fontPath].get();
}

sf::Texture* SFML::loadTexture(const std::string& texturePath) {
    if (_textures.find(texturePath) == _textures.end()) {
        _textures[texturePath] = std::make_unique<sf::Texture>();
        if (!_textures[texturePath]->loadFromFile(texturePath)) {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
            return nullptr;
        }
    }
    return _textures[texturePath].get();
}

const std::string& SFML::getName() const {
    return _name;
}

int SFML::getHeight() const {
    return _windowHeight;
}

int SFML::getWidth() const {
    return _windowWidth;
}

bool SFML::isKeyPressed(int keyCode) {
    auto arcadeKey = static_cast<Arcade::Keys>(keyCode);
    sf::Keyboard::Key sfmlKey = Arcade::SFMLKeyMap::getSFMLKey(arcadeKey);
    return sf::Keyboard::isKeyPressed(sfmlKey);
}

bool SFML::isMouseButtonPressed(int button) const {
    auto arcadeButton = static_cast<Arcade::MouseButton>(button);
    sf::Mouse::Button sfmlButton =
        Arcade::SFMLKeyMap::getSFMLButton(arcadeButton);
    return sf::Mouse::isButtonPressed(sfmlButton);
}

std::pair<size_t, size_t> SFML::getMousePosition() const {
    if (!_window)
        return std::make_pair(0, 0);
    sf::Vector2i position = sf::Mouse::getPosition(*_window);
    position.x = (position.x < 0) ? 0 : position.x;
    position.y = (position.y < 0) ? 0 : position.y;
    position.x = (position.x > _windowWidth) ? _windowWidth : position.x;
    position.y = (position.y > _windowHeight) ? _windowHeight : position.y;
    return std::make_pair(static_cast<size_t>(position.x),
                          static_cast<size_t>(position.y));
}
