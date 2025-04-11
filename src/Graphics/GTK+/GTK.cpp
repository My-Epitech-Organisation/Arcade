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
#include "Models/ColorType.hpp"

GTKModule::GTKModule() : _name("GTK+"), _eventManager(nullptr) {
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
    _eventManager = GTK::GTKEvent(_window.getWindow().get());
    _renderer.createRenderer(width, height);
    clearScreen();

    while (g_main_context_iteration(nullptr, FALSE)) {}

    _running = true;
}

void GTKModule::stop() {
    if (!_running) return;

    _running = false;
    _textures.clear();

    _eventManager.disconnectSignals();
    _eventManager = GTK::GTKEvent(nullptr);

    if (_window.getWindow()) {
        gtk_widget_set_visible(_window.getWindow().get(), FALSE);
        int timeout = 0;
        while (g_main_context_iteration(nullptr, TRUE) && timeout++ < 1000) {
            std::cout << timeout << std::endl;
        }
        if (GTK_IS_WINDOW(_window.getWindow().get())) {
            GtkWindow* window = GTK_WINDOW(_window.getWindow().get());
            gtk_window_destroy(window);
        }

        int timeout2 = 0;
        while (g_main_context_iteration(nullptr, TRUE) && timeout2++ < 3) {
            std::cout << timeout2 << std::endl;
        }

        _window.resetDrawingArea();
    }

    if (_app) {
        g_application_quit(G_APPLICATION(_app.get()));
        int timeout3 = 0;
        std::cout << "trying to quit app" << std::endl;
        while (g_main_context_iteration(nullptr, TRUE) && timeout3++ < 3) {
            std::cout << timeout3 << std::endl;
        }
        std::cout << "Quitted app" << std::endl;
        _app.reset();
    }

    _renderer = GTK::GTKRenderer();

    GdkDisplay* display = gdk_display_get_default();
    if (display) {
        gdk_display_flush(display);
        gdk_display_close(display);
        g_object_unref(display);
    }
}

void GTKModule::clearScreen() {
    _renderer.clearScreen();
}

void GTKModule::refreshScreen() {
    _renderer.present();
    gtk_widget_queue_draw(_window.getDrawingArea().get());
}

void GTKModule::drawDrawable(const Arcade::DrawableComponent &drawable) {
    if (!drawable.isVisible)
        return;

    if (drawable.shouldRenderAsText()) {
        drawText(drawable.text, static_cast<int>(drawable.posX),
            static_cast<int>(drawable.posY), drawable.color);
    } else if (drawable.shouldRenderAsTexture()) {
        drawTexture(static_cast<int>(drawable.posX),
            static_cast<int>(drawable.posY), drawable.path);
    } else if (drawable.shouldRenderAsCharacter()) {
        drawEntity(static_cast<int>(drawable.posX),
            static_cast<int>(drawable.posY), drawable.character);
    }
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

void GTKModule::drawText(const std::string &text, int x, int y,
    Arcade::Color color) {
    auto renderer = _renderer.getRenderer();
    if (renderer) {
        auto font = _fontManager.getFont("default");
        _textManager.drawText(renderer, x, y, text, font, color);
    }
}

void GTKModule::pollEvents() {
    _eventManager.handleEvents();

    if (!_window.getWindow() || !GTK_IS_WINDOW(_window.getWindow().get())) {
        _running = false;
        stop();
    }
}

bool GTKModule::isOpen() const {
    return _running;
}

const std::string& GTKModule::getName() const {
    return _name;
}

int GTKModule::getWidth() const {
    return _windowWidth;
}

int GTKModule::getHeight() const {
    return _windowHeight;
}

bool GTKModule::isKeyPressed(int keyCode) {
    return _eventManager.isKeyPressed(keyCode);
}

std::pair<size_t, size_t> GTKModule::getMousePosition() const {
    return _eventManager.getMousePosition();
}

bool GTKModule::isMouseButtonPressed(int button) const {
    return _eventManager.isMouseButtonPressed(button);
}

extern "C" {
    __attribute__((constructor))
    const char* init_gtk(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_gtk(void) {
    }

    Arcade::IArcadeModule* entryPoint(void) {
        return new GTKModule();
    }
}
