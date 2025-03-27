// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLFont
*/
#include "SDLUtils/SDLFont.hpp"
#include <iostream>
#include <memory>

void SDL::SDLFont::loadFont(int fontSize) {
    auto newFont = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>(
        TTF_OpenFont("assets/fonts/arial.ttf", fontSize),
        TTF_CloseFont);

    if (!newFont) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    _font.swap(newFont);
}

std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> SDL::SDLFont::getFont() {
    auto fontPtr = _font.get();
    return std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>(fontPtr,
        [](TTF_Font*){});
}
