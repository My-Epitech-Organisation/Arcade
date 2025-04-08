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
    #include "Shared/Interface/Core/IMenu.hpp"
    #include "Core/Score/ScoreManager.hpp"
    #include "Shared/Interface/Core/IGameState.hpp"
    #include "Shared/Interface/Core/IWindowModule.hpp"
    #include "Shared/Models/ColorType.hpp"

namespace Arcade {

class Menu : public IMenu {
 public:
    enum MenuState {
        MAIN_MENU,
        GAME_SELECTION,
        GRAPHICS_SELECTION
    };

    /**
     * @brief Constructs a Menu object.
     * @param window A shared pointer to the Window object used for rendering.
     */
    Menu(std::shared_ptr<IWindowModule> window,
        std::shared_ptr<ScoreManager> scoreManager = nullptr);

    /**
     * @brief Destroys the Menu object.
     */
    ~Menu() override;

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
                      size_t selectedGame) override;

    void displayGameSelection(const std::vector<std::string> &gameLibs,
                           size_t selectedGame) override;

    void displayGraphicsSelection(const std::vector<std::string> &graphicsLibs,
                           size_t selectedGraphics) override;

    void setWindow(std::shared_ptr<IWindowModule> window) override;
    void setScoreManager(std::shared_ptr<ScoreManager> scoreManager) override;

    /**
     * @brief Displays the name input screen.
     * @param currentInput The current input text.
     */
    void displayNameInput(const std::string &currentInput);

 private:
    std::shared_ptr<IWindowModule> _window;
    std::shared_ptr<ScoreManager> _scoreManager;

    void displayTitle(const std::string &title);
    void displayMenuOption(const std::string &option,
        int x, int y, Color color);
    void displayStatus(const std::string &label,
        const std::string &value, int y, Color color);
    void displaySelectionMenu(const std::string &title,
        const std::vector<std::string> &options,
        size_t selectedOption, bool showScores);

    static constexpr int TITLE_Y = 50;
    static constexpr int MENU_START_Y = 100;
    static constexpr int MENU_ITEM_HEIGHT = 20;
    static constexpr int STATUS_OFFSET_Y = 40;
};

}  // namespace Arcade

#endif  // SRC_CORE_MENU_MENU_HPP_
