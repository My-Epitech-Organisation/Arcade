// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCurses
*/

#ifndef SRC_GRAPHICS_NCURSES_NCURSES_HPP_
    #define SRC_GRAPHICS_NCURSES_NCURSES_HPP_
    #include <ncurses.h>
    #include <memory>
    #include <string>
    #include "Interface/Display/IDisplayModule.hpp"

class NCurses : public Arcade::IDisplayModule {
 private:
    std::string _name;
    WINDOW* _window;
    int _windowWidth;
    int _windowHeight;
    bool _running;
    void createWindow(int width, int height);
    void initColors();
    void handleKeyPress(int key);

 public:
    NCurses() : _name("NCurses"), _window(nullptr),
                _windowWidth(0), _windowHeight(0), _running(true) {}
    ~NCurses() override;
    void init(float width = 80.f, float height = 24.f) override;
    void stop() override;
    void clearScreen() override;
    void refreshScreen() override;
    void drawEntity(int x, int y, char symbol) override;
    void drawTexture(int x, int y, const std::string &textureId) override;
    void drawText(int x, int y, const std::string &text) override;
    void pollEvents() override;
    bool isOpen() const override;
    const std::string& getName() const override;
};

#endif  // SRC_GRAPHICS_NCURSES_NCURSES_HPP_
