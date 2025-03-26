// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5Window
*/
#include <stdexcept>
#include <iostream>
#include <memory>
#include "Allegro5Utils/Allegro5Window.hpp"

namespace Allegro {

Allegro5Window::Allegro5Window()
: _display(nullptr, al_destroy_display), _width(0), _height(0) {
}

void Allegro5Window::createWindow(int width, int height) {
    _width = width;
    _height = height;

    _display = std::unique_ptr<ALLEGRO_DISPLAY,
        decltype(&al_destroy_display)>(
            al_create_display(width, height), al_destroy_display);
    if (!_display) {
        throw std::runtime_error("Failed to create display");
    }

    al_set_window_title(_display.get(), "Allegro5 - Arcade");
}

ALLEGRO_DISPLAY* Allegro5Window::getDisplay() const {
    return _display.get();
}

int Allegro5Window::getWidth() const {
    return _width;
}

int Allegro5Window::getHeight() const {
    return _height;
}

}  // namespace Allegro
