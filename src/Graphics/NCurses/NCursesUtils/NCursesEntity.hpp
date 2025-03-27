// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesEntity
*/

#ifndef SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESENTITY_HPP_
    #define SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESENTITY_HPP_
    #include <ncurses.h>
    #include <map>

namespace NCurses {
class NCursesEntity {
 public:
    NCursesEntity();
    void drawEntity(WINDOW* window, int x, int y, char symbol, int colorPair);
    void registerEntitySymbol(char symbol, int colorPair);
    int getEntityColor(char symbol) const;

 private:
    std::map<char, int> _entityColors;
};
}  // namespace NCurses

#endif  // SRC_GRAPHICS_NCURSES_NCURSESUTILS_NCURSESENTITY_HPP_
