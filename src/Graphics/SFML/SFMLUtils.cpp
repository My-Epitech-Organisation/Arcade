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


void SFML::createWindow(int width, int height) {
    _window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(width, height),
        "Arcade",
        sf::Style::Close);

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
