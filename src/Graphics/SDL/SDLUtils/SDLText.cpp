// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLText
*/
#include "SDLUtils/SDLText.hpp"
#include <string>

SDL::SDLText::SDLText() {}

void SDL::SDLText::drawText(SDL_Renderer* renderer, const std::string& text,
    int x, int y, int fontSize) {

    _font.loadFont(fontSize);
    auto font = _font.getFont();

    SDL_Color textColor = {255, 255, 255, 255};
    auto surface = _surface.createTextSurface(font.get(), text, textColor);
    if (!surface)
        return;
    auto texture = _texture.createTextTexture(renderer, surface.get());
    if (!texture)
        return;
    int textWidth, textHeight;
    TTF_SizeText(font.get(), text.c_str(), &textWidth, &textHeight);
    _texture.renderTextTexture(renderer, texture.get(), x, y,
        textWidth, textHeight);
}
