// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKRenderer
*/

#ifndef SRC_GRAPHICS_GTK__GTKUTILS_GTKRENDERER_HPP_
    #define SRC_GRAPHICS_GTK__GTKUTILS_GTKRENDERER_HPP_
    #include <gtk/gtk.h>
    #include <cairo/cairo.h>
    #include <memory>

namespace GTK {

class GTKRenderer {
 public:
    GTKRenderer();
    ~GTKRenderer() = default;

    void createRenderer(int width, int height);
    void clearScreen();
    void present();

    std::shared_ptr<cairo_t> getRenderer() const;
    std::shared_ptr<cairo_surface_t> getSurface() const;

    void drawSurfaceToRenderer(std::shared_ptr<cairo_surface_t> surface,
        int x, int y);

 private:
    std::shared_ptr<cairo_surface_t> _surface;
    std::shared_ptr<cairo_t> _cr;
};

}  // namespace GTK

#endif  // SRC_GRAPHICS_GTK__GTKUTILS_GTKRENDERER_HPP_
