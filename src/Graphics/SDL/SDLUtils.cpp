// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLUtils
*/
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include "SDL/SDL.hpp"
#include "SDL/SDLKeys.hpp"
#include "Models/KeysType.hpp"


void SDL::createWindow(int width, int height) {
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

void SDL::renderTextTexture(SDL_Texture* texture, int x, int y, int width, int height) {
    if (!texture) {
        std::cerr << "Invalid texture for rendering" << std::endl;
        return;
    }
    SDL_Rect destRect = {x, y, width, height};
    if (SDL_RenderCopy(_renderer.get(), texture, NULL, &destRect) != 0) {
        std::cerr << "Failed to render texture: " << SDL_GetError() << std::endl;
    }
}

const std::string& SDL::getName() const {
    return _name;
}

int SDL::getHeight() const {
    return _windowHeight;
}

int SDL::getWidth() const {
    return _windowWidth;
}

bool SDL::isKeyPressed(int keyCode) {
    SDL_Keycode sdlKey = Arcade::SDLKeyMap::getSDLKey(
        static_cast<Arcade::Keys>(keyCode));
    SDL_Scancode scanCode = SDL_GetScancodeFromKey(sdlKey);
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    return state[scanCode] != 0;
}

bool SDL::isMouseButtonPressed(int button) const {
    auto arcadeButton = static_cast<Arcade::MouseButton>(button);
    Uint8 sdlButton = Arcade::SDLKeyMap::getSDLButton(arcadeButton);
    bool isPressed = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(sdlButton);
    return isPressed;
}

std::pair<size_t, size_t> SDL::getMousePosition() const {
    if (!_window)
        return {0, 0};
    int x, y;
    SDL_GetMouseState(&x, &y);
    x = (x < 0) ? 0 : x;
    y = (y < 0) ? 0 : y;
    return {static_cast<size_t>(x), static_cast<size_t>(y)};
}
