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
    if (!_window) return;

    auto drawingArea = gtk_drawing_area_new();
    _drawingArea = std::shared_ptr<GtkWidget>(
        GTK_WIDGET(drawingArea),
        g_object_unref);

    GtkWidget *widget = _drawingArea.get();
    gtk_widget_set_can_target(widget, FALSE);

    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(widget), width);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(widget), height);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(widget),
        drawCallback, userData, nullptr);

    gtk_window_set_child(GTK_WINDOW(_window.get()), widget);
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
