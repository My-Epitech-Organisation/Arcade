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

void GTKModule::on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height,
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
    auto self = GTK::nonOwningPtr(static_cast<GTKModule*>(user_data));
    auto areaPtr = GTK::nonOwningPtr(area);
    auto crPtr = GTK::nonOwningPtr(cr);
    self->draw(areaPtr, crPtr, width, height);
}

void GTKModule::draw(std::shared_ptr<GtkDrawingArea> area,
    std::shared_ptr<cairo_t> cr, int width, int height) {
    (void)area;
    (void)width;
    (void)height;

    auto surface = _renderer.getSurface();
    if (surface) {
        cairo_set_source_surface(cr.get(), surface.get(), 0, 0);
        cairo_paint(cr.get());
    }
}

void GTKModule::on_activate(GtkApplication *app, gpointer user_data) {
    //? These are raw pointer parameters from a C callback - unavoidable
    //? We immediately wrap them in smart pointers when entering C++ code
    //? Function signature must match what GTK expects (C function pointer)
    (void)app;

    auto appPtr = GTK::nonOwningPtr(app);
    //? This cast is unavoidable - we need to convert the
    //? generic C void pointer (gpointer)
    //? back to our C++ object type. GTK has no type safety
    //? for user_data and always passes it as a void pointer.
    //? We immediately wrap it in a non-owning smart pointer to
    //? prevent raw pointer usage in the rest of the code.
    auto self = GTK::nonOwningPtr(static_cast<GTKModule*>(user_data));
    self->_window.createWindow(self->_app, self->_windowWidth, self->_windowHeight);
    self->_renderer.createRenderer(self->_windowWidth, self->_windowHeight);

    if (!self->_fontManager.hasFonts())
        self->_fontManager.loadFonts();

    self->clearScreen();
}
