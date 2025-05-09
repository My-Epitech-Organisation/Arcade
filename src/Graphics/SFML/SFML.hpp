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
    #include <utility>
    #include <string>
    #include <unordered_map>
    #include <SFML/Graphics.hpp>
    #include "Interface/Display/IDisplayModule.hpp"
    #include "Interface/Core/IWindowModule.hpp"
    #include "Models/ColorType.hpp"

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
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> _fonts;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> _textures;
    void drawEntity(int x, int y, char symbol, float scale);
    void drawTexture(int x, int y, const std::string &textureId, float scale);
    void drawText(const std::string &text, int x, int y,
        Arcade::Color, float scale);

 public:
    SFML() : _name("SFML") {}
    ~SFML() override;
    void init(const Arcade::IWindowModule& windowParam) override;
    void stop() override;
    void clearScreen() override;
    void refreshScreen() override;
    void pollEvents() override;
    bool isOpen() const override;
    void drawDrawable(std::shared_ptr<Arcade::IDrawableComponent>
        drawable) override;
    const std::string& getName() const override;
    int getWidth() const override;
    int getHeight() const override;
    bool isKeyPressed(int keyCode) override;
    bool isMouseButtonPressed(int button) const override;
    std::pair<size_t, size_t> getMousePosition() const override;
};
#endif  // SRC_GRAPHICS_SFML_SFML_HPP_
