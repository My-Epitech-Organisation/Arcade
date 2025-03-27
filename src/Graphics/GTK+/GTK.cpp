// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GTKModule+
*/
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include "GTK+/GTK.hpp"

GTKModule::GTKModule() : _name("GTK+") {
    gtk_init();
    _fontManager.loadFonts();
}

GTKModule::~GTKModule() {
    stop();
}

void GTKModule::init(float x, float y) {
    int width = static_cast<int>(x);
    int height = static_cast<int>(y);

    _windowWidth = width;
    _windowHeight = height;

    _app = std::shared_ptr<GtkApplication>(
        gtk_application_new("com.epitech.arcade", G_APPLICATION_NON_UNIQUE),
        g_object_unref);

    g_signal_connect(_app.get(), "activate", G_CALLBACK(on_activate), this);

    g_application_register(G_APPLICATION(_app.get()), nullptr, nullptr);

    _window.createWindow(_app, width, height);
    _window.setupDrawingArea(width, height, on_draw, this);
    _window.showWindow();
    _renderer.createRenderer(width, height);
    clearScreen();

    while (g_main_context_iteration(nullptr, FALSE)) {}

    _running = true;
}

void GTKModule::stop() {
    _running = false;

    _textures.clear();

    if (_app) {
        g_application_quit(G_APPLICATION(_app.get()));

        while (g_main_context_iteration(nullptr, FALSE)) {}

        _app.reset();
    }  _app.reset();
}

void GTKModule::clearScreen() {
    _renderer.clearScreen();
}

void GTKModule::refreshScreen() {
    _renderer.present();
    gtk_widget_queue_draw(_window.getDrawingArea().get());
}

void GTKModule::drawEntity(int x, int y, char symbol) {
}

void GTKModule::drawTexture(int x, int y, const std::string &texturePath) {
    auto renderer = _renderer.getRenderer();
    if (renderer) {
        std::shared_ptr<cairo_surface_t> texture = nullptr;
        int width = 0;
        int height = 0;

        auto it = _textures.find(texturePath);
        if (it == _textures.end()) {
            texture = _textureManager.loadTexture(texturePath);
            if (!texture)
                return;

            width = cairo_image_surface_get_width(texture.get());
            height = cairo_image_surface_get_height(texture.get());
            _textures[texturePath] = {texture, width, height};
        } else {
            texture = it->second.surface;
        }

        _textureManager.renderTexture(renderer, texture, x, y);
    }
}

void GTKModule::drawText(int x, int y, const std::string &text) {
    auto renderer = _renderer.getRenderer();
    if (renderer) {
        auto font = _fontManager.getFont("default");
        _textManager.drawText(renderer, x, y, text, font);
    }
}

void GTKModule::pollEvents() {
    _eventManager.handleEvents();

    if (!_window.getWindow() || !GTK_IS_WINDOW(_window.getWindow().get()))
        _running = false;
}

bool GTKModule::isOpen() const {
    return _running;
}

const std::string& GTKModule::getName() const {
    return _name;
}

extern "C" {
    __attribute__((constructor))
    const char* init_gtk(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_gtk(void) {
    }

    Arcade::IDisplayModule* entryPoint(void) {
        return new GTKModule();
    }
}
