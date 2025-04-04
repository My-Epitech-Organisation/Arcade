// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKWindow implementation
*/

#include "GTKUtils/GTKWindow.hpp"
#include <memory>

namespace GTK {

GTKWindow::GTKWindow() : _window(nullptr), _drawingArea(nullptr) {}

void GTKWindow::createWindow(std::shared_ptr<GtkApplication> app, int width,
int height) {
    _window = nonOwningPtr(gtk_application_window_new(app.get()));

    gtk_window_set_title(GTK_WINDOW(_window.get()), "Arcade");
    gtk_window_set_default_size(GTK_WINDOW(_window.get()), width, height);
}

void GTKWindow::setupDrawingArea(int width, int height,
void (*drawCallback)(GtkDrawingArea*, cairo_t*, int, int, gpointer),
gpointer userData) {
    _drawingArea = nonOwningPtr(gtk_drawing_area_new());

    gtk_widget_set_size_request(_drawingArea.get(), width, height);

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(_drawingArea.get()),
        drawCallback, userData, nullptr);

    gtk_window_set_child(GTK_WINDOW(_window.get()), _drawingArea.get());
}

void GTKWindow::showWindow() {
    gtk_widget_set_visible(_window.get(), TRUE);
    gtk_window_present(GTK_WINDOW(_window.get()));
}

std::shared_ptr<GtkWidget> GTKWindow::getWindow() const {
    return _window;
}

std::shared_ptr<GtkWidget> GTKWindow::getDrawingArea() const {
    return _drawingArea;
}

void GTKWindow::resetDrawingArea() {
    _drawingArea.reset();
}

}  // namespace GTK
