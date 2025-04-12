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
#include "Models/ModuleInfos.hpp"

SDLModule::~SDLModule() {
    stop();
}

void SDLModule::init(const Arcade::IWindowModule& windowParam) {
    if (_initialized)
        return;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL could not initialize! SDL Error: "
            + std::string(SDL_GetError()));
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Quit();
        throw std::runtime_error
        ("SDL_image could not initialize! SDL_image Error: "
            + std::string(IMG_GetError()));
    }
    if (TTF_Init() == -1) {
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("SDL_ttf could not initialize! SDL_ttf Error: "
            + std::string(TTF_GetError()));
    }
    int width = static_cast<int>(windowParam.getWidth());
    int height = static_cast<int>(windowParam.getHeight());
    _window.createWindow(width, height);
    _renderer.createRenderer(_window.getWindow());
    _windowWidth = static_cast<int>(width);
    _windowHeight = static_cast<int>(height);
    _initialized = true;
}

void SDLModule::stop() {
    if (!_initialized)
        return;
    _renderer.destroyRenderer();
    _window.destroyWindow();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    _initialized = false;
}

void SDLModule::clearScreen() {
    _renderer.clearScreen();
}

void SDLModule::refreshScreen() {
    _renderer.refreshScreen();
}

void SDLModule::drawDrawable
(std::shared_ptr<Arcade::IDrawableComponent> drawable) {
    if (!drawable->isRenderable())
        return;

    if (drawable->shouldRenderAsText()) {
        auto renderer = _renderer.getRenderer();
        int fontSize = static_cast<int>(drawable->getScale());
        if (fontSize <= 0) fontSize = 24;
        _text.drawText(renderer.get(),
                      drawable->getText(),
                      static_cast<int>(drawable->getPositionX()),
                      static_cast<int>(drawable->getPositionY()),
                      fontSize,
                      drawable->getColor());
    } else if (drawable->shouldRenderAsTexture()) {
        auto renderer = _renderer.getRenderer();

        auto surface = _surface.loadSurface(drawable->getPath());
        if (!surface)
            return;
        auto texture = _texture.createTexture(renderer.get(),
            surface.get(), drawable->getPath());
        if (!texture)
            return;
        _texture.renderTexture(renderer.get(), texture.get(),
            drawable->getPositionX(),
            drawable->getPositionY());
    } else if (drawable->shouldRenderAsCharacter()) {
        auto renderer = _renderer.getRenderer();
        std::string charAsString(1, drawable->getCharacter());
        _text.drawText(renderer.get(),
                      charAsString,
                      drawable->getPositionX(),
                      drawable->getPositionY(),
                      24,
                      drawable->getColor());
    }
}

void SDLModule::pollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            _running = false;
            stop();
        }
    }
}

int SDLModule::getWidth() const {
    return _window.getWidth();
}

int SDLModule::getHeight() const {
    return _window.getWidth();
}

bool SDLModule::isOpen() const {
    return (_running);
}

bool SDLModule::isKeyPressed(int keyCode) {
    return _event.isKeyPressed(keyCode);
}

bool SDLModule::isMouseButtonPressed(int button) const {
    return _event.isMouseButtonPressed(button);
}

std::pair<size_t, size_t> SDLModule::getMousePosition() const {
    return _event.getMousePosition();
}

const std::string& SDLModule::getName() const {
    return _name;
}

extern "C" __attribute__((constructor))
    const char *init_sdl(void) {
        return "Lib";
}

extern "C" __attribute__((destructor)) void fini_sdl(void) { }

extern "C" Arcade::IArcadeModule* entryPoint(void) {
    return new SDLModule();
}

extern "C" Arcade::ModuleInfos module_infos() {
    return {"SDL2", "IDK",
        "IDK",
        "./lib/arcade_sdl2.so", Arcade::ModuleType::GRAPHIC_LIB};
}
