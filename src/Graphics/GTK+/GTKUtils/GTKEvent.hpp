// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKEvent
*/

#ifndef SRC_GRAPHICS_GTK__GTKUTILS_GTKEVENT_HPP_
    #define SRC_GRAPHICS_GTK__GTKUTILS_GTKEVENT_HPP_
    #include <gtk/gtk.h>
    #include <memory>
    #include <utility>
    #include <map>

namespace GTK {

class GTKEvent {
 private:
    GtkWidget *_window;
    std::map<int, bool> _keyStates;
    std::map<int, bool> _mouseButtonStates;
    size_t _mouseX;
    size_t _mouseY;
 public:
    explicit GTKEvent(GtkWidget *window = nullptr);
    ~GTKEvent() = default;

    void handleEvents();
    void resetEvents();
    bool isKeyPressed(int keyCode) const;
    bool isMouseButtonPressed(int button) const;
    std::pair<size_t, size_t> getMousePosition() const;
};

}  // namespace GTK

#endif  // SRC_GRAPHICS_GTK__GTKUTILS_GTKEVENT_HPP_
