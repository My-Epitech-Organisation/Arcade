// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDL
*/
#include "SDL/SDL.hpp"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include "SDL/SDLColor.hpp"
#include "Interface/IArcadeModule.hpp"

SDL::~SDL() {
    stop();
}

void SDL::init(float x, float y) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL could not initialize! SDL Error: "
            + std::string(SDL_GetError()));
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Quit();
        throw std::runtime_error
        ("SDL_image could not initialize! SDL_image Error: "
            + std::string(IMG_GetError()));
    }
    if (TTF_Init() == -1) {
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error
        ("SDL_ttf could not initialize! SDL_ttf Error: "
            + std::string(TTF_GetError()));
    }
    int width = static_cast<int>(x);
    int height = static_cast<int>(y);
    if (_renderer) {
        SDL_DestroyRenderer(_renderer.release());
    }
    if (_window) {
        SDL_DestroyWindow(_window.release());
    }
    createWindow(width, height);

    createRenderer();

    SDL_SetRenderDrawColor(_renderer.get(), 255, 0, 0, 255);
    _windowWidth = static_cast<int>(x);
    _windowHeight = static_cast<int>(y);
}

void SDL::stop() {
    if (_renderer) {
        _renderer.reset();
    }
    if (_window) {
        _window.reset();
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void SDL::clearScreen() {
    SDL_SetRenderDrawColor(_renderer.get(), 255, 0, 0, 255);
    if (SDL_RenderClear(_renderer.get()) != 0) {
        std::cerr << "Failed to clear screen: " << SDL_GetError() << std::endl;
    }
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

void SDL::drawText(const std::string &text, int x, int y, Arcade::Color color) {
    if (text.empty()) {
        return;
    }
    auto font = loadFont(24);
    if (!font) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }
    SDL_Color textColor = SDLColor::convertColor(color);
    auto textSurface = createTextSurface(font.get(), text, textColor);
    if (!textSurface) {
        std::cerr << "Failed to create text surface: "
            << TTF_GetError() << std::endl;
        return;
    }
    auto textTexture = createTextTexture(textSurface.get());
    if (!textTexture) {
        std::cerr << "Failed to create text texture: "
            << SDL_GetError() << std::endl;
        return;
    }
    SDL_SetTextureBlendMode(textTexture.get(), SDL_BLENDMODE_BLEND);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect destRect = {x, y, textWidth, textHeight};
    if (SDL_RenderCopy(_renderer.get(), textTexture.get(),
        NULL, &destRect) != 0) {
        std::cerr << "Failed to render text: " << SDL_GetError() << std::endl;
    }
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

extern "C" __attribute__((constructor))
    const char *init_sdl(void) {
        return "Lib";
}

extern "C" __attribute__((destructor)) void fini_sdl(void) { }

extern "C" Arcade::IArcadeModule* entryPoint(void) {
    return new SDL();
}
