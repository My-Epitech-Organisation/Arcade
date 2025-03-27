// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLRenderer
*/
#include "SDLUtils/SDLRenderer.hpp"
#include <string>
#include <memory>

void SDL::SDLRenderer::createRenderer(std::unique_ptr<SDL_Window,
        decltype(&SDL_DestroyWindow)> _window) {
    _renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
        SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer);
    if (!_renderer) {
        SDL_Quit();
        throw std::runtime_error("Renderer could not be created! SDL Error: "
            + std::string(SDL_GetError()));
    }
}

std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>
    SDL::SDLRenderer::getRenderer() {
    auto rendererPtr = _renderer.get();
    return std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
        rendererPtr,
        [](SDL_Renderer*){});
}

void SDL::SDLRenderer::clearScreen() {
    SDL_RenderClear(_renderer.get());
}

void SDL::SDLRenderer::refreshScreen() {
    SDL_RenderPresent(_renderer.get());
}
