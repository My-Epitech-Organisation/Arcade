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
    if (!al_init())
        throw std::runtime_error("Failed to initialize Allegro");

    if (!al_init_image_addon())
        throw std::runtime_error("Failed to initialize Allegro image addon");

    if (!al_init_font_addon())
        throw std::runtime_error("Failed to initialize Allegro font addon");

    if (!al_init_ttf_addon())
        throw std::runtime_error("Failed to initialize Allegro TTF addon");

    if (!al_install_keyboard())
        throw std::runtime_error("Failed to install keyboard");

    int width = static_cast<int>(x);
    int height = static_cast<int>(y);
    createWindow(width, height);
    _windowWidth = width;
    _windowHeight = height;

    _eventQueue.reset(al_create_event_queue());
    if (!_eventQueue)
        throw std::runtime_error("Failed to create event queue");

    al_register_event_source(_eventQueue.get(),
        al_get_display_event_source(_display.get()));
    al_register_event_source(_eventQueue.get(), al_get_keyboard_event_source());
}

void Allegro5::createWindow(int width, int height) {
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    _display.reset(al_create_display(width, height));
    if (!_display)
        throw std::runtime_error("Failed to create display");
    al_set_window_title(_display.get(), "Arcade");
}

void Allegro5::stop() {
    _fonts.clear();
    _bitmaps.clear();
    _eventQueue.reset();
    _display.reset();

    al_uninstall_keyboard();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_shutdown_image_addon();
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
    auto bitmap = loadBitmap(texturePath);
    if (!bitmap)
        return;

    al_draw_bitmap(bitmap.get(), x, y, 0);
}

void Allegro5::drawText(int x, int y, const std::string& text) {
    auto font = loadFont("assets/fonts/arial.ttf");
    if (!font)
        return;

    al_draw_text(font.get(), al_map_rgb(255, 255, 255), x, y, 0, text.c_str());
}

void Allegro5::pollEvents() {
    ALLEGRO_EVENT event;
    while (al_get_next_event(_eventQueue.get(), &event)) {
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            _running = false;
    }
}

bool Allegro5::isOpen() const {
    return _running && _display != nullptr;
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
