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

namespace GTK {

class GTKEvent {
 public:
    GTKEvent() = default;
    ~GTKEvent() = default;

    void handleEvents();
};

}  // namespace GTK

#endif  // SRC_GRAPHICS_GTK__GTKUTILS_GTKEVENT_HPP_
