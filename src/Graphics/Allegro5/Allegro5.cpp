// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5
*/
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include "Allegro5/Allegro5.hpp"
#include "Models/ColorType.hpp"

Allegro5::~Allegro5() {
    stop();
}

void Allegro5::init(float x, float y) {
    if (!al_init()) {
        throw std::runtime_error("Failed to initialize Allegro");
    }

    al_install_keyboard();
    al_init_image_addon();

    _window.createWindow(static_cast<int>(x), static_cast<int>(y));
    _event.init();
    _text.init();

    _event.registerDisplayEventSource(_window.getDisplay());
}

void Allegro5::stop() {
}

void Allegro5::clearScreen() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

void Allegro5::refreshScreen() {
    al_flip_display();
}

void Allegro5::drawEntity(int x, int y, char symbol) {
}

void Allegro5::drawTexture(int x, int y, const std::string& texturePath) {
    auto bitmap = _texture.loadBitmap(texturePath);
    if (!bitmap)
        return;

    al_draw_bitmap(bitmap.get(), x, y, 0);
}

void Allegro5::drawText(const std::string& text, int x,
int y, Arcade::Color color) {
    _text.drawText(x, y, text);
}

void Allegro5::pollEvents() {
    _event.pollEvents();
}

bool Allegro5::isOpen() const {
    return _event.isRunning();
}

const std::string& Allegro5::getName() const {
    return _name;
}

int Allegro5::getWidth() const {
    return al_get_display_width(_window.getDisplay());
}

int Allegro5::getHeight() const {
    return al_get_display_height(_window.getDisplay());
}

bool Allegro5::isKeyPressed(int keyCode) {
    _event.registerEventSource();
    return _event.isKeyPressed(keyCode);
}

bool Allegro5::isMouseButtonPressed(int button) const {
    _event.registerEventSource();
    return _event.isMouseButtonPressed(button);
}

std::pair<size_t, size_t> Allegro5::getMousePosition() const {
    _event.registerEventSource();
    return _event.getMousePosition();
}

extern "C" {
    __attribute__((constructor))
    const char* init_allegro5(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_allegro5(void) {
    }

    Arcade::IDisplayModule* entryPoint(void) {
        return new Allegro5();
    }
}
