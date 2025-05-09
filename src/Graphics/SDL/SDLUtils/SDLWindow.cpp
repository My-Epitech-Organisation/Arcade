// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLWindow
*/
#include "SDLUtils/SDLWindow.hpp"
#include <string>
#include <memory>

void SDL::SDLWindow::createWindow(int width, int height) {
    _window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(
        SDL_CreateWindow(
            "Arcade",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_RESIZABLE),
        SDL_DestroyWindow);
    SDL_ShowWindow(_window.get());
    if (!_window) {
        SDL_Quit();
        throw std::runtime_error("Window could not be created! SDL Error: "
            + std::string(SDL_GetError()));
    }
    _windowWidth = width;
    _windowHeight = height;
}

std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>
    SDL::SDLWindow::getWindow() {
    auto windowPtr = _window.get();
    return std::unique_ptr<SDL_Window,
        decltype(&SDL_DestroyWindow)>(windowPtr, [](SDL_Window*){});
}

int SDL::SDLWindow::getHeight() const {
    return _windowHeight;
}

void SDL::SDLWindow::destroyWindow() {
    SDL_DestroyWindow(_window.get());
    _window.reset();
}

int SDL::SDLWindow::getWidth() const {
    return _windowWidth;
}
