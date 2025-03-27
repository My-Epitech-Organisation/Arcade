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
    #include "SDLUtils/SDLWindow.hpp"
    #include "SDLUtils/SDLRenderer.hpp"
    #include "SDLUtils/SDLTexture.hpp"
    #include "SDLUtils/SDLSurface.hpp"
    #include "SDLUtils/SDLFont.hpp"
    #include "SDLUtils/SDLText.hpp"

    class SDLModule : public Arcade::IDisplayModule {
     private:
        std::string _name;
        SDL::SDLWindow _window;
        SDL::SDLRenderer _renderer;
        SDL::SDLSurface _surface;
        SDL::SDLTexture _texture;
        SDL::SDLText _text;
        int _windowWidth = 0;
        int _windowHeight = 0;
        bool _running = true;

     public:
        SDLModule() : _name("SDL2") {}
        ~SDLModule() override;
        void init(float width = 800.f, float height = 600.f) override;
        void stop() override;
        void clearScreen() override;
        void refreshScreen() override;
        void drawEntity(int x, int y, char symbol) override;
        void drawTexture(int x, int y, const std::string &textureId) override;
        void drawText(int x, int y, const std::string &text) override;
        void pollEvents() override;
        bool isOpen() const override;
        const std::string& getName() const override;
    };

#endif  // SRC_GRAPHICS_SDL_SDL_HPP_
