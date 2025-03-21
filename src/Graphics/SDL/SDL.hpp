// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDL
*/

#ifndef SRC_GRAPHICS_SDL_SDL_HPP_
    #define SRC_GRAPHICS_SDL_SDL_HPP_
    #include <SDL2/SDL.h>
    #include <memory>
    #include <map>
    #include <utility>
    #include <string>
    #include <vector>
    #include "../../Shared/Interface/IDisplayModule.hpp"

    class SDL : public Arcade::IDisplayModule {
     private:
        std::string _name;
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window;
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer;
        std::vector<std::pair<std::unique_ptr<SDL_Texture,
            decltype(&SDL_DestroyTexture)>, std::pair<int, int>>> _sprite;
        int _windowWidth = 0;
        int _windowHeight = 0;
        bool _running = true;
     public:
        SDL() : _name("SDL2"),
        _window(nullptr, SDL_DestroyWindow),
        _renderer(nullptr, SDL_DestroyRenderer) {}
        ~SDL() override;
        void init(float width, float height) override;
        void stop() override;
        void clearScreen();
        void refreshScreen();
        void drawEntity(int x, int y, char symbol);
        void drawTexture(int x, int y, const std::string &textureId);
        void drawText(int x, int y, const std::string &text);
        void pollEvents();
        bool isOpen() const;
        const std::string& getName() const override { return _name;};
    };

#endif  // SRC_GRAPHICS_SDL_SDL_HPP_
