// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5Text
*/
#include <iostream>
#include <string>
#include "Allegro5Utils/Allegro5Text.hpp"
#include "Allegro5/Allegro5Color.hpp"

namespace Allegro {

Allegro5Text::Allegro5Text() : _color(al_map_rgb(255, 255, 255)) {
}

void Allegro5Text::init() {
    al_init_font_addon();
    al_init_ttf_addon();
}

void Allegro5Text::drawText(int x, int y, const std::string& text,
Arcade::Color color) {
    auto font = _font.loadFont("assets/fonts/arial.ttf", 24);
    _color = Allegro5Color::convertColor(color);
    if (font) {
        al_draw_text(font.get(), _color, x, y,
            ALLEGRO_ALIGN_LEFT, text.c_str());
    }
}

}  // namespace Allegro
