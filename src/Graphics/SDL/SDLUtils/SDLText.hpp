// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLText
*/

#ifndef SRC_GRAPHICS_SDL_SDLUTILS_SDLTEXT_HPP_
    #define SRC_GRAPHICS_SDL_SDLUTILS_SDLTEXT_HPP_
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
    #include <memory>
    #include <string>
    #include "SDLUtils/SDLFont.hpp"
    #include "SDLUtils/SDLSurface.hpp"
    #include "SDLUtils/SDLTexture.hpp"
    #include "Models/ColorType.hpp"

namespace SDL {
class SDLText {
 public:
    SDLText();
    void drawText(SDL_Renderer* renderer, const std::string& text, int x,
        int y, int fontSize = 24, Arcade::Color color = Arcade::WHITE);
 private:
    SDLFont _font;
    SDLSurface _surface;
    SDLTexture _texture;
};
}  // namespace SDL

#endif  // SRC_GRAPHICS_SDL_SDLUTILS_SDLTEXT_HPP_
