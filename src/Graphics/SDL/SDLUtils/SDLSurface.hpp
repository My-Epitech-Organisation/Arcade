// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLSurface
*/

#ifndef SRC_GRAPHICS_SDL_SDLUTILS_SDLSURFACE_HPP_
    #define SRC_GRAPHICS_SDL_SDLUTILS_SDLSURFACE_HPP_
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <memory>
    #include <string>
    #include <stdexcept>
    #include <iostream>

namespace SDL {
class SDLSurface {
 public:
    SDLSurface() : _surface(nullptr, SDL_FreeSurface) {}
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
        loadSurface(const std::string &texturePath);
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
        createTextSurface(TTF_Font* font,
            const std::string& text, SDL_Color color);
 private:
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> _surface;
};
}  // namespace SDL

#endif  // SRC_GRAPHICS_SDL_SDLUTILS_SDLSURFACE_HPP_
