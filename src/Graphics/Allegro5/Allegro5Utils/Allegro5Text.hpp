// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5Text
*/

#ifndef SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5TEXT_HPP_
    #define SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5TEXT_HPP_
    #include <allegro5/allegro.h>
    #include <allegro5/allegro_font.h>
    #include <allegro5/allegro_ttf.h>
    #include <memory>
    #include <string>
    #include "Allegro5Utils/Allegro5Font.hpp"
    #include "Models/ColorType.hpp"

namespace Allegro {

class Allegro5Text {
 private:
    Allegro5Font _font;
    ALLEGRO_COLOR _color;

 public:
    Allegro5Text();
    ~Allegro5Text() = default;

    void init();
    void drawText(int x, int y, const std::string& text, Arcade::Color color);
};

}  // namespace Allegro

#endif  // SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5TEXT_HPP_
