// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesWindow
*/

#ifndef SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESWINDOW_HPP_
    #define SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESWINDOW_HPP_
    #include <ncurses.h>
    #include <memory>
    #include <string>
    #include <functional>

namespace NCurses {
class NCursesWindow {
 public:
    NCursesWindow() : _window(nullptr) {}
    ~NCursesWindow();
    void createWindow(int width, int height);
    void clearWindow();
    void refreshWindow();
    void closeWindow();
    WINDOW* getWindow() const;
    bool isOpen() const;
    void enableKeypad(bool enable);
    int getChar();
    int getWidth() const;
    int getHeight() const;

 private:
    WINDOW* _window;
    int _windowWidth = 0;
    int _windowHeight = 0;
    bool _isOpen = false;
};
}  // namespace NCurses

#endif  // SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESWINDOW_HPP_
