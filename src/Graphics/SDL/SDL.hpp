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
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
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
        int _windowWidth = 0;
        int _windowHeight = 0;
        bool _running = true;
        void createWindow(int width, int height);
        void createRenderer();
        std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
            loadSurface(const std::string &texturePath);
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>
            createTexture(SDL_Surface *surface,
                const std::string &texturePath);
        void renderTexture(SDL_Texture *texture);
        std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>
            loadFont(int fontSize);
        std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
            createTextSurface(TTF_Font* font, const std::string& text,
                SDL_Color color);
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>
            createTextTexture(SDL_Surface* surface);
        void renderTextTexture(SDL_Texture* texture, int x, int y,
            int width, int height);

     public:
        SDL() : _name("SDL2"),
        _window(nullptr, SDL_DestroyWindow),
        _renderer(nullptr, SDL_DestroyRenderer) {}
        ~SDL() override;
        void init(float width = 800.f, float height = 600.f) override;
        void stop() override;
        void clearScreen() override;
        void refreshScreen() override;
        void drawEntity(int x, int y, char symbol) override;
        void drawTexture(int x, int y, const std::string &textureId) override;
        void drawText(int x, int y, const std::string &text) override;
        void pollEvents() override;
        bool isOpen() const override;
        const std::string& getName() const override { return _name;};
    };

#endif  // SRC_GRAPHICS_SDL_SDL_HPP_
