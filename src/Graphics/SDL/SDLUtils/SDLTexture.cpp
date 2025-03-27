// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLTexture
*/
#include "SDLUtils/SDLTexture.hpp"
#include <string>
#include <memory>
#include <iostream>

std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>
    SDL::SDLTexture::createTexture(SDL_Renderer* renderer,
        SDL_Surface* surface, const std::string& texturePath) {
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texturePtr(
        SDL_CreateTextureFromSurface(renderer, surface),
        SDL_DestroyTexture);
    if (!texturePtr) {
        throw std::runtime_error("Unable to create texture from "
            + texturePath + "! SDL Error: " + std::string(SDL_GetError()));
    }
    return texturePtr;
}

std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>
    SDL::SDLTexture::createTextTexture(SDL_Renderer* renderer,
        SDL_Surface* surface) {
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> textTexture(
        SDL_CreateTextureFromSurface(renderer, surface),
        SDL_DestroyTexture);

    if (!textTexture) {
        std::cerr << "Unable to create texture from rendered text: "
            << SDL_GetError() << std::endl;
    }
    return textTexture;
}

void SDL::SDLTexture::renderTexture(SDL_Renderer* renderer,
    SDL_Texture* texture, int windowWidth, int windowHeight) {
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

    SDL_Rect destRect;
    destRect.x = (windowWidth - w) / 2;
    destRect.y = (windowHeight - h) / 2;
    destRect.w = w;
    destRect.h = h;

    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

void SDL::SDLTexture::renderTextTexture(SDL_Renderer* renderer,
    SDL_Texture* texture, int x, int y, int width, int height) {
    SDL_Rect renderRect = {x, y, width, height};
    SDL_RenderCopy(renderer, texture, nullptr, &renderRect);
}
