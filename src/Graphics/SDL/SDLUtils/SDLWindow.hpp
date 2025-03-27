// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLWindow
*/

#ifndef SRC_GRAPHICS_SDL_SDLUTILS_SDLWINDOW_HPP_
    #define SRC_GRAPHICS_SDL_SDLUTILS_SDLWINDOW_HPP_
    #include <SDL2/SDL.h>
    #include <memory>
    #include <string>
    #include <stdexcept>

namespace SDL {
class SDLWindow {
 private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window;
    int _windowWidth = 0;
    int _windowHeight = 0;

 public:
    SDLWindow() : _window(nullptr, SDL_DestroyWindow) {}
    void createWindow(int width, int height);
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> getWindow();
    int getHeight() const;
    int getWidth() const;
};
}  // namespace SDL

#endif  // SRC_GRAPHICS_SDL_SDLUTILS_SDLWINDOW_HPP_
