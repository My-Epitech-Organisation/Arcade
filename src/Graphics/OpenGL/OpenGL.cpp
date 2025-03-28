// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGL
*/
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include "OpenGL/OpenGL.hpp"

OpenGLModule::~OpenGLModule() {
    stop();
}

void OpenGLModule::init(float width, float height) {
    _windowWidth = static_cast<int>(width);
    _windowHeight = static_cast<int>(height);
    _window.createWindow(_windowWidth, _windowHeight);
    _renderer.init(_window.getWindow());
    _texture.init();
    _text.init();
    _running = true;
}

void OpenGLModule::stop() {
    _running = false;
    glfwTerminate();
}

void OpenGLModule::clearScreen() {
    _renderer.clearScreen();
}

void OpenGLModule::refreshScreen() {
    _renderer.refreshScreen(_window.getWindow());
}

void OpenGLModule::drawEntity(int x, int y, char symbol) {
    std::string entityStr(1, symbol);
    drawText(x, y, entityStr);
}

void OpenGLModule::drawTexture(int x, int y, const std::string& texturePath) {
    unsigned int textureID = _texture.loadTexture(texturePath);
    if (textureID != 0)
        _texture.renderTexture(x, y, textureID, _windowWidth, _windowHeight);
}

void OpenGLModule::drawText(int x, int y, const std::string& text) {
    _text.drawText(x, y, text, _windowWidth, _windowHeight);
}

void OpenGLModule::pollEvents() {
    glfwPollEvents();
    if (_window.shouldClose())
        _running = false;
}

bool OpenGLModule::isOpen() const {
    return _running;
}

const std::string& OpenGLModule::getName() const {
    return _name;
}

extern "C" {
    __attribute__((constructor))
    const char* init_opengl(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_opengl(void) {
    }

    Arcade::IDisplayModule* entryPoint(void) {
        return new OpenGLModule();
    }
}
