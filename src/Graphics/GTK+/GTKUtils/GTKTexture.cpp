// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKTexture implementation
*/
#include <cstdio>
#include <string>
#include <iostream>
#include <memory>
#include "GTKUtils/GTKTexture.hpp"
#include "GTKUtils/GTKWindow.hpp"

namespace GTK {

std::shared_ptr<cairo_surface_t> GTKTexture::loadTexture(const std::string&
texturePath) {
    FILE* file = fopen(texturePath.c_str(), "r");
    if (!file)
        return nullptr;
    fclose(file);

    GError* tempError = nullptr;

    std::shared_ptr<GdkPixbuf> pixbuf(
        gdk_pixbuf_new_from_file(texturePath.c_str(), &tempError),
        g_object_unref);

    std::unique_ptr<GError, decltype(&g_error_free)> error(tempError,
        g_error_free);

    if (!pixbuf || error) {
        if (error)
            std::cerr << "Error loading texture: " <<
                error->message << std::endl;
        return nullptr;
    }

    int width = gdk_pixbuf_get_width(pixbuf.get());
    int height = gdk_pixbuf_get_height(pixbuf.get());

    auto surface = std::shared_ptr<cairo_surface_t>(
        cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height),
        cairo_surface_destroy);

    auto cr = std::shared_ptr<cairo_t>(
        cairo_create(surface.get()),
        cairo_destroy);

    gdk_cairo_set_source_pixbuf(cr.get(), pixbuf.get(), 0, 0);
    cairo_paint(cr.get());

    return surface;
}

void GTKTexture::renderTexture(std::shared_ptr<cairo_t> cr,
std::shared_ptr<cairo_surface_t> texture, int x, int y) {
    cairo_save(cr.get());
    cairo_set_source_surface(cr.get(), texture.get(), x, y);
    cairo_paint(cr.get());
    cairo_restore(cr.get());
}

void GTKTexture::renderTextureScaled(std::shared_ptr<cairo_t> cr,
std::shared_ptr<cairo_surface_t> texture, int x, int y, int w, int h) {
    int origWidth = cairo_image_surface_get_width(texture.get());
    int origHeight = cairo_image_surface_get_height(texture.get());

    cairo_save(cr.get());
    cairo_translate(cr.get(), x, y);
    cairo_scale(cr.get(), static_cast<double>(w) / origWidth,
        static_cast<double>(h) / origHeight);
    cairo_set_source_surface(cr.get(), texture.get(), 0, 0);
    cairo_pattern_set_filter(cairo_get_source(cr.get()), CAIRO_FILTER_NEAREST);
    cairo_paint(cr.get());
    cairo_restore(cr.get());
}

}  // namespace GTK
