// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5Font
*/

#ifndef SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5FONT_HPP_
    #define SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5FONT_HPP_
    #include <allegro5/allegro.h>
    #include <allegro5/allegro_font.h>
    #include <allegro5/allegro_ttf.h>
    #include <memory>
    #include <string>
    #include <unordered_map>

namespace Allegro {

class Allegro5Font {
 private:
    std::unordered_map<std::string,
        std::unique_ptr<ALLEGRO_FONT, decltype(&al_destroy_font)>> _fonts;

 public:
    Allegro5Font() = default;
    ~Allegro5Font() = default;

    std::unique_ptr<ALLEGRO_FONT, decltype(&al_destroy_font)>
        loadFont(const std::string& fontPath, int fontSize = 24);
};

}  // namespace Allegro

#endif  // SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5FONT_HPP_
