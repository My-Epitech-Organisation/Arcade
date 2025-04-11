// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Qt5Utils
*/
#include <QFontDatabase>
#include <string>
#include <memory>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "Qt5/Qt5.hpp"
#include "Qt5/Qt5Keys.hpp"

void Qt5Module::createWindow(int width, int height) {
    _window = std::make_unique<QMainWindow>();
    _window->setWindowTitle("Arcade - Qt5");
    _window->resize(width, height);

    _centralWidget = std::make_unique<ArcadeWidget>();
    _window->setCentralWidget(_centralWidget.get());

    _buffer = std::make_unique<QPixmap>(width, height);
    _buffer->fill(Qt::black);

    _centralWidget->setBuffer(_buffer.get());

    _painter = std::make_unique<QPainter>();

    _eventFilter = std::make_unique<ArcadeEventFilter>(
        std::ref(_keyStates),
        std::ref(_mouseButtonStates),
        std::ref(_mousePosition));

    _window->installEventFilter(_eventFilter.get());
    _centralWidget->installEventFilter(_eventFilter.get());

    _centralWidget->setFocusPolicy(Qt::StrongFocus);
    _centralWidget->setFocus();

    _window->show();

    QApplication::processEvents();
    _centralWidget->activateWindow();
    _centralWidget->setFocus();
}

QFont* Qt5Module::loadFont(const std::string& fontPath) {
    std::lock_guard<std::mutex> lock(_resourceMutex);

    if (_fonts.find(fontPath) == _fonts.end()) {
        _fonts[fontPath] = std::make_unique<QFont>();
        int fontId = QFontDatabase::addApplicationFont(
            QString::fromStdString(fontPath));
        if (fontId != -1) {
            QString family = QFontDatabase::applicationFontFamilies(
                fontId).at(0);
            _fonts[fontPath] = std::make_unique<QFont>(family, 14);
        } else {
            std::cerr << "Failed to load font: " << fontPath << std::endl;
            _fonts[fontPath] = std::make_unique<QFont>("Arial", 14);
        }
    }
    return _fonts[fontPath].get();
}

QPixmap* Qt5Module::loadTexture(const std::string& texturePath) {
    std::lock_guard<std::mutex> lock(_resourceMutex);

    if (_textures.find(texturePath) == _textures.end()) {
        _textures[texturePath] = std::make_unique<QPixmap>();
        if (!_textures[texturePath]->load(
            QString::fromStdString(texturePath))) {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
            return nullptr;
        }
    }
    return _textures[texturePath].get();
}

const std::string& Qt5Module::getName() const {
    return _name;
}

int Qt5Module::getWidth() const {
    return _windowWidth;
}

int Qt5Module::getHeight() const {
    return _windowHeight;
}

bool Qt5Module::isKeyPressed(int keyCode) {
    auto arcadeKey = static_cast<Arcade::Keys>(keyCode);

    int qtKey = Arcade::Qt5KeyMap::getQtKey(arcadeKey);

    std::lock_guard<std::mutex> lock(_resourceMutex);
    auto it = _keyStates.find(qtKey);
    return (it != _keyStates.end()) && it->second;
}

bool Qt5Module::isMouseButtonPressed(int button) const {
    auto arcadeButton = static_cast<Arcade::MouseButton>(button);
    int qtButton = Arcade::Qt5KeyMap::getQtButton(arcadeButton);

    std::lock_guard<std::mutex> lock(_resourceMutex);
    return _mouseButtonStates.find(qtButton) != _mouseButtonStates.end() &&
           _mouseButtonStates.at(qtButton);
}

std::pair<size_t, size_t> Qt5Module::getMousePosition() const {
    std::lock_guard<std::mutex> lock(_resourceMutex);
    return _mousePosition;
}
