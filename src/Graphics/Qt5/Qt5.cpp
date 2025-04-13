// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Qt5
*/
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <algorithm>
#include <iostream>
#include "Qt5/Qt5.hpp"
#include "Qt5/Qt5Color.hpp"
#include "Interface/IArcadeModule.hpp"
#include "Models/ModuleInfos.hpp"

Qt5Module::~Qt5Module() {
    stop();
}

void Qt5Module::threadMain() {
    putenv(const_cast<char*>("QT_X11_NO_MITSHM=1"));
    putenv(const_cast<char*>("QT_FATAL_WARNINGS=0"));
    putenv(const_cast<char*>("QT_QPA_PLATFORM=xcb"));
    putenv(const_cast<char*>("QT_QPA_PLATFORMTHEME="));
    putenv(const_cast<char*>("QT_STYLE_OVERRIDE=fusion"));

    std::string tempDir = "/tmp/qt-arcade-" + std::to_string(getpid());
    std::string homeVar = "HOME=" + tempDir;
    putenv(const_cast<char*>(homeVar.c_str()));

    static int argc = 1;
    static char* argv[] = {const_cast<char*>("Arcade"), nullptr};

    _app = std::make_unique<QApplication>(argc, argv);

    if (!_app) {
        std::cerr << "Failed to create Qt application" << std::endl;
        _threadRunning = false;
        _initialized = false;
        return;
    }

    createWindow(_windowWidth, _windowHeight);

    if (_app)
        _app->processEvents();

    _initialized = true;
    _running = true;

    while (_threadRunning) {
        if (_app)
            _app->processEvents();

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

        if (!_window || !_window->isVisible())
            _running = false;
    }

    if (_painter && _painter->isActive())
        _painter->end();
    _painter.reset();

    {
        std::lock_guard<std::mutex> lock(_resourceMutex);
        _textures.clear();
        _fonts.clear();
    }

    if (_centralWidget)
        _centralWidget.release();

    if (_window) {
        _window->setVisible(false);
        _window->close();
        _window.reset();
    }

    _buffer.reset();
    _eventFilter.reset();

    if (_app) {
        _app->processEvents();
        _app.reset();
    }
}

void Qt5Module::executeCommand(std::function<void()> command) {
    std::unique_lock<std::mutex> lock(_commandMutex);
    _commandQueue.push(command);
    _commandCV.notify_one();
}

void Qt5Module::executeCommandAndWait(std::function<void()> command) {
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

void Qt5Module::init(float x, float y) {
    if (_initialized)
        return;

    _windowWidth = static_cast<int>(x);
    _windowHeight = static_cast<int>(y);

    _threadRunning = true;
    _qtThread = std::thread(&Qt5Module::threadMain, this);

    while (!_initialized && _threadRunning) {}
}

void Qt5Module::stop() {
    if (!_running && !_threadRunning)
        return;

    _running = false;
    _threadRunning = false;

    if (_qtThread.joinable())
        _qtThread.join();

    _initialized = false;
}

void Qt5Module::clearScreen() {
    if (!_running) return;

    executeCommand([this]() {
        if (_buffer) {
            _buffer->fill(Qt::black);
            if (!_painter)
                _painter = std::make_unique<QPainter>();

            if (_painter->isActive())
                _painter->end();

            if (!_painter->begin(_buffer.get()))
                std::cerr << "Failed to begin painting on buffer" << std::endl;
        }
    });
}

void Qt5Module::drawDrawable
(std::shared_ptr<Arcade::IDrawableComponent> drawable) {
    if (!drawable->isRenderable())
        return;

    if (drawable->shouldRenderAsText()) {
        drawText(drawable->getText(),
            static_cast<int>(drawable->getPositionX()),
            static_cast<int>(drawable->getPositionY()), drawable->getColor());
    } else if (drawable->shouldRenderAsTexture()) {
        drawTexture(static_cast<int>(drawable->getPositionX()),
            static_cast<int>(drawable->getPositionY()), drawable->getPath());
    } else if (drawable->shouldRenderAsCharacter()) {
        drawEntity(static_cast<int>(drawable->getPositionX()),
            static_cast<int>(drawable->getPositionY()),
            drawable->getCharacter());
    }
}

void Qt5Module::refreshScreen() {
    if (!_running) return;

    executeCommand([this]() {
        if (_painter && _painter->isActive())
            _painter->end();

        if (_centralWidget)
            _centralWidget->update();
    });
}

void Qt5Module::drawDrawable(const Arcade::DrawableComponent &drawable) {
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

void Qt5Module::drawEntity(int x, int y, char symbol) {
    // Optional implementation
}

void Qt5Module::drawTexture(int x, int y, const std::string& texturePath) {
    if (!_running) return;

    executeCommand([this, x, y, texturePath]() {
        auto texture = loadTexture(texturePath);
        if (!texture || !_painter || !_painter->isActive())
            return;

        _painter->drawPixmap(x, y, *texture);
    });
}

void Qt5Module::drawText(const std::string& text, int x, int y,
    Arcade::Color color) {
    if (!_running) return;

    executeCommand([this, text, x, y, color]() {
        auto font = loadFont("assets/fonts/arial.ttf");
        if (!font || !_painter || !_painter->isActive())
            return;

        _painter->setFont(*font);
        _painter->setPen(Qt5Color::convertColor(color));
        _painter->drawText(x, y, QString::fromStdString(text));
    });
}

void Qt5Module::pollEvents() {
    if (!_running) return;

    executeCommandAndWait([this]() {
        if (_app)
            _app->processEvents();

        if (_centralWidget && !_centralWidget->hasFocus())
            _centralWidget->setFocus();

        if (_window && _centralWidget) {
            QPoint globalPos = QCursor::pos();
            QPoint localPos = _centralWidget->mapFromGlobal(globalPos);

            std::lock_guard<std::mutex> lock(_resourceMutex);
            _mousePosition = {
                static_cast<size_t>(std::max(0, localPos.x())),
                static_cast<size_t>(std::max(0, localPos.y()))
            };
        }
    });
}

bool Qt5Module::isOpen() const {
    return _running && _window && _window->isVisible();
}

extern "C" __attribute__((constructor)) const char* init_qt5(void) {
    return "Lib";
}

extern "C" __attribute__((destructor)) void fini_qt5(void) {}

extern "C" Arcade::IArcadeModule* entryPoint(void) {
    return new Qt5Module();
}

extern "C" Arcade::ModuleInfos module_infos() {
    return {"Qt5", "Qt5 Graphics Library",
        "IDK",
        "./lib/arcade_qt5.so", Arcade::ModuleType::GRAPHIC_LIB};
}
