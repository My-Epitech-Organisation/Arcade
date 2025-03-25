// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5Utils
*/
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include "Allegro5/Allegro5.hpp"

std::unique_ptr<ALLEGRO_FONT, decltype(&al_destroy_font)>
Allegro5::loadFont(const std::string& fontPath, int fontSize) {
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

std::unique_ptr<ALLEGRO_BITMAP, decltype(&al_destroy_bitmap)>
Allegro5::loadBitmap(const std::string& bitmapPath) {
    auto it = _bitmaps.find(bitmapPath);
    if (it == _bitmaps.end()) {
        auto bitmap = std::unique_ptr<ALLEGRO_BITMAP,
            decltype(&al_destroy_bitmap)>(
            al_load_bitmap(bitmapPath.c_str()), al_destroy_bitmap);
        if (!bitmap) {
            std::cerr << "Failed to load bitmap: " << bitmapPath << std::endl;
            return std::unique_ptr<ALLEGRO_BITMAP,
                decltype(&al_destroy_bitmap)>(nullptr, al_destroy_bitmap);
        }
        auto result = _bitmaps.emplace(bitmapPath, std::move(bitmap));
        it = result.first;
    }
    auto bitmapPtr = it->second.get();
    return std::unique_ptr<ALLEGRO_BITMAP,
        decltype(&al_destroy_bitmap)>(bitmapPtr, [](ALLEGRO_BITMAP*){});
}
