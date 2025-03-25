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
    #include <string>
    #include <unordered_map>
    #include "Interface/Display/IDisplayModule.hpp"

    class Allegro5 : public Arcade::IDisplayModule {
     private:
        std::string _name;
        std::unique_ptr<ALLEGRO_DISPLAY,
            decltype(&al_destroy_display)> _display;
        std::unique_ptr<ALLEGRO_EVENT_QUEUE,
            decltype(&al_destroy_event_queue)> _eventQueue;
        int _windowWidth = 0;
        int _windowHeight = 0;
        bool _running = true;
        void createWindow(int width, int height);
        std::unique_ptr<ALLEGRO_FONT, decltype(&al_destroy_font)>
            loadFont(const std::string& fontPath, int fontSize = 24);
        std::unique_ptr<ALLEGRO_BITMAP, decltype(&al_destroy_bitmap)>
            loadBitmap(const std::string& bitmapPath);
        std::unordered_map<std::string,
            std::unique_ptr<ALLEGRO_FONT, decltype(&al_destroy_font)>> _fonts;
        std::unordered_map<std::string,
            std::unique_ptr<ALLEGRO_BITMAP,
                decltype(&al_destroy_bitmap)>> _bitmaps;

     public:
        Allegro5() : _name("Allegro5"),
            _display(nullptr, al_destroy_display),
            _eventQueue(nullptr, al_destroy_event_queue) {}
        ~Allegro5() override;
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

#endif  // SRC_GRAPHICS_ALLEGRO5_ALLEGRO5_HPP_
