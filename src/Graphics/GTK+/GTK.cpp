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
#include "GTK+/GTK.hpp"
#include "Models/ColorType.hpp"
#include "Models/ModuleInfos.hpp"

static gboolean on_window_close_request(GtkWindow* window, gpointer user_data) {
    GTKModule* module = static_cast<GTKModule*>(user_data);
    if (module)
        module->_running = false;
    return TRUE;
}

GTKModule::~GTKModule() {
    stop();
}

void GTKModule::threadMain() {
    putenv(const_cast<char*>("GTK_MODULES="));
    putenv(const_cast<char*>("GTK_PATH="));
    putenv(const_cast<char*>("GTK_IM_MODULE="));
    putenv(const_cast<char*>("GTK_THEME=Adwaita"));

    std::string tempDir = "/tmp/gtk-arcade-" + std::to_string(getpid());
    std::string xdgVar = "XDG_DATA_HOME=" + tempDir;
    putenv(const_cast<char*>(xdgVar.c_str()));

    putenv(const_cast<char*>("GDK_BACKEND=x11"));
    putenv(const_cast<char*>("GSK_RENDERER=cairo"));

    GMainContext *context = g_main_context_new();
    g_main_context_push_thread_default(context);

    if (!gtk_is_initialized())
        gtk_init();

    _fontManager.loadFonts();

    _app = std::shared_ptr<GtkApplication>(
        gtk_application_new("com.epitech.arcade", G_APPLICATION_NON_UNIQUE),
        g_object_unref);

    if (!_app) {
        std::cerr << "Failed to create GTK application" << std::endl;
        _threadRunning = false;
        _initialized = false;
        return;
    }

    g_signal_connect(_app.get(), "activate", G_CALLBACK(on_activate), this);
    g_application_register(G_APPLICATION(_app.get()), nullptr, nullptr);

    _window.createWindow(_app, _windowWidth, _windowHeight);
    _window.setupDrawingArea(_windowWidth, _windowHeight, on_draw, this);

    _eventManager = GTK::GTKEvent(_window.getWindow().get());

    auto window = _window.getWindow();
    if (window) {
        gulong closeId = g_signal_connect(window.get(), "close-request",
            G_CALLBACK(on_window_close_request), this);
        _eventManager.addSignalHandler(closeId);
    }

    _window.showWindow();

    _renderer.createRenderer(_windowWidth, _windowHeight);

    _renderer.clearScreen();

    while (g_main_context_iteration(nullptr, FALSE)) {}

    _initialized = true;
    _running = true;

    while (_threadRunning) {
        while (g_main_context_iteration(nullptr, FALSE)) {}

        std::function<void()> command;
        {
            std::unique_lock<std::mutex> lock(_commandMutex);
            if (!_commandQueue.empty()) {
                command = _commandQueue.front();
                _commandQueue.pop();
            }
        }

        if (command)
            command();

        auto currentWindow = _window.getWindow();
        if (!currentWindow || !GTK_IS_WINDOW(currentWindow.get()))
            _running = false;
    }

    _eventManager.disconnectSignals();

    if (_window.getWindow()) {
        if (GTK_IS_WIDGET(_window.getWindow().get()))
            gtk_widget_set_visible(_window.getWindow().get(), FALSE);

        if (_app) {
            g_application_quit(G_APPLICATION(_app.get()));
            for (int i = 0; i < 10 && g_main_context_iteration(NULL,
                FALSE); i++) {}
        }
    }

    _window.resetDrawingArea();
    _textures.clear();
    _renderer = GTK::GTKRenderer();

    while (g_main_context_pending(NULL)) {
        g_main_context_iteration(NULL, FALSE);
    }

    _app.reset();
}

void GTKModule::executeCommand(std::function<void()> command) {
    std::unique_lock<std::mutex> lock(_commandMutex);
    _commandQueue.push(command);
    _commandCV.notify_one();
}

void GTKModule::executeCommandAndWait(std::function<void()> command) {
    std::mutex waitMutex;
    std::condition_variable waitCV;
    bool done = false;

    executeCommand([&]() {
        command();
        {
            std::lock_guard<std::mutex> lock(waitMutex);
            done = true;
        }
        waitCV.notify_one();
    });

    std::unique_lock<std::mutex> lock(waitMutex);
    waitCV.wait(lock, [&]() { return done; });
}

void GTKModule::init(float x, float y) {
    if (_initialized)
        return;

    int width = static_cast<int>(x);
    int height = static_cast<int>(y);
    _windowWidth = width;
    _windowHeight = height;

    _threadRunning = true;
    _gtkThread = std::thread(&GTKModule::threadMain, this);

    while (!_initialized && _threadRunning) {}
}

void GTKModule::stop() {
    if (!_running && !_threadRunning)
        return;

    _running = false;
    _threadRunning = false;

    if (_gtkThread.joinable())
        _gtkThread.join();

    _initialized = false;
}

void GTKModule::clearScreen() {
    if (!_running) return;

    executeCommand([this]() {
        _renderer.clearScreen();
    });
}

void GTKModule::refreshScreen() {
    if (!_running) return;

    executeCommand([this]() {
        _renderer.present();

        auto drawingArea = _window.getDrawingArea();
        if (drawingArea && GTK_IS_DRAWING_AREA(drawingArea.get())) {
            gtk_widget_queue_draw(drawingArea.get());
        }
    });
}

void GTKModule::drawEntity(int x, int y, char symbol) {
    // Implementation for drawEntity (if needed)
}

void GTKModule::drawTexture(int x, int y, const std::string &texturePath) {
    if (!_running) return;

    executeCommand([this, x, y, texturePath]() {
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
    });
}

void GTKModule::drawText(const std::string &text, int x,
    int y, Arcade::Color color) {
    if (!_running) return;

    executeCommand([this, text, x, y, color]() {
        auto renderer = _renderer.getRenderer();
        if (renderer) {
            auto font = _fontManager.getFont("default");
            _textManager.drawText(renderer, x, y, text, font, color);
        }
    });
}

void GTKModule::pollEvents() {
    if (!_running)
        return;

    executeCommandAndWait([this]() {
        _eventManager.handleEvents();
    });
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
    void fini_gtk(void) {}

    Arcade::IArcadeModule* entryPoint(void) {
        return new GTKModule();
    }

    Arcade::ModuleInfos module_infos() {
        return {"GTK+", "IDK",
            "IDK",
            "./lib/arcade_gtk+.so", Arcade::ModuleType::GRAPHIC_LIB};
    }
}
