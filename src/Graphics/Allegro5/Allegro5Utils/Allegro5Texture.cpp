// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5Texture
*/
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include "Allegro5Utils/Allegro5Texture.hpp"

namespace Allegro {

std::unique_ptr<ALLEGRO_BITMAP, decltype(&al_destroy_bitmap)>
Allegro5Texture::loadBitmap(const std::string& bitmapPath) {
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

}  // namespace Allegro
