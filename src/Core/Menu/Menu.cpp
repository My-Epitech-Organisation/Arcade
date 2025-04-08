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
#include "Core/Menu/Menu.hpp"
#include "Core/Window/Window.hpp"
#include "Shared/Interface/Display/IDisplayModule.hpp"
#include "Shared/Models/ColorType.hpp"

namespace Arcade {

// Update constructor to match the declaration in header
Menu::Menu(std::shared_ptr<IWindowModule> window) : _window(window) { }

Menu::~Menu() { }

void Menu::displayTitle(const std::string &title) {
    auto window = std::dynamic_pointer_cast<Window>(_window);
    if (!window) {
        throw std::runtime_error("Window cast failed");
    }
    int centerX = window->getWidth() / 2;
    window->drawText(title,
        centerX - (title.size() * 5), TITLE_Y, Color::WHITE);
}

void Menu::displayMenuOption(const std::string &option,
int x, int y, Color color) {
    auto window = std::dynamic_pointer_cast<Window>(_window);
    if (!window) {
        throw std::runtime_error("Window cast failed");
    }
    window->drawText(option, x, y, color);
}

void Menu::displayStatus(const std::string &label,
const std::string &value, int y, Color color) {
    auto window = std::dynamic_pointer_cast<Window>(_window);
    if (!window) {
        throw std::runtime_error("Window cast failed");
    }
    int width = window->getWidth();
    window->drawText(label + ": " +
        (value.empty() ? "None" : value), 10, y, color);
}

void Menu::displayMainMenu(const std::vector<std::string> &graphicsLibs,
const std::vector<std::string> &gameLibs,
size_t selectedGraphics,
size_t selectedGame) {
    auto window = std::dynamic_pointer_cast<Window>(_window);
    if (!window) {
        throw std::runtime_error("Window cast failed");
    }
    int centerX = window->getWidth() / 2;
    int height = window->getHeight();

    displayTitle("ARCADE");

    displayMenuOption("1. Play Game", centerX - 40,
        MENU_START_Y, Color::WHITE);
    displayMenuOption("2. Select Graphics Library",
        centerX - 40, MENU_START_Y + MENU_ITEM_HEIGHT, Color::WHITE);
    displayMenuOption("3. Select Game", centerX - 40,
        MENU_START_Y + 2 * MENU_ITEM_HEIGHT, Color::WHITE);
    displayMenuOption("4. Exit", centerX - 40,
        MENU_START_Y + 3 * MENU_ITEM_HEIGHT, Color::WHITE);
    displayStatus("Current Graphics",
        graphicsLibs.empty() ? "" : graphicsLibs[selectedGraphics],
        height - 2 * STATUS_OFFSET_Y, Color::GREEN);
    displayStatus("Current Game",
        gameLibs.empty() ? "" : gameLibs[selectedGame],
        height - STATUS_OFFSET_Y, Color::GREEN);
}

void Menu::displaySelectionMenu(const std::string &title,
const std::vector<std::string> &options,
size_t selectedOption) {
    auto window = std::dynamic_pointer_cast<Window>(_window);
    if (!window) {
        throw std::runtime_error("Window cast failed");
    }
    int centerX = window->getWidth() / 2;

    displayTitle(title);

    int yPos = MENU_START_Y;
    for (size_t i = 0; i < options.size(); i++) {
        Color color = (i == selectedOption) ? Color::GREEN : Color::WHITE;
        displayMenuOption(options[i], centerX - 100, yPos, color);
        yPos += MENU_ITEM_HEIGHT;
    }

    displayMenuOption("Press ENTER to select, ESC to go back",
        centerX - 100, yPos + 40, Color::WHITE);
}

void Menu::displayGameSelection(
const std::vector<std::string> &gameLibs, size_t selectedGame) {
    displaySelectionMenu("SELECT GAME", gameLibs, selectedGame);
}

void Menu::displayGraphicsSelection(
const std::vector<std::string> &graphicsLibs, size_t selectedGraphics) {
    displaySelectionMenu("SELECT GRAPHICS LIBRARY",
        graphicsLibs, selectedGraphics);
}

void Menu::setWindow(std::shared_ptr<IWindowModule> window) {
    _window = window;
    if (!window) {
        throw std::runtime_error("Invalid window");
    }
}

}  // namespace Arcade

extern "C" {
    Arcade::IMenu* entryPoint() {
        return new Arcade::Menu(nullptr);
    }
}
