// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** SDLEvent
*/

#include "SDLUtils/SDLEvent.hpp"
#include "SDL/SDLKeys.hpp"
#include "Models/KeysType.hpp"

namespace SDL {
bool SDLEvent::isKeyPressed(int keyCode) const {
    SDL_Keycode sdlKey = Arcade::SDLKeyMap::getSDLKey(
        static_cast<Arcade::Keys>(keyCode));
    SDL_Scancode scanCode = SDL_GetScancodeFromKey(sdlKey);
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    return state[scanCode] != 0;
}

bool SDLEvent::isMouseButtonPressed(int button) const {
    auto arcadeButton = static_cast<Arcade::MouseButton>(button);
    Uint8 sdlButton = Arcade::SDLKeyMap::getSDLButton(arcadeButton);
    bool isPressed = SDL_GetMouseState(nullptr, nullptr)
        & SDL_BUTTON(sdlButton);
    return isPressed;
}

std::pair<size_t, size_t> SDLEvent::getMousePosition() const {
    int x, y;
    SDL_GetMouseState(&x, &y);
    x = (x < 0) ? 0 : x;
    y = (y < 0) ? 0 : y;
    return {static_cast<size_t>(x), static_cast<size_t>(y)};
}
}  // namespace SDL
