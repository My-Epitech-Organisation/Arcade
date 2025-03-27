// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLSurface
*/
#include "SDLUtils/SDLSurface.hpp"
#include <string>
#include <memory>
#include <iostream>

std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
    SDL::SDLSurface::loadSurface(const std::string &texturePath) {
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> tempSurface(
        IMG_Load(texturePath.c_str()), SDL_FreeSurface);
    if (!tempSurface) {
        throw std::runtime_error("Unable to load image " + texturePath +
            "! SDL_image Error: " + std::string(IMG_GetError()));
    }
    return tempSurface;
}

std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
    SDL::SDLSurface::createTextSurface(TTF_Font* font,
        const std::string& text, SDL_Color color) {
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> textSurface(
        TTF_RenderText_Solid(font, text.c_str(), color),
        SDL_FreeSurface);

    if (!textSurface) {
        std::cerr << "Unable to render text surface: " <<
            TTF_GetError() << std::endl;
    }
    return textSurface;
}
