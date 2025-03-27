// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5Texture
*/

#ifndef SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5TEXTURE_HPP_
    #define SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5TEXTURE_HPP_
    #include <allegro5/allegro.h>
    #include <allegro5/allegro_image.h>
    #include <memory>
    #include <string>
    #include <unordered_map>

namespace Allegro {

class Allegro5Texture {
 private:
    std::unordered_map<std::string,
        std::unique_ptr<ALLEGRO_BITMAP,
            decltype(&al_destroy_bitmap)>> _bitmaps;

 public:
    Allegro5Texture() = default;
    ~Allegro5Texture() = default;

    std::unique_ptr<ALLEGRO_BITMAP, decltype(&al_destroy_bitmap)>
        loadBitmap(const std::string& bitmapPath);
};

}  // namespace Allegro

#endif  // SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5TEXTURE_HPP_
