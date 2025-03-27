// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5Font
*/
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include "Allegro5Utils/Allegro5Font.hpp"

namespace Allegro {

std::unique_ptr<ALLEGRO_FONT, decltype(&al_destroy_font)>
Allegro5Font::loadFont(const std::string& fontPath, int fontSize) {
    std::string fontKey = fontPath + "_" + std::to_string(fontSize);
    auto it = _fonts.find(fontKey);
    if (it == _fonts.end()) {
        auto font = std::unique_ptr<ALLEGRO_FONT, decltype(&al_destroy_font)>(
            al_load_ttf_font(fontPath.c_str(), fontSize, 0), al_destroy_font);
        if (!font) {
            std::cerr << "Failed to load font: " << fontPath << std::endl;
            return std::unique_ptr<ALLEGRO_FONT,
                decltype(&al_destroy_font)>(nullptr, al_destroy_font);
        }
        auto result = _fonts.emplace(fontKey, std::move(font));
        it = result.first;
    }
    auto fontPtr = it->second.get();
    return std::unique_ptr<ALLEGRO_FONT,
        decltype(&al_destroy_font)>(fontPtr, [](ALLEGRO_FONT*){});
}

}  // namespace Allegro
