// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLModule
*/

#ifndef SRC_GRAPHICS_SDL_SDL_HPP_
    #define SRC_GRAPHICS_SDL_SDL_HPP_
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <memory>
    #include <map>
    #include <utility>
    #include <string>
    #include <vector>
    #include "Interface/Display/IDisplayModule.hpp"
    #include "Interface/Display/IDrawableComponent.hpp"
    #include "Interface/Core/IWindowModule.hpp"
    #include "Models/ColorType.hpp"
    #include "Models/MouseButtonType.hpp"
    #include "SDLUtils/SDLWindow.hpp"
    #include "SDLUtils/SDLRenderer.hpp"
    #include "SDLUtils/SDLTexture.hpp"
    #include "SDLUtils/SDLSurface.hpp"
    #include "SDLUtils/SDLFont.hpp"
    #include "SDLUtils/SDLText.hpp"
    #include "SDLUtils/SDLEvent.hpp"

class SDLModule : public Arcade::IDisplayModule {
 private:
    std::string _name;
    SDL::SDLWindow _window;
    SDL::SDLRenderer _renderer;
    SDL::SDLSurface _surface;
    SDL::SDLTexture _texture;
    SDL::SDLText _text;
    SDL::SDLEvent _event;
    int _windowWidth = 0;
    int _windowHeight = 0;
    bool _running = true;
    bool _initialized = false;

 public:
    SDLModule() : _name("SDL2") {}
    ~SDLModule() override;
    void init(const Arcade::IWindowModule& windowParam) override;
    void stop() override;
    void clearScreen() override;
    void refreshScreen() override;
    void drawDrawable(std::shared_ptr<Arcade::IDrawableComponent> drawable) override;
    void pollEvents() override;
    bool isOpen() const override;
    const std::string& getName() const override;
    int getWidth() const override;
    int getHeight() const override;
    bool isKeyPressed(int keyCode) override;
    bool isMouseButtonPressed(int button) const override;
    std::pair<size_t, size_t> getMousePosition() const override;
};

#endif  // SRC_GRAPHICS_SDL_SDL_HPP_
