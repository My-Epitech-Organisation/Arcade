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
#include <iostream>

SDL::~SDL() {
    _renderer.reset();
    _window.reset();
    SDL_Quit();
}

void SDL::createWindow(int width, int height) {
    _window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(
        SDL_CreateWindow(
            "Arcade",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_SHOWN),
        SDL_DestroyWindow);
    if (!_window) {
        SDL_Quit();
        throw std::runtime_error("Window could not be created! SDL Error: "
            + std::string(SDL_GetError()));
    }
}

void SDL::createRenderer() {
    _renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
        SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer);
    if (!_renderer) {
        SDL_Quit();
        throw std::runtime_error("Renderer could not be created! SDL Error: "
            + std::string(SDL_GetError()));
    }
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

std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
    SDL::loadSurface(const std::string &texturePath) {
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> tempSurface(
        IMG_Load(texturePath.c_str()), SDL_FreeSurface);
    if (!tempSurface) {
        throw std::runtime_error("Unable to load image " + texturePath +
            "! SDL_image Error: " + std::string(IMG_GetError()));
    }
    return tempSurface;
}

std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> SDL::createTexture(
    SDL_Surface *surface, const std::string &texturePath) {
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texturePtr(
        SDL_CreateTextureFromSurface(_renderer.get(), surface),
        SDL_DestroyTexture);
    if (!texturePtr) {
        throw std::runtime_error("Unable to create texture from "
            + texturePath + "! SDL Error: " + std::string(SDL_GetError()));
    }
    return texturePtr;
}

void SDL::renderTexture(SDL_Texture *texture) {
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

    SDL_Rect destRect;
    destRect.x = (_windowWidth - w) / 2;
    destRect.y = (_windowHeight - h) / 2;
    destRect.w = w;
    destRect.h = h;

    SDL_RenderCopy(_renderer.get(), texture, nullptr, &destRect);
}

void SDL::drawTexture(int x, int y, const std::string &texturePath) {
    auto surface = loadSurface(texturePath);
    auto texture = createTexture(surface.get(), texturePath);
    renderTexture(texture.get());
}

std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>
    SDL::loadFont(int fontSize) {
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font(
        TTF_OpenFont("assets/fonts/arial.ttf", fontSize),
        TTF_CloseFont);

    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
    return font;
}

std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> SDL::createTextSurface(
    TTF_Font* font, const std::string& text, SDL_Color color) {

    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> textSurface(
        TTF_RenderText_Solid(font, text.c_str(), color),
        SDL_FreeSurface);

    if (!textSurface) {
        std::cerr << "Unable to render text surface: " <<
            TTF_GetError() << std::endl;
    }
    return textSurface;
}

std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>
    SDL::createTextTexture(
    SDL_Surface* surface) {

    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> textTexture(
        SDL_CreateTextureFromSurface(_renderer.get(), surface),
        SDL_DestroyTexture);

    if (!textTexture) {
        std::cerr << "Unable to create texture from rendered text: "
            << SDL_GetError() << std::endl;
    }
    return textTexture;
}

void SDL::renderTextTexture(SDL_Texture* texture, int x, int y,
    int width, int height) {
    SDL_Rect renderRect = {x, y, width, height};
    SDL_RenderCopy(_renderer.get(), texture, nullptr, &renderRect);
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

