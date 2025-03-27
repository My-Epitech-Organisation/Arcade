// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Menu
*/

#ifndef SRC_CORE_MENU_MENU_HPP_
#define SRC_CORE_MENU_MENU_HPP_

#include <vector>
#include <string>
#include <memory>
#include "Interface/Core/IGameState.hpp"
#include "Window/Window.hpp"

namespace Arcade {

class Menu {
 public:
    enum MenuState {
        MAIN_MENU,
        GAME_SELECTION,
        GRAPHICS_SELECTION
    };
    explicit Menu(std::shared_ptr<Window> window);
    ~Menu();
    void displayMainMenu(const std::vector<std::string> &graphicsLibs,
                        const std::vector<std::string> &gameLibs,
                        size_t selectedGraphics,
                        size_t selectedGame);
    void displayGameSelection(const std::vector<std::string> &gameLibs,
                            size_t selectedGame);
    void displayGraphicsSelection(const std::vector<std::string> &graphicsLibs,
                                size_t selectedGraphics);
    void setWindow(std::shared_ptr<Window> window);

 private:
    std::shared_ptr<Window> _window;
    static constexpr int TITLE_Y = 50;
    static constexpr int MENU_START_Y = 100;
    static constexpr int MENU_ITEM_HEIGHT = 20;
    static constexpr int STATUS_OFFSET_Y = 40;
};

}  // namespace Arcade

#endif  // SRC_CORE_MENU_MENU_HPP_
