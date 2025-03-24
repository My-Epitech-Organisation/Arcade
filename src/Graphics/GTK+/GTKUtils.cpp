// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKUtils
*/
#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdexcept>
#include <memory>
#include <cstdio>
#include <string>
#include <iostream>
#include "GTK+/GTK.hpp"

template<typename T>
std::shared_ptr<T> nonOwningPtr(T* ptr) {
    //? This function receives a raw pointer
    //? but immediately wraps it in a smart pointer
    //? Cannot be avoided because we're receiving
    //? pointers from GTK C API functions
    return std::shared_ptr<T>(ptr, [](T*){});
}

void GTK::createWindow(int width, int height) {
    _window = nonOwningPtr(gtk_application_window_new(_app.get()));

    gtk_window_set_title(GTK_WINDOW(_window.get()), "Arcade");
    gtk_window_set_default_size(GTK_WINDOW(_window.get()), width, height);

    setupDrawingArea();

    gtk_widget_set_visible(_window.get(), TRUE);
    gtk_window_present(GTK_WINDOW(_window.get()));
}

void GTK::setupDrawingArea() {
    _drawingArea = nonOwningPtr(gtk_drawing_area_new());

    gtk_widget_set_size_request(_drawingArea.get(), _windowWidth,
        _windowHeight);

    //? 'this' pointer is unavoidable as GTK expects a raw pointer for user_data
    //? Using a smart pointer here would be useless
    //? as GTK doesn't manage C++ objects
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(_drawingArea.get()),
        on_draw, this, nullptr);

    gtk_window_set_child(GTK_WINDOW(_window.get()), _drawingArea.get());
}

void GTK::on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height,
    gpointer user_data) {
    //? These are raw pointer parameters from a C callback - unavoidable
    //? We immediately wrap them in smart pointers when entering C++ code
    //? Function signature must match what GTK expects (C function pointer)

    //? This cast is unavoidable - we need to convert the
    //? generic C void pointer (gpointer)
    //? back to our C++ object type. GTK has no type safety
    //? for user_data and always passes it as a void pointer.
    //? We immediately wrap it in a non-owning smart pointer to
    //? prevent raw pointer usage in the rest of the code.
    auto self = nonOwningPtr(static_cast<GTK*>(user_data));
    auto areaPtr = nonOwningPtr(area);
    auto crPtr = nonOwningPtr(cr);
    self->draw(areaPtr, crPtr, width, height);
}

void GTK::draw(std::shared_ptr<GtkDrawingArea> area,
    std::shared_ptr<cairo_t> cr, int width, int height) {
    (void)area;
    (void)width;
    (void)height;

    if (_surface) {
        cairo_set_source_surface(cr.get(), _surface.get(), 0, 0);
        cairo_paint(cr.get());
    }
}

void GTK::drawTextInternal(std::shared_ptr<cairo_t> cr, int x, int y,
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

void GTK::on_activate(GtkApplication *app, gpointer user_data) {
    //? These are raw pointer parameters from a C callback - unavoidable
    //? We immediately wrap them in smart pointers when entering C++ code
    //? Function signature must match what GTK expects (C function pointer)
    (void)app;

    auto appPtr = nonOwningPtr(app);
    //? This cast is unavoidable - we need to convert the
    //? generic C void pointer (gpointer)
    //? back to our C++ object type. GTK has no type safety
    //? for user_data and always passes it as a void pointer.
    //? We immediately wrap it in a non-owning smart pointer to
    //? prevent raw pointer usage in the rest of the code.
    auto self = nonOwningPtr(static_cast<GTK*>(user_data));
    self->createWindow(self->_windowWidth, self->_windowHeight);

    self->_surface = std::shared_ptr<cairo_surface_t>(
        cairo_image_surface_create(
            CAIRO_FORMAT_ARGB32,
            self->_windowWidth,
            self->_windowHeight),
        cairo_surface_destroy);

    self->_cr = std::shared_ptr<cairo_t>(
        cairo_create(self->_surface.get()),
        cairo_destroy);

    if (self->_fonts.empty())
        self->loadFonts();

    self->clearScreen();
}

std::shared_ptr<cairo_surface_t>
    GTK::loadTexture(const std::string& texturePath) {
    FILE* file = fopen(texturePath.c_str(), "r");
    if (!file)
        return nullptr;
    fclose(file);

    //? GError** is a special case: many GTK functions take a GError** parameter
    //? which is used as an out-parameter to report errors
    //? Cannot be directly wrapped in a smart pointer because the function needs
    //? a pointer-to-pointer, not the pointer itself
    GError* tempError = nullptr;

    std::shared_ptr<GdkPixbuf> pixbuf(
        gdk_pixbuf_new_from_file(texturePath.c_str(), &tempError),
        g_object_unref);

    std::unique_ptr<GError, decltype(&g_error_free)> error(tempError,
        g_error_free);

    if (!pixbuf || error) {
        if (error)
            std::cerr << "Error loading texture: "
                << error->message << std::endl;
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

std::shared_ptr<PangoFontDescription>
    GTK::loadFont(const std::string& fontPath) {
    (void)fontPath;
    return std::shared_ptr<PangoFontDescription>(
        pango_font_description_from_string("Sans 12"),
        pango_font_description_free);
}

void GTK::loadFonts() {
    _fonts["default"] = std::shared_ptr<PangoFontDescription>(
        pango_font_description_from_string("Sans 12"),
        pango_font_description_free);

    _fonts["monospace"] = std::shared_ptr<PangoFontDescription>(
        pango_font_description_from_string("Monospace 12"),
        pango_font_description_free);

    _fonts["title"] = std::shared_ptr<PangoFontDescription>(
        pango_font_description_from_string("Sans Bold 18"),
        pango_font_description_free);
}

void GTK::handleEvents() {
    while (g_main_context_iteration(nullptr, FALSE)) {}
}
