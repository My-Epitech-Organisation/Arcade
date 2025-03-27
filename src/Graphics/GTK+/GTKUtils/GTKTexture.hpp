// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKTexture
*/

#ifndef SRC_GRAPHICS_GTK__GTKUTILS_GTKTEXTURE_HPP_
    #define SRC_GRAPHICS_GTK__GTKUTILS_GTKTEXTURE_HPP_
    #include <gtk/gtk.h>
    #include <memory>
    #include <string>
    #include <iostream>

namespace GTK {

class GTKTexture {
 public:
    GTKTexture() = default;
    ~GTKTexture() = default;

    std::shared_ptr<cairo_surface_t> loadTexture(const std::string&
        texturePath);
    void renderTexture(std::shared_ptr<cairo_t> cr,
        std::shared_ptr<cairo_surface_t> texture, int x, int y);
    void renderTextureScaled(std::shared_ptr<cairo_t> cr,
        std::shared_ptr<cairo_surface_t> texture, int x, int y, int w, int h);
};

}  // namespace GTK

#endif  // SRC_GRAPHICS_GTK__GTKUTILS_GTKTEXTURE_HPP_
