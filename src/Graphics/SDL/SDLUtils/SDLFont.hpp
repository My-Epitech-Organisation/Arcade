// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLFont
*/

#ifndef SRC_GRAPHICS_SDL_SDLUTILS_SDLFONT_HPP_
    #define SRC_GRAPHICS_SDL_SDLUTILS_SDLFONT_HPP_
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
    #include <memory>
    #include <iostream>

namespace SDL {
class SDLFont {
 private:
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> _font;
 public:
    SDLFont() : _font(nullptr, TTF_CloseFont) {}
    void loadFont(int fontSize);
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> getFont();
};
}  // namespace SDL
#endif  // SRC_GRAPHICS_SDL_SDLUTILS_SDLFONT_HPP_
