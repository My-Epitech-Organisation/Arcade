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
#include <memory>
#include "Core/Menu/Menu.hpp"
#include "Core/Window/Window.hpp"
#include "Shared/Interface/Display/IDisplayModule.hpp"
#include "Shared/Models/ColorType.hpp"
#include "Shared/Models/ModuleInfos.hpp"

namespace Arcade {

Menu::Menu(std::shared_ptr<IWindowModule> window,
std::shared_ptr<ScoreManager> scoreManager)
: _window(window), _scoreManager(scoreManager) {}

Menu::~Menu() { }

void Menu::displayTitle(const std::string &title) {
    auto window = std::dynamic_pointer_cast<Window>(_window);
    if (!window) {
        throw std::runtime_error("Window cast failed");
    }
    int centerX = window->getWidth() / 2;
    // Create DrawableComponent for the title
    DrawableComponent titleText;
    titleText.setAsText(title, "./assets/fonts/Arial.ttf", 32.0f);
    titleText.posX = centerX - (title.length() * 8);
    titleText.posY = 80;
    titleText.color = Color::YELLOW;
    titleText.isVisible = true;
    window->drawDrawable(titleText);
}

void Menu::displayMenuOption(const std::string &option, int x,
int y, Color color, bool isSelected) {
    auto window = std::dynamic_pointer_cast<Window>(_window);
    if (!window) {
        throw std::runtime_error("Window cast failed");
    }
    // Format the option text and create a DrawableComponent
    std::string displayText = isSelected ? "> " + option : option;
    DrawableComponent optionText;
    optionText.setAsText(displayText, "./assets/fonts/Arial.ttf", 20.0f);
    optionText.posX = x;
    optionText.posY = y;
    optionText.color = color;
    optionText.isVisible = true;
    window->drawDrawable(optionText);
}

void Menu::displayStatus(const std::string &label,
const std::string &value, int y, Color color) {
    auto window = std::dynamic_pointer_cast<Window>(_window);
    if (!window) {
        throw std::runtime_error("Window cast failed");
    }
    // Create DrawableComponent for the status text
    std::string statusText = label + ": "
        + (value.empty() ? "None" : value);
    DrawableComponent statusComponent;
    statusComponent.setAsText(statusText,
        "./assets/fonts/Arial.ttf", 16.0f);
    statusComponent.posX = 10;  // Left-aligned position
    statusComponent.posY = y;
    statusComponent.color = color;
    statusComponent.isVisible = true;
    window->drawDrawable(statusComponent);
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
        MENU_START_Y, Color::WHITE, false);
    displayMenuOption("2. Select Graphics Library",
        centerX - 40, MENU_START_Y + MENU_ITEM_HEIGHT, Color::WHITE,
        false);
    displayMenuOption("3. Select Game", centerX - 40,
        MENU_START_Y + 2 * MENU_ITEM_HEIGHT, Color::WHITE,
        false);
    displayMenuOption("4. Exit", centerX - 40,
        MENU_START_Y + 3 * MENU_ITEM_HEIGHT, Color::WHITE,
        false);
    displayMenuOption("5. Enter Player Name", centerX - 40,
        MENU_START_Y + 4 * MENU_ITEM_HEIGHT, Color::WHITE,
        false);
    displayStatus("Current Graphics",
        graphicsLibs.empty() ? "" : graphicsLibs[selectedGraphics],
        height - 2 * STATUS_OFFSET_Y, Color::GREEN);
    displayStatus("Current Game",
        gameLibs.empty() ? "" : gameLibs[selectedGame],
        height - STATUS_OFFSET_Y, Color::GREEN);
    if (_scoreManager) {
        std::string playerName = _scoreManager->getCurrentPlayerName();
        displayStatus("Player",
            playerName.empty() ? "Not Set" : playerName,
            height - 3 * STATUS_OFFSET_Y, Color::GREEN);
    }
}

void Menu::displaySelectionMenu(const std::string &title,
const std::vector<std::string> &options,
size_t selectedOption,
bool showScores) {
    auto window = std::dynamic_pointer_cast<Window>(_window);
    if (!window) {
        throw std::runtime_error("Window cast failed");
    }
    int centerX = window->getWidth() / 2;

    displayTitle(title);

    int yPos = MENU_START_Y;
    for (size_t i = 0; i < options.size(); i++) {
        Color color = (i == selectedOption) ? Color::GREEN : Color::WHITE;
        std::string displayText = options[i];

        if (showScores && _scoreManager) {
            auto highScore = _scoreManager->getHighScore(options[i]);
            displayText += "  -  High: " + highScore.first + " (" +
                std::to_string(highScore.second) + ")";
        }

        displayMenuOption(displayText, centerX - 150, yPos, color,
            i == selectedOption);
        yPos += MENU_ITEM_HEIGHT;
    }

    displayMenuOption("Press ENTER to select, ESC to go back",
        centerX - 150, yPos + 40, Color::WHITE, false);
}

void Menu::displayGameSelection(
const std::vector<std::string> &gameLibs, size_t selectedGame) {
    displaySelectionMenu("SELECT GAME", gameLibs, selectedGame, true);
}

void Menu::displayGraphicsSelection(
const std::vector<std::string> &graphicsLibs, size_t selectedGraphics) {
    displaySelectionMenu("SELECT GRAPHICS LIBRARY",
        graphicsLibs, selectedGraphics, false);
}

void Menu::setWindow(std::shared_ptr<IWindowModule> window) {
    _window = window;
    if (!window) {
        throw std::runtime_error("Invalid window");
    }
}
void Menu::displayNameInput(const std::string &currentInput) {
    auto window = std::dynamic_pointer_cast<Window>(_window);
    if (!window) {
        throw std::runtime_error("Window cast failed");
    }
    int centerX = window->getWidth() / 2;
    int centerY = window->getHeight() / 2;

    displayTitle("ENTER YOUR NAME");

    // Create DrawableComponent for the user input
    std::string displayName = currentInput + "_";
    DrawableComponent nameInput;
    nameInput.setAsText(displayName, "./assets/fonts/Arial.ttf", 24.0f);
    nameInput.posX = centerX - (displayName.length() * 5);
    nameInput.posY = centerY;
    nameInput.color = Color::GREEN;
    nameInput.isVisible = true;

    // Create DrawableComponent for instructions
    DrawableComponent instructions;
    instructions.setAsText("Press ENTER to confirm, ESC to cancel",
                          "./assets/fonts/Arial.ttf", 18.0f);
    instructions.posX = centerX - 150;
    instructions.posY = centerY + 60;
    instructions.color = Color::WHITE;
    instructions.isVisible = true;
    // Draw using the new component-based approach
    window->drawDrawable(nameInput);
    window->drawDrawable(instructions);
}

void Menu::setScoreManager(std::shared_ptr<ScoreManager> scoreManager) {
    _scoreManager = scoreManager;
    if (!scoreManager)
        throw std::runtime_error("Invalid window");
}

}  // namespace Arcade

extern "C" {
    Arcade::IMenu* entryPoint() {
        return new Arcade::Menu(nullptr, nullptr);
    }

    Arcade::ModuleInfos module_infos() {
        return {"Menu", "IDK",
            "IDK",
            "./lib/arcade_menu.so", Arcade::ModuleType::GAME_COMPONENT};
    }
}
