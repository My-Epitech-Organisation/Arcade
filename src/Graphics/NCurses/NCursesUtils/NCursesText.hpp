// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesText
*/

#ifndef SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESTEXT_HPP_
    #define SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESTEXT_HPP_
    #include <ncurses.h>
    #include <string>
    #include <memory>

namespace NCurses {
class NCursesText {
 public:
    NCursesText() = default;
    void drawText(WINDOW* window, const std::string &text, int x, int y);
    void drawTextWithColor(WINDOW* window, const std::string &text,
                          int x, int y, int colorPair);

 private:
};
}  // namespace NCurses

#endif  // SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESTEXT_HPP_
