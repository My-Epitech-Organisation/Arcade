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
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
        std::vector<std::pair<std::unique_ptr<SDL_Texture,
            decltype(&SDL_DestroyTexture)>, std::pair<int, int>>> _sprite;
        int windowWidth = 0;
        int windowHeight = 0;
     public:
        SDL() : _name("SDL2"),
        window(nullptr, SDL_DestroyWindow),
        renderer(nullptr, SDL_DestroyRenderer) {}
        ~SDL() override;
        void init() override;
        void init(float x, float y);
        void drawElement() override;
        void addSprite(const std::string &textPath);
        void render();
        InputModel::Input getInput() override;
        void stop() override;
        const std::string& getName() const override { return _name;};
    };

#endif  // SRC_GRAPHICS_SDL_SDL_HPP_
