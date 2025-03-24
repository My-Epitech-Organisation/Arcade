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

    class GTK : public Arcade::IDisplayModule {
     private:
        std::string _name;
        std::shared_ptr<GtkApplication> _app;
        std::shared_ptr<GtkWidget> _window;
        std::shared_ptr<GtkWidget> _drawingArea;
        std::shared_ptr<cairo_surface_t> _surface;
        std::shared_ptr<cairo_t> _cr;
        int _windowWidth = 0;
        int _windowHeight = 0;
        bool _running = true;

        void createWindow(int width, int height);
        void setupDrawingArea();
        void loadFonts();
        void draw(std::shared_ptr<GtkDrawingArea> area,
            std::shared_ptr<cairo_t> cr, int width, int height);
        void handleEvents();

        struct TextureData {
            std::shared_ptr<cairo_surface_t> surface;
            int width;
            int height;
        };

        std::unordered_map<std::string, TextureData> _textures;
        std::unordered_map<std::string,
            std::shared_ptr<PangoFontDescription>> _fonts;

        std::shared_ptr<cairo_surface_t>
            loadTexture(const std::string& texturePath);
        std::shared_ptr<PangoFontDescription>
            loadFont(const std::string& fontPath);
        void drawTextInternal(std::shared_ptr<cairo_t> cr, int x, int y,
            const std::string &text,
            std::shared_ptr<PangoFontDescription> font);

     public:
        GTK();
        ~GTK() override;
        void init(float width = 800.f, float height = 600.f) override;
        void stop() override;
        void clearScreen() override;
        void refreshScreen() override;
        void drawEntity(int x, int y, char symbol) override;
        void drawTexture(int x, int y, const std::string &textureId) override;
        void drawText(int x, int y, const std::string &text) override;
        void pollEvents() override;
        bool isOpen() const override;
        const std::string& getName() const override { return _name; }

        static void on_draw(GtkDrawingArea *area, cairo_t *cr,
                           int width, int height, gpointer user_data);
        static void on_activate(GtkApplication *app, gpointer user_data);
    };

#endif  // SRC_GRAPHICS_GTK__GTK_HPP_
