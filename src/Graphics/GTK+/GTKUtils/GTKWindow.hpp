// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKWindow
*/

#ifndef SRC_GRAPHICS_GTK__GTKUTILS_GTKWINDOW_HPP_
    #define SRC_GRAPHICS_GTK__GTKUTILS_GTKWINDOW_HPP_
    #include <gtk/gtk.h>
    #include <memory>
    #include <string>

namespace GTK {

template<typename T>
std::shared_ptr<T> nonOwningPtr(T* ptr) {
    return std::shared_ptr<T>(ptr, [](T*){});
}

class GTKWindow {
 public:
    GTKWindow();
    ~GTKWindow() = default;

    void createWindow(std::shared_ptr<GtkApplication> app, int width,
        int height);
    void setupDrawingArea(int width, int height,
        void (*drawCallback)(GtkDrawingArea*, cairo_t*, int, int, gpointer),
        gpointer userData);
    void showWindow();
    void resetDrawingArea();

    std::shared_ptr<GtkWidget> getWindow() const;
    std::shared_ptr<GtkWidget> getDrawingArea() const;

 private:
    std::shared_ptr<GtkWidget> _window;
    std::shared_ptr<GtkWidget> _drawingArea;
};

}  // namespace GTK

#endif  // SRC_GRAPHICS_GTK__GTKUTILS_GTKWINDOW_HPP_
