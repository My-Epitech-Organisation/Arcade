// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKText implementation
*/
#include <string>
#include <memory>
#include "GTKUtils/GTKText.hpp"
#include "GTKUtils/GTKWindow.hpp"

namespace GTK {

void GTKText::drawText(std::shared_ptr<cairo_t> cr, int x, int y,
const std::string &text, std::shared_ptr<PangoFontDescription> font) {
    cairo_save(cr.get());

    std::shared_ptr<PangoLayout> layout(
        pango_cairo_create_layout(cr.get()),
        g_object_unref);

    pango_layout_set_text(layout.get(), text.c_str(), -1);

    if (font)
        pango_layout_set_font_description(layout.get(), font.get());

    cairo_set_source_rgb(cr.get(), 1, 1, 1);

    cairo_move_to(cr.get(), x, y);
    pango_cairo_show_layout(cr.get(), layout.get());

    cairo_restore(cr.get());
}

std::shared_ptr<cairo_surface_t> GTKText::createTextSurface(
const std::string &text, std::shared_ptr<PangoFontDescription> font,
int *width, int *height) {
    auto tempSurface = std::shared_ptr<cairo_surface_t>(
        cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1),
        cairo_surface_destroy);
    auto tempCr = std::shared_ptr<cairo_t>(
        cairo_create(tempSurface.get()),
        cairo_destroy);
    std::shared_ptr<PangoLayout> layout(
        pango_cairo_create_layout(tempCr.get()),
        g_object_unref);

    pango_layout_set_text(layout.get(), text.c_str(), -1);

    if (font)
        pango_layout_set_font_description(layout.get(), font.get());

    int w, h;
    pango_layout_get_pixel_size(layout.get(), &w, &h);

    if (width) *width = w;
    if (height) *height = h;

    auto surface = std::shared_ptr<cairo_surface_t>(
        cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h),
        cairo_surface_destroy);

    auto cr = std::shared_ptr<cairo_t>(
        cairo_create(surface.get()),
        cairo_destroy);

    cairo_set_source_rgba(cr.get(), 0, 0, 0, 0);
    cairo_paint(cr.get());

    cairo_set_source_rgb(cr.get(), 1, 1, 1);
    std::shared_ptr<PangoLayout> finalLayout(
        pango_cairo_create_layout(cr.get()),
        g_object_unref);

    pango_layout_set_text(finalLayout.get(), text.c_str(), -1);

    if (font)
        pango_layout_set_font_description(finalLayout.get(), font.get());

    pango_cairo_show_layout(cr.get(), finalLayout.get());

    return surface;
}

}  // namespace GTK
