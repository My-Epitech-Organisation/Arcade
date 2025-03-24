// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GTK+
*/
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include "GTK+/GTK.hpp"

GTK::GTK() : _name("GTK+") {
    gtk_init();
    loadFonts();
}

GTK::~GTK() {
    stop();
}

void GTK::init(float x, float y) {
    int width = static_cast<int>(x);
    int height = static_cast<int>(y);

    _windowWidth = width;
    _windowHeight = height;

    _app = std::shared_ptr<GtkApplication>(
        gtk_application_new("com.epitech.arcade", G_APPLICATION_NON_UNIQUE),
        g_object_unref);

    g_signal_connect(_app.get(), "activate", G_CALLBACK(on_activate), this);

    g_application_register(G_APPLICATION(_app.get()), nullptr, nullptr);

    createWindow(width, height);

    _surface = std::shared_ptr<cairo_surface_t>(
        cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
            _windowWidth, _windowHeight),
        cairo_surface_destroy);

    _cr = std::shared_ptr<cairo_t>(
        cairo_create(_surface.get()),
        cairo_destroy);

    if (_fonts.empty())
        loadFonts();

    clearScreen();

    while (g_main_context_iteration(nullptr, FALSE)) {}

    _running = true;
}

void GTK::stop() {
    _running = false;

    _textures.clear();
    _fonts.clear();
    _cr.reset();
    _surface.reset();

    if (_app) {
        g_application_quit(G_APPLICATION(_app.get()));

        while (g_main_context_iteration(nullptr, FALSE)) {}

        _window.reset();
        _app.reset();
    }
}

void GTK::clearScreen() {
    if (_cr) {
        cairo_set_source_rgb(_cr.get(), 0, 0, 0);
        cairo_paint(_cr.get());
    }
}

void GTK::refreshScreen() {
    if (_drawingArea) {
        gtk_widget_queue_draw(_drawingArea.get());

        while (g_main_context_iteration(nullptr, FALSE)) {}
    }
}

void GTK::drawEntity(int x, int y, char symbol) {
}

void GTK::drawTexture(int x, int y, const std::string &texturePath) {
    if (_cr) {
        std::shared_ptr<cairo_surface_t> texture = nullptr;
        int width = 0;
        int height = 0;

        auto it = _textures.find(texturePath);
        if (it == _textures.end()) {
            texture = loadTexture(texturePath);
            if (!texture)
                return;

            width = cairo_image_surface_get_width(texture.get());
            height = cairo_image_surface_get_height(texture.get());
            _textures[texturePath] = {texture, width, height};
        } else {
            texture = it->second.surface;
            width = it->second.width;
            height = it->second.height;
        }

        cairo_save(_cr.get());

        cairo_set_source_surface(_cr.get(), texture.get(), x, y);

        cairo_rectangle(_cr.get(), x, y, width, height);

        cairo_fill(_cr.get());

        cairo_restore(_cr.get());
    }
}

void GTK::drawText(int x, int y, const std::string &text) {
    if (_cr) {
        std::shared_ptr<PangoFontDescription> font = nullptr;
        auto it = _fonts.find("default");
        if (it != _fonts.end()) {
            font = it->second;
        } else {
            font = std::shared_ptr<PangoFontDescription>(
                pango_font_description_from_string("Sans 12"),
                pango_font_description_free);
            if (font)
                _fonts["default"] = font;
        }
        drawTextInternal(_cr, x, y, text, font);
    }
}

void GTK::pollEvents() {
    while (g_main_context_iteration(nullptr, FALSE)) {}

    if (!_window || !GTK_IS_WINDOW(_window.get()))
        _running = false;
}

bool GTK::isOpen() const {
    return _running;
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
        return new GTK();
    }
}
