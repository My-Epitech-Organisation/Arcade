// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLText
*/
#include <string>
#include "SDLUtils/SDLText.hpp"
#include "Models/ColorType.hpp"
#include "SDL/SDLColor.hpp"

SDL::SDLText::SDLText() {}

void SDL::SDLText::drawText(SDL_Renderer* renderer, const std::string& text,
    int x, int y, int fontSize, Arcade::Color color) {

    _font.loadFont(fontSize);
    if (_font.getFont() == nullptr)
        exit(84);
    auto font = _font.getFont();

    auto surface = _surface.createTextSurface(font.get(), text,
        SDLColor::convertColor(color));
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
