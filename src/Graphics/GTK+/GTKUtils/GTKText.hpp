// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKText
*/

#ifndef SRC_GRAPHICS_GTK__GTKUTILS_GTKTEXT_HPP_
    #define SRC_GRAPHICS_GTK__GTKUTILS_GTKTEXT_HPP_
    #include <gtk/gtk.h>
    #include <pango/pango.h>
    #include <memory>
    #include <string>

namespace GTK {

class GTKText {
 public:
    GTKText() = default;
    ~GTKText() = default;

    void drawText(std::shared_ptr<cairo_t> cr, int x, int y,
        const std::string &text, std::shared_ptr<PangoFontDescription> font);

    std::shared_ptr<cairo_surface_t> createTextSurface(
        const std::string &text,
        std::shared_ptr<PangoFontDescription> font,
        int *width, int *height);
};

}  // namespace GTK

#endif  // SRC_GRAPHICS_GTK__GTKUTILS_GTKTEXT_HPP_
