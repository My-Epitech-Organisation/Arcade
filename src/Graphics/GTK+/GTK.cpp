// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GTKModule+
*/
#include <GL/gl.h>
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include <future>
#include "GTK+/GTK.hpp"
#include "Models/ColorType.hpp"
#include "Models/ModuleInfos.hpp"

static gboolean on_window_close_request(GtkWindow* window, gpointer user_data) {
    GTKModule* module = static_cast<GTKModule*>(user_data);
    if (module) {
        module->_running = false;
        module->_threadRunning = false;
    }
    return TRUE;
}

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

    gdk_set_allowed_backends("x11,*");
    putenv(const_cast<char*>("GSK_RENDERER=cairo"));

    _threadRunning = true;
    _initialized = false;
    _gtkThread = std::thread(&GTKModule::gtkThreadFunction, this);

    waitForInitialization();
    _running = true;
}

void GTKModule::gtkThreadFunction() {
    if (!gtk_is_initialized())
        gtk_init();

    _fontManager.loadFonts();

    _app = std::shared_ptr<GtkApplication>(
        gtk_application_new("com.epitech.arcade", G_APPLICATION_NON_UNIQUE),
        g_object_unref);

    if (!_app) {
        _initialized = true;
        _threadRunning = false;
        signalInitialized();
        return;
    }

    g_signal_connect(_app.get(), "activate", G_CALLBACK(on_activate), this);
    g_application_register(G_APPLICATION(_app.get()), nullptr, nullptr);

    _window.createWindow(_app, _windowWidth, _windowHeight);
    _windowValid.store(true);

    _window.setupDrawingArea(_windowWidth, _windowHeight, on_draw, this);
    _drawingAreaValid.store(true);

    _eventManager = GTK::GTKEvent(_window.getWindow().get());

    auto window = _window.getWindow();
    if (window) {
        gulong closeId = g_signal_connect(window.get(), "close-request",
            G_CALLBACK(on_window_close_request), this);
        _eventManager.addSignalHandler(closeId);
    }

    _window.showWindow();

    _renderer.createRenderer(_windowWidth, _windowHeight);
    clearScreen();

    _initialized = true;
    signalInitialized();

    while (_threadRunning) {
        g_main_context_iteration(nullptr, FALSE);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (_window.getWindow()) {
        _windowValid.store(false);
        _drawingAreaValid.store(false);

        _eventManager.disconnectSignals();

        if (GTK_IS_WIDGET(_window.getWindow().get()))
            gtk_widget_set_visible(_window.getWindow().get(), FALSE);

        if (_app) {
            g_application_quit(G_APPLICATION(_app.get()));
            for (int i = 0; i < 10 &&
                g_main_context_iteration(NULL, FALSE); i++) {}
        }
    }

    _window.resetDrawingArea();
    _textures.clear();
    _renderer = GTK::GTKRenderer();

    while (g_main_context_pending(NULL)) {
        g_main_context_iteration(NULL, FALSE);
    }
}

void GTKModule::stop() {
    _running = false;
    _windowValid.store(false);
    _drawingAreaValid.store(false);
    _threadRunning = false;

    if (_gtkThread.joinable()) {
        try {
            auto joinFuture = std::async(std::launch::async, [this]() {
                if (_gtkThread.joinable()) {
                    _gtkThread.join();
                }
            });

            auto status = joinFuture.wait_for(std::chrono::seconds(1));
            if (status != std::future_status::ready) {
                std::cerr <<
                    "GTK thread join timed out, detaching to prevent crash"
                << std::endl;
                if (_gtkThread.joinable())
                    _gtkThread.detach();
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception joining GTK thread: "
                << e.what() << std::endl;
            if (_gtkThread.joinable())
                _gtkThread.detach();
        }
    }

    _app.reset();

    while (g_main_context_iteration(NULL, FALSE)) {}
}

void GTKModule::waitForInitialization() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [this]() { return _initialized.load(); });
}

void GTKModule::signalInitialized() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.notify_all();
}

void GTKModule::clearScreen() {
    if (!_running) return;

    std::unique_lock<std::mutex> lock(_mutex);
    _renderer.clearScreen();
}

void GTKModule::refreshScreen() {
    if (!_running || !_drawingAreaValid.load()) return;

    std::unique_lock<std::mutex> lock(_mutex);
    _renderer.present();

    auto drawingArea = _window.getDrawingArea();
    if (drawingArea && _drawingAreaValid.load()
        && GTK_IS_DRAWING_AREA(drawingArea.get())) {
        gtk_widget_queue_draw(drawingArea.get());
    }
}

void GTKModule::drawEntity(int x, int y, char symbol) {
}

void GTKModule::drawTexture(int x, int y, const std::string &texturePath) {
    if (!_running) return;

    std::unique_lock<std::mutex> lock(_mutex);
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
    if (!_running) return;

    std::unique_lock<std::mutex> lock(_mutex);
    auto renderer = _renderer.getRenderer();
    if (renderer) {
        auto font = _fontManager.getFont("default");
        _textManager.drawText(renderer, x, y, text, font, color);
    }
}

void GTKModule::pollEvents() {
    if (!_running || !_windowValid.load() || !_threadRunning) {
        _running = false;
        return;
    }

    _eventManager.handleEvents();

    auto window = _window.getWindow();
    if (!window || !_windowValid.load() || !GTK_IS_WINDOW(window.get())) {
        _running = false;
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

    Arcade::ModuleInfos module_infos() {
        return {"GTK+", "IDK",
            "IDK",
            "./lib/arcade_gtk+.so", Arcade::ModuleType::GRAPHIC_LIB};
    }
}
