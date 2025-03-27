// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GTK+ class
*/

#ifndef SRC_GRAPHICS_GTK__GTK_HPP_
    #define SRC_GRAPHICS_GTK__GTK_HPP_
    #include <gtk/gtk.h>
    #include <memory>
    #include <string>
    #include <unordered_map>
    #include <functional>
    #include "../../Shared/Interface/Display/IDisplayModule.hpp"
    #include "GTKUtils/GTKWindow.hpp"
    #include "GTKUtils/GTKRenderer.hpp"
    #include "GTKUtils/GTKTexture.hpp"
    #include "GTKUtils/GTKText.hpp"
    #include "GTKUtils/GTKFont.hpp"
    #include "GTKUtils/GTKEvent.hpp"

    class GTKModule : public Arcade::IDisplayModule {
     private:
        std::string _name;
        std::shared_ptr<GtkApplication> _app;
        int _windowWidth = 0;
        int _windowHeight = 0;
        bool _running = true;
        GTK::GTKWindow _window;
        GTK::GTKRenderer _renderer;
        GTK::GTKTexture _textureManager;
        GTK::GTKText _textManager;
        GTK::GTKFont _fontManager;
        GTK::GTKEvent _eventManager;
        struct TextureData {
            std::shared_ptr<cairo_surface_t> surface;
            int width;
            int height;
        };
        std::unordered_map<std::string, TextureData> _textures;

        static void on_draw(GtkDrawingArea *area, cairo_t *cr,
            int width, int height, gpointer user_data);
        static void on_activate(GtkApplication *app, gpointer user_data);

        void draw(std::shared_ptr<GtkDrawingArea> area,
            std::shared_ptr<cairo_t> cr, int width, int height);

     public:
        GTKModule();
        ~GTKModule() override;
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

#endif  // SRC_GRAPHICS_GTK__GTK_HPP_
