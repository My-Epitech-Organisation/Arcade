// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLTexture
*/

#ifndef SRC_GRAPHICS_SDL_SDLUTILS_SDLTEXTURE_HPP_
    #define SRC_GRAPHICS_SDL_SDLUTILS_SDLTEXTURE_HPP_
    #include <SDL2/SDL.h>
    #include <memory>
    #include <string>
    #include <stdexcept>
    #include <iostream>

namespace SDL {
class SDLTexture {
 public:
    SDLTexture() : _texture(nullptr, SDL_DestroyTexture) {}
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>
        createTexture(SDL_Renderer* renderer, SDL_Surface* surface,
            const std::string& texturePath);
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>
        createTextTexture(SDL_Renderer* renderer, SDL_Surface* surface);
    void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture,
        int windowWidth, int windowHeight);
    void renderTextTexture(SDL_Renderer* renderer, SDL_Texture* texture,
        int x, int y, int width, int height);
 private:
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> _texture;
};
}  // namespace SDL

#endif  // SRC_GRAPHICS_SDL_SDLUTILS_SDLTEXTURE_HPP_
