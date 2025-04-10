// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGL
*/

#include "OpenGL/OpenGL.hpp"
#include <iostream>
#include <string>
#include <utility>
#include "Models/ModuleInfos.hpp"

OpenGLModule::OpenGLModule()
    : _name("OpenGL"), _windowWidth(800), _windowHeight(600), _running(true) {}

OpenGLModule::~OpenGLModule() {
    stop();
}

void OpenGLModule::init(float width, float height) {
    try {
        _windowWidth = static_cast<int>(width);
        _windowHeight = static_cast<int>(height);
        _window.init(width, height, _name);
        _event.setupCallbacks(_window.getWindow());
        _text.init();
        _texture.init();
        _running = true;
    } catch (const std::exception &e) {
        std::cerr << "OpenGL initialization error: " << e.what() << std::endl;
        throw;
    }
}

void OpenGLModule::stop() {
    if (_window.isOpen())
        _window.close();
    _running = false;
}

void OpenGLModule::clearScreen() {
    _window.clearScreen();
}

void OpenGLModule::refreshScreen() {
    _window.refreshScreen();
}

void OpenGLModule::drawEntity(int x, int y, char symbol) {
    std::string text(1, symbol);
    _text.draw(text, x, y, Arcade::Color::WHITE);
}

void OpenGLModule::drawTexture(int x, int y, const std::string &textureId) {
    _texture.draw(x, y, textureId);
}

void OpenGLModule::drawText(const std::string &text, int x, int y,
    Arcade::Color color) {
    _text.draw(text, x, y, color);
}

void OpenGLModule::pollEvents() {
    _event.pollEvents(_window.getWindow());
    if (!_window.isOpen())
        _running = false;
}

bool OpenGLModule::isOpen() const {
    return _running;
}

const std::string& OpenGLModule::getName() const {
    return _name;
}

int OpenGLModule::getWidth() const {
    return _windowWidth;
}

int OpenGLModule::getHeight() const {
    return _windowHeight;
}

bool OpenGLModule::isKeyPressed(int keyCode) {
    return _event.isKeyPressed(keyCode);
}

bool OpenGLModule::isMouseButtonPressed(int button) const {
    return _event.isMouseButtonPressed(button);
}

std::pair<size_t, size_t> OpenGLModule::getMousePosition() const {
    return _event.getMousePosition();
}

extern "C" {
    Arcade::IDisplayModule* entryPoint(void) {
        return new OpenGLModule();
    }

    Arcade::ModuleInfos module_infos() {
        return {"OpenGL", "IDK",
            "IDK",
            "./lib/arcade_opengl.so", Arcade::ModuleType::GRAPHIC_LIB};
    }
}
