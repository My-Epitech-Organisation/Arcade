// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDL
*/
#include "SDL/SDL.hpp"
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>

SDL::~SDL() {
    _renderer.reset();
    _window.reset();
    SDL_Quit();
}

void SDL::init(float x, float y) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL could not initialize! SDL Error: "
            + std::string(SDL_GetError()));

    int width = static_cast<int>(x);
    int height = static_cast<int>(y);
    createWindow(width, height);

    createRenderer();

    SDL_SetRenderDrawColor(_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
    _windowWidth = static_cast<int>(x);
    _windowHeight = static_cast<int>(y);
}

void SDL::stop() {
    _renderer.reset();
    _window.reset();
    SDL_Quit();
}

void SDL::clearScreen() {
    SDL_RenderClear(_renderer.get());
}

void SDL::refreshScreen() {
    SDL_RenderPresent(_renderer.get());
}

void SDL::drawEntity(int x, int y, char symbol) {}


void SDL::drawTexture(int x, int y, const std::string &texturePath) {
    auto surface = loadSurface(texturePath);
    auto texture = createTexture(surface.get(), texturePath);
    renderTexture(texture.get());
}

void SDL::drawText(int x, int y, const std::string &text) {
    auto font = loadFont(24);
    if (!font) {
        return;
    }

    SDL_Color textColor = {255, 255, 255, 255};

    auto textSurface = createTextSurface(font.get(), text, textColor);
    if (!textSurface) {
        return;
    }

    auto textTexture = createTextTexture(textSurface.get());
    if (!textTexture) {
        return;
    }

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    renderTextTexture(textTexture.get(), x, y, textWidth, textHeight);
}

void SDL::pollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            _running = false;
    }
}

bool SDL::isOpen() const {
    return (_running);
}

extern "C" {
    __attribute__((constructor))
    const char *init_sdl(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_sdl(void) {
    }

    Arcade::IDisplayModule* entryPoint(void) {
        return new SDL();
    }
}

