// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5Window
*/

#ifndef SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5WINDOW_HPP_
    #define SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5WINDOW_HPP_
    #include <allegro5/allegro.h>
    #include <memory>
    #include <string>

namespace Allegro {

class Allegro5Window {
 private:
    std::unique_ptr<ALLEGRO_DISPLAY,
        decltype(&al_destroy_display)> _display;
    int _width;
    int _height;

 public:
    Allegro5Window();
    ~Allegro5Window() = default;

    void createWindow(int width, int height);
    ALLEGRO_DISPLAY* getDisplay() const;
    int getWidth() const;
    int getHeight() const;
};

}  // namespace Allegro

#endif  // SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5WINDOW_HPP_
