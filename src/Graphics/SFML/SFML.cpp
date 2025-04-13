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
#include "SFML/SFMLColor.hpp"
#include "Interface/IArcadeModule.hpp"
#include "Models/ModuleInfos.hpp"
#include "Shared/Interface/Core/IWindowModule.hpp"

SFML::~SFML() {
    _window.reset();
    _fonts.clear();
    _textures.clear();
}

void SFML::init(const Arcade::IWindowModule& windowParam) {
    int width = static_cast<int>(windowParam.getWidth());
    int height = static_cast<int>(windowParam.getHeight());
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

void SFML::drawEntity(int x, int y, char symbol, float scale) {
}

void SFML::drawTexture(int x, int y,
const std::string& texturePath, float scale) {
    auto texture = loadTexture(texturePath);
    if (!texture) {
        return;
    }

    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
    sprite.setScale(scale, scale);
    _window->draw(sprite);
}

void SFML::drawDrawable(std::shared_ptr<Arcade::IDrawableComponent> drawable) {
    if (!drawable->isRenderable())
        return;
    float scale;
    if (drawable->getScale() != 1.0f) {
        scale = drawable->getScale();
    } else {
        // Remplacer 0.1f par 1.0f pour correspondre à votre implémentation
        scale = 1.0f;
    }
    // Ajuster les dimensions dans leurs fonctions de rendu pour compenser
    // le changement d'échelle (multiplier par 10)
    if (drawable->shouldRenderAsTexture()) {
        // Multiplier la taille des textures par 10 pour compenser l'échelle
        drawTexture(static_cast<int>(drawable->getPositionX()),
            static_cast<int>(drawable->getPositionY()),
            drawable->getPath(), scale);
    } else if (drawable->shouldRenderAsText()) {
        // Ajuster la taille du texte en conséquence
        drawText(drawable->getText(),
            static_cast<int>(drawable->getPositionX()),
            static_cast<int>(drawable->getPositionY()),
            drawable->getColor(), scale);
    } else if (drawable->shouldRenderAsCharacter()) {
        // Ajuster la taille du caractère en conséquence
        drawEntity(static_cast<int>(drawable->getPositionX()),
            static_cast<int>(drawable->getPositionY()),
            drawable->getCharacter(), scale);
    }
}

void SFML::drawText(const std::string& text, int x, int y,
    Arcade::Color color, float scale) {
    auto font = loadFont("assets/fonts/arial.ttf");
    if (!font)
        return;

    sf::Text sfText;
    sfText.setFont(*font);
    sfText.setString(text);
    sfText.setCharacterSize(scale);
    sfText.setFillColor(SFMLColor::convertColor(color));
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

extern "C" __attribute__((constructor)) const char* init_sfml(void) {
    return "Lib";
}

extern "C" __attribute__((destructor)) void fini_sfml(void) {}


extern "C" Arcade::IArcadeModule* entryPoint(void) {
    return new SFML();
}

extern "C" Arcade::ModuleInfos module_infos() {
    return {"SFML", "IDK",
        "IDK",
        "./lib/arcade_sfml.so", Arcade::ModuleType::GRAPHIC_LIB};
}
