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

void Allegro5::drawText(int x, int y, const std::string& text) {
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
