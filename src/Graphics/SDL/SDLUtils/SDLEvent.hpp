// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLFont
*/

#ifndef SRC_GRAPHICS_SDL_SDLUTILS_SDLEVENT_HPP_
    #define SRC_GRAPHICS_SDL_SDLUTILS_SDLEVENT_HPP_
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
    #include <memory>
    #include <iostream>
    #include <utility>

namespace SDL {
class SDLEvent {
 public:
    ~SDLEvent() = default;

    bool isKeyPressed(int keyCode) const;
    bool isMouseButtonPressed(int button) const;
    std::pair<size_t, size_t> getMousePosition() const;
};
};  // namespace SDL
#endif  // SRC_GRAPHICS_SDL_SDLUTILS_SDLEVENT_HPP_
