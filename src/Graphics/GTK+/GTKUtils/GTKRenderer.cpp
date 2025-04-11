// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKRenderer implementation
*/
#include <memory>
#include "GTKUtils/GTKRenderer.hpp"
#include "GTKUtils/GTKWindow.hpp"

namespace GTK {

GTKRenderer::GTKRenderer() : _surface(nullptr), _cr(nullptr) {}

void GTKRenderer::createRenderer(int width, int height) {
    if (_surface || _cr) {
        _surface.reset();
        _cr.reset();
    }

    try {
        _surface = std::shared_ptr<cairo_surface_t>(
            cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height),
            cairo_surface_destroy);

        if (cairo_surface_status(_surface.get()) != CAIRO_STATUS_SUCCESS) {
            _surface.reset();
            return;
        }

        _cr = std::shared_ptr<cairo_t>(
            cairo_create(_surface.get()),
            cairo_destroy);

        if (cairo_status(_cr.get()) != CAIRO_STATUS_SUCCESS) {
            _cr.reset();
            _surface.reset();
            return;
        }

        cairo_set_source_rgb(_cr.get(), 0, 0, 0);
        cairo_paint(_cr.get());
    } catch (const std::exception& e) {
        _surface.reset();
        _cr.reset();
    }
}

void GTKRenderer::clearScreen() {
    cairo_set_source_rgb(_cr.get(), 0, 0, 0);
    cairo_paint(_cr.get());
}

void GTKRenderer::present() {
    cairo_surface_flush(_surface.get());
}

std::shared_ptr<cairo_t> GTKRenderer::getRenderer() const {
    return _cr;
}

std::shared_ptr<cairo_surface_t> GTKRenderer::getSurface() const {
    return _surface;
}

void GTKRenderer::drawSurfaceToRenderer(std::shared_ptr<cairo_surface_t>
surface, int x, int y) {
    cairo_save(_cr.get());
    cairo_set_source_surface(_cr.get(), surface.get(), x, y);
    cairo_paint(_cr.get());
    cairo_restore(_cr.get());
}

}  // namespace GTK
