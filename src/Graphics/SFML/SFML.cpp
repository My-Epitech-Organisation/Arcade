// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** SFML
*/
#include "SFML/SFML.hpp"
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>

SFML::~SFML() {
    _window.reset();
    _fonts.clear();
    _textures.clear();
}

void SFML::init(float x, float y) {
    int width = static_cast<int>(x);
    int height = static_cast<int>(y);
    createWindow(width, height);
    _windowWidth = width;
    _windowHeight = height;
}

void SFML::stop() {
    if (_window && _window->isOpen()) {
        _window->close();
    }
    _window.reset();
}

void SFML::clearScreen() {
    if (_window) {
        _window->clear(sf::Color::Black);
    }
}

void SFML::refreshScreen() {
    if (_window) {
        _window->display();
    }
}

void SFML::drawEntity(int x, int y, char symbol) {
}

void SFML::drawTexture(int x, int y, const std::string& texturePath) {
    auto texture = loadTexture(texturePath);
    if (!texture) {
        return;
    }

    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sprite.setPosition(static_cast<float>(x), static_cast<float>(y));

    _window->draw(sprite);
}

void SFML::drawText(int x, int y, const std::string& text) {
    auto font = loadFont("assets/fonts/arial.ttf");
    if (!font)
        return;

    sf::Text sfText;
    sfText.setFont(*font);
    sfText.setString(text);
    sfText.setCharacterSize(24);
    sfText.setFillColor(sf::Color::White);
    sfText.setPosition(static_cast<float>(x), static_cast<float>(y));

    _window->draw(sfText);
}

void SFML::pollEvents() {
    sf::Event event;
    while (_window && _window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _running = false;
            _window->close();
        }
    }
}

bool SFML::isOpen() const {
    return _running && _window && _window->isOpen();
}

extern "C" {
    __attribute__((constructor))
    const char* init_sfml(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_sfml(void) {
    }

    Arcade::IDisplayModule* entryPoint(void) {
        return new SFML();
    }
}