// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLRenderer
*/

#ifndef SRC_GRAPHICS_SDL_SDLUTILS_SDLRENDERER_HPP_
    #define SRC_GRAPHICS_SDL_SDLUTILS_SDLRENDERER_HPP_
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <memory>
    #include <string>
    #include <stdexcept>

namespace SDL {
class SDLRenderer {
 public:
    SDLRenderer() : _renderer(nullptr, SDL_DestroyRenderer) {}
    void createRenderer(std::unique_ptr<SDL_Window,
        decltype(&SDL_DestroyWindow)> _window);
    std::unique_ptr<SDL_Renderer,
        decltype(&SDL_DestroyRenderer)> getRenderer();
    void clearScreen();
    void refreshScreen();
    void destroyRenderer();
    SDL_Renderer* getRendererPtr() const { return _renderer.get(); }
 private:
    std::unique_ptr<SDL_Renderer,
        decltype(&SDL_DestroyRenderer)> _renderer;
};
}  // namespace SDL

#endif  // SRC_GRAPHICS_SDL_SDLUTILS_SDLRENDERER_HPP_
