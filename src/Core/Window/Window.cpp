// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Window
*/

#include <utility>
#include <memory>
#include <string>
#include <iostream>
#include "Window/Window.hpp"
#include "EventManager/KeyEvent/RawInputState.hpp"

namespace Arcade {

Window::Window(std::shared_ptr<IDisplayModule> displayModule,
std::shared_ptr<AEventManager> eventManager)
: _displayModule(std::move(displayModule)),
_eventManager(eventManager),
_width(800),
_height(600),
_title("Arcade"),
_isShuttingDown(false) {
    if (_displayModule)
        _displayModule->init(_width, _height);
}

Window::~Window() {
    _isShuttingDown = true;
    if (_displayModule) {
        try {
            _displayModule->stop();
        } catch (const std::exception& e) {
            std::cerr << "Exception during display module cleanup: "
                << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception during display module cleanup"
                << std::endl;
        }
        _displayModule.reset();
    }
}

void Window::createWindow(int width, int height, const std::string &title) {
    if (_isShuttingDown) return;
    _width = width;
    _height = height;
    _title = title;
    if (_displayModule)
        _displayModule->init(width, height);
}

void Window::resizeWindow(int width, int height) {
    if (_isShuttingDown) return;
    _width = width;
    _height = height;
    if (_displayModule) {
        _displayModule->stop();
        _displayModule->init(width, height);
    }
}

void Window::closeWindow() {
    _isShuttingDown = true;
    if (_displayModule)
        _displayModule->stop();
}

bool Window::isWindowOpen() const {
    return !_isShuttingDown && _displayModule && _displayModule->isOpen();
}

void Window::clearScreen() {
    if (_isShuttingDown || !_displayModule) return;
    _displayModule->clearScreen();
}

void Window::refreshScreen() {
    if (_isShuttingDown || !_displayModule) return;
    _displayModule->refreshScreen();
}

void Window::drawText(const std::string &text, int x, int y, Color color) {
    if (_isShuttingDown || !_displayModule) return;
    _displayModule->drawText(text, x, y, color);
}

bool Window::isKeyPressed(int keyCode) {
    if (_isShuttingDown || !_eventManager) return false;
    return _eventManager->isKeyPressed(static_cast<Keys>(keyCode));
}

std::shared_ptr<IEventManager> Window::getEventManager() const {
    return _eventManager;
}

int Window::getWidth() const {
    return _width;
}

int Window::getHeight() const {
    return _height;
}

IDisplayModule* Window::getDisplayModule() {
    if (_isShuttingDown) return nullptr;
    return _displayModule.get();
}
void Window::setDisplayModule(std::shared_ptr<IDisplayModule> displayModule) {
    _isShuttingDown = false;
    cleanupCurrentDisplayModule();
    if (!displayModule) {
        std::cerr << "Error: Trying to set null display module!" << std::endl;
        return;
    }
    initializeNewDisplayModule(std::move(displayModule));
}

void Window::cleanupCurrentDisplayModule() {
    if (_displayModule) {
        try {
            _displayModule->stop();
        } catch (const std::exception& e) {
            std::cerr << "Exception during stop: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception during stop" << std::endl;
        }
        _displayModule.reset();
    }
}

void Window::initializeNewDisplayModule(
std::shared_ptr<IDisplayModule> displayModule) {
    _displayModule = std::move(displayModule);
    try {
        _displayModule->init(_width, _height);
    } catch (const std::exception& e) {
        std::cerr << "Exception during display module initialization: "
                  << e.what() << std::endl;
        _displayModule.reset();
        _isShuttingDown = true;
    } catch (...) {
        std::cerr << "Unknown exception during display module initialization"
                  << std::endl;
        _displayModule.reset();
        _isShuttingDown = true;
    }
}

void Window::pollEvents() {
    if (_isShuttingDown || !_displayModule) return;

    try {
        _displayModule->pollEvents();
        if (_isShuttingDown) return;
        processInputState();
    } catch (const std::exception &e) {
        std::cerr << "Exception during event polling: " << e.what()
                  << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception during event polling" << std::endl;
    }
}

void Window::processInputState() {
    auto state = std::make_shared<RawInputState>();

    updateKeyStates(state);
    updateMouseStates(state);
    updateMousePosition(state);
    if (_eventManager) {
        _eventManager->updateInputState(*state);
    }
}

void Window::updateKeyStates(std::shared_ptr<RawInputState> state) {
    for (int i = 0; i <= static_cast<int>(Arcade::Keys::Z); ++i) {
        state->keyStates[static_cast<Arcade::Keys>(i)]
            = _displayModule->isKeyPressed(i);
    }
}

void Window::updateMouseStates(std::shared_ptr<RawInputState> state) {
    for (int i = 0; i <= 2; ++i) {
        state->mouseButtons[static_cast<Arcade::MouseButton>(i)] =
            _displayModule->isMouseButtonPressed(i);
    }
}

void Window::updateMousePosition(std::shared_ptr<RawInputState> state) {
    auto [mouseX, mouseY] = _displayModule->getMousePosition();
    state->mouseX = mouseX;
    state->mouseY = mouseY;
}

}  // namespace Arcade
