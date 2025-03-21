// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDL
*/
#include "SDL/SDL.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>

SDL::~SDL() {
    _renderer.reset();
    _window.reset();
    SDL_Quit();
}

void SDL::init(float x = 800.f, float y = 600.f) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL could not initialize! SDL Error: "
            + std::string(SDL_GetError()));

    _window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(
        SDL_CreateWindow(
            "Arcade",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            static_cast<int>(x),
            static_cast<int>(y),
            SDL_WINDOW_SHOWN),
        SDL_DestroyWindow);
    if (!_window) {
        SDL_Quit();
        throw std::runtime_error("Window could not be created! SDL Error: "
            + std::string(SDL_GetError()));
    }

    _renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
        SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer);
    if (!_renderer) {
        SDL_Quit();
        throw std::runtime_error("Renderer could not be created! SDL Error: "
            + std::string(SDL_GetError()));
    }

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

void SDL::drawEntity(int x, int y, char symbol) {
    return;
}

void SDL::drawTexture(int x, int y, const std::string &texturePath) {
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> tempSurface(
        IMG_Load(texturePath.c_str()), SDL_FreeSurface);
    if (!tempSurface) {
        throw std::runtime_error("Unable to load image " + texturePath +
            "! SDL_image Error: " +std::string(IMG_GetError()));
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(_renderer.get(),
        tempSurface.get());
    if (!newTexture) {
        throw std::runtime_error("Unable to create texture from" + texturePath +
            "! SDL Error: " + std::string(SDL_GetError()));
    }

    int w, h;
    SDL_QueryTexture(newTexture, nullptr, nullptr, &w, &h);

    auto texturePtr = std::unique_ptr<SDL_Texture,
        decltype(&SDL_DestroyTexture)>( newTexture,
        SDL_DestroyTexture);

    SDL_Rect destRect;
    destRect.x = (_windowWidth - w) / 2;
    destRect.y = (_windowHeight - h) / 2;
    destRect.w = w;
    destRect.h = h;

    SDL_RenderCopy(_renderer.get(), texturePtr.get(), nullptr, &destRect);
}

void SDL::drawText(int x, int y, const std::string &text) {
    auto _font = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>(
        TTF_OpenFont("assets/fonts/arial.ttf", 24),
        TTF_CloseFont);

    if (!_font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = {255, 255, 255, 255};

    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> textSurface(
        TTF_RenderText_Solid(_font.get(), text.c_str(), textColor),
        SDL_FreeSurface);
    if (!textSurface) {
        std::cerr << "Unable to render text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer.get(), textSurface.get());
    if (!textTexture) {
        std::cerr << "Unable to create texture from rendered text: " << SDL_GetError() << std::endl;
        return;
    }

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    SDL_Rect renderRect = {x, y, textWidth, textHeight};
    SDL_RenderCopy(_renderer.get(), textTexture, nullptr, &renderRect);
    SDL_DestroyTexture(textTexture);
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

