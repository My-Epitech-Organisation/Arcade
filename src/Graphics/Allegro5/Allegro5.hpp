// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5
*/

#ifndef SRC_GRAPHICS_ALLEGRO5_ALLEGRO5_HPP_
    #define SRC_GRAPHICS_ALLEGRO5_ALLEGRO5_HPP_
    #include <allegro5/allegro.h>
    #include <allegro5/allegro_image.h>
    #include <allegro5/allegro_font.h>
    #include <allegro5/allegro_ttf.h>
    #include <memory>
    #include <utility>
    #include <string>
    #include <unordered_map>
    #include "Interface/Display/IDisplayModule.hpp"
    #include "Interface/Display/IDrawableComponent.hpp"
    #include "Interface/Core/IWindowModule.hpp"
    #include "Allegro5Utils/Allegro5Window.hpp"
    #include "Allegro5Utils/Allegro5Event.hpp"
    #include "Allegro5Utils/Allegro5Text.hpp"
    #include "Allegro5Utils/Allegro5Texture.hpp"
    #include "Models/ColorType.hpp"

    class Allegro5 : public Arcade::IDisplayModule {
     private:
        std::string _name;
        Allegro::Allegro5Window _window;
        Allegro::Allegro5Event _event;
        Allegro::Allegro5Text _text;
        Allegro::Allegro5Texture _texture;
        void drawEntity(int x, int y, char symbol);
        void drawTexture(int x, int y, const std::string &textureId);
        void drawText(const std::string &text, int x, int y,
            Arcade::Color color);

     public:
        Allegro5() : _name("Allegro5") {}
        ~Allegro5() override;
        void init(const Arcade::IWindowModule& windowParam) override;
        void stop() override;
        void clearScreen() override;
        void refreshScreen() override;
        void drawDrawable(std::shared_ptr<Arcade::IDrawableComponent> drawable)
            override;
        void pollEvents() override;
        bool isOpen() const override;
        const std::string& getName() const override;
        int getWidth() const override;
        int getHeight() const override;
        bool isKeyPressed(int keyCode) override;
        bool isMouseButtonPressed(int button) const override;
        std::pair<size_t, size_t> getMousePosition() const override;
    };

#endif  // SRC_GRAPHICS_ALLEGRO5_ALLEGRO5_HPP_
