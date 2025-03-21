// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** SFML class
*/

#ifndef SRC_GRAPHICS_SFML_SFML_HPP_
    #define SRC_GRAPHICS_SFML_SFML_HPP_
    #include <memory>
    #include <string>
    #include <unordered_map>
    #include <SFML/Graphics.hpp>
    #include "../../Shared/Interface/IDisplayModule.hpp"

    class SFML : public Arcade::IDisplayModule {
     private:
        std::string _name;
        std::unique_ptr<sf::RenderWindow> _window;
        int _windowWidth = 0;
        int _windowHeight = 0;
        bool _running = true;
        void createWindow(int width, int height);
        sf::Font* loadFont(const std::string& fontPath);
        sf::Texture* loadTexture(const std::string& texturePath);
        std::unique_ptr<sf::RenderWindow> _window;
        std::unordered_map<std::string, std::unique_ptr<sf::Font>> _fonts;
        std::unordered_map<std::string, std::unique_ptr<sf::Texture>> _textures;

     public:
        SFML() : _name("SFML") {}
        ~SFML() override;
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
#endif  // SRC_GRAPHICS_SFML_SFML_HPP_
