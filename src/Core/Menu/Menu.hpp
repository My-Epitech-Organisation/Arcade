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

/**
 * @file Menu.hpp
 * @brief Declaration of the Menu class for the Arcade project.
 */

namespace Arcade {

/**
 * @class Menu
 * @brief Represents the menu system for the Arcade application.
 *
 * The Menu class is responsible for displaying and managing the main menu,
 * game selection menu, and graphics library selection menu. It interacts
 * with a Window object to render the menus and handles user input for
 * selecting options.
 */
class Menu {
 public:
    /**
     * @enum MenuState
     * @brief Represents the different states of the menu.
     */
    enum MenuState {
        MAIN_MENU,          /** The main menu state. */
        GAME_SELECTION,     /** The game selection menu state. */
        GRAPHICS_SELECTION  /** The graphics library selection menu state. */
    };

    /**
     * @brief Constructs a Menu object.
     * @param window A shared pointer to the Window object used for rendering.
     */
    explicit Menu(std::shared_ptr<Window> window);

    /**
     * @brief Destroys the Menu object.
     */
    ~Menu();

    /**
     * @brief Displays the main menu.
     * @param graphicsLibs A vector of available graphics libraries.
     * @param gameLibs A vector of available game libraries.
     * @param selectedGraphics The index of the currently selected graphics library.
     * @param selectedGame The index of the currently selected game library.
     */
    void displayMainMenu(const std::vector<std::string> &graphicsLibs,
                         const std::vector<std::string> &gameLibs,
                         size_t selectedGraphics,
                         size_t selectedGame);

    /**
     * @brief Displays the game selection menu.
     * @param gameLibs A vector of available game libraries.
     * @param selectedGame The index of the currently selected game library.
     */
    void displayGameSelection(const std::vector<std::string> &gameLibs,
                              size_t selectedGame);

    /**
     * @brief Displays the graphics library selection menu.
     * @param graphicsLibs A vector of available graphics libraries.
     * @param selectedGraphics The index of the currently selected graphics library.
     */
    void displayGraphicsSelection(const std::vector<std::string> &graphicsLibs,
                                  size_t selectedGraphics);

    /**
     * @brief Sets the Window object used for rendering.
     * @param window A shared pointer to the new Window object.
     */
    void setWindow(std::shared_ptr<Window> window);

 private:
    std::shared_ptr<Window> _window;
        /** Shared pointer to the Window object used for rendering. */
    void displayTitle(const std::string &title);
    void displayMenuOption(const std::string &option,
        int x, int y, Color color);
    void displayStatus(const std::string &label,
        const std::string &value, int y, Color color);
    void displaySelectionMenu(const std::string &title,
        const std::vector<std::string> &options,
        size_t selectedOption);
    static constexpr int TITLE_Y = 50;
        /** Y-coordinate for the title. */
    static constexpr int MENU_START_Y = 100;
        /** Y-coordinate where the menu starts. */
    static constexpr int MENU_ITEM_HEIGHT = 20;
        /** Height of each menu item. */
    static constexpr int STATUS_OFFSET_Y = 40;
        /** Offset for the status display. */
};

}  // namespace Arcade

#endif  // SRC_CORE_MENU_MENU_HPP_
