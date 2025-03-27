// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Menu
*/

#include <iostream>
#include <vector>
#include <string>
#include "Menu/Menu.hpp"
#include "Interface/Display/IDisplayModule.hpp"
#include "Models/ColorType.hpp"

namespace Arcade {

Menu::Menu(std::shared_ptr<Window> window) : _window(window) { }

Menu::~Menu() { }

void Menu::displayMainMenu(const std::vector<std::string> &graphicsLibs,
const std::vector<std::string> &gameLibs,
size_t selectedGraphics,
size_t selectedGame) {
    int centerX = _window->getWidth() / 2;
    int width = _window->getWidth();
    int height = _window->getHeight();
    _window->drawText("ARCADE", centerX - 30, TITLE_Y, Color::WHITE);
    _window->drawText("1. Play Game", centerX - 40, MENU_START_Y, Color::WHITE);
    _window->drawText("2. Select Graphics Library", centerX - 40,
        MENU_START_Y + MENU_ITEM_HEIGHT, Color::WHITE);
    _window->drawText("3. Select Game", centerX - 40,
        MENU_START_Y + 2 * MENU_ITEM_HEIGHT, Color::WHITE);
    _window->drawText("4. Exit", centerX - 40,
        MENU_START_Y + 3 * MENU_ITEM_HEIGHT, Color::WHITE);
    _window->drawText("Current Graphics: " +
                  (graphicsLibs.empty() ? "None"
                  : graphicsLibs[selectedGraphics]),
                  10, height - 2 * STATUS_OFFSET_Y, Color::GREEN);
    _window->drawText("Current Game: " +
                  (gameLibs.empty() ? "None" : gameLibs[selectedGame]),
                  10, height - STATUS_OFFSET_Y, Color::GREEN);
}

void Menu::displayGameSelection(const std::vector<std::string> &gameLibs,
size_t selectedGame) {
    int centerX = _window->getWidth() / 2;
    _window->drawText("SELECT GAME", centerX - 30, TITLE_Y, Color::WHITE);
    int yPos = MENU_START_Y;
    for (size_t i = 0; i < gameLibs.size(); i++) {
        Color color = (i == selectedGame) ? Color::GREEN : Color::WHITE;
        _window->drawText(gameLibs[i], centerX - 100, yPos, color);
        yPos += MENU_ITEM_HEIGHT;
    }
    _window->drawText("Press ENTER to select, ESC to go back",
        centerX - 100, yPos + 40, Color::WHITE);
}

void Menu::displayGraphicsSelection(
const std::vector<std::string> &graphicsLibs, size_t selectedGraphics) {
    int centerX = _window->getWidth() / 2;
    _window->drawText("SELECT GRAPHICS LIBRARY",
        centerX - 60, TITLE_Y, Color::WHITE);
    int yPos = MENU_START_Y;
    for (size_t i = 0; i < graphicsLibs.size(); i++) {
        Color color = (i == selectedGraphics) ? Color::GREEN : Color::WHITE;
        _window->drawText(graphicsLibs[i], centerX - 100, yPos, color);
        yPos += MENU_ITEM_HEIGHT;
    }
    _window->drawText("Press ENTER to select, ESC to go back",
        centerX - 100, yPos + 40, Color::WHITE);
}

void Menu::setWindow(std::shared_ptr<Window> window) {
    _window = window;
}

}  // namespace Arcade
