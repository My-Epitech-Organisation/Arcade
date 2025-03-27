// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GameLoop class
*/

#ifndef SRC_CORE_GAMELOOP_GAMELOOP_HPP_
#define SRC_CORE_GAMELOOP_GAMELOOP_HPP_

#include <vector>
#include <string>
#include <memory>
#include <map>
#include "Interface/Display/IDisplayModule.hpp"
#include "Interface/Game/IGameModule.hpp"
#include "Interface/Core/IStateManager.hpp"
#include "EventManager/AEventManager.hpp"
#include "Window/Window.hpp"
#include "Menu/Menu.hpp"
#include "DLLoader/DLLoader.hpp"

/**
 * @file GameLoop.hpp
 * @brief Declaration of the GameLoop class, which manages the main game loop and state transitions.
 */

namespace Arcade {

/**
 * @class GameLoop
 * @brief Manages the main game loop, state transitions, and library loading for the Arcade application.
 */
class GameLoop : public IStateManager {
 public:
    /**
     * @brief Constructs a GameLoop instance with the specified initial library.
     * @param initialLib The name of the initial library to load.
     */
    explicit GameLoop(const std::string& initialLib);

    /**
     * @brief Destroys the GameLoop instance and releases resources.
     */
    ~GameLoop();

    /**
     * @brief Starts the main game loop.
     */
    void run();

    /**
     * @brief Changes the current game state.
     * @param newState A shared pointer to the new game state.
     */
    void changeState(std::shared_ptr<IGameState> newState) override;

 private:
    /**
     * @enum State
     * @brief Represents the different states of the game loop.
     */
    enum State {
        MAIN_MENU,           /** The main menu state. */
        GAME_PLAYING,        /** The game-playing state. */
        GRAPHICS_SELECTION,  /** The graphics library selection state. */
        GAME_SELECTION       /** The game library selection state. */
    };

    State _state; /** The current state of the game loop. */

    /**
     * @brief Loads the available libraries for graphics and games.
     */
    void loadAvailableLibraries();

    /**
     * @brief Subscribes to general events.
     */
    void subscribeEvents();

    /**
     * @brief Subscribes to navigation-related events.
     */
    void subscribeNavEvents();

    /**
     * @brief Subscribes to mouse-related events.
     */
    void subscribeMouseEvents();

    /**
     * @brief Loads and starts the selected game.
     */
    void loadAndStartGame();

    /**
     * @brief Loads the available graphics libraries.
     */
    void loadGraphicsLibraries();

    std::shared_ptr<AEventManager> _eventManager;
        /** The event manager for handling input events. */
    std::shared_ptr<Window> _window; /** The window instance for rendering. */
    std::unique_ptr<Menu> _menu; /** The menu instance for user interaction. */
    DLLoader<IDisplayModule> _graphicsLoader;
        /** The dynamic loader for graphics libraries. */
    DLLoader<IGameModule> _gameLoader;
        /** The dynamic loader for game libraries. */
    std::shared_ptr<IDisplayModule> _currentGraphics;
        /** The currently loaded graphics library. */
    std::shared_ptr<IGameModule> _currentGame;
        /** The currently loaded game library. */
    std::vector<std::string> _graphicsLibs;
        /** List of available graphics libraries. */
    std::vector<std::string> _gameLibs;
        /** List of available game libraries. */
    size_t _selectedGraphics;
        /** Index of the currently selected graphics library. */
    size_t _selectedGame;
        /** Index of the currently selected game library. */
    const std::string _libDir = "./lib/";
        /** Directory containing the libraries. */
    std::map<std::string, void*> _loadedLibHandles;
        /** Map of loaded library handles. */

    static constexpr int TITLE_Y = 50;
        /** Y-coordinate for the title display. */
    static constexpr int MENU_START_Y = 100;
        /** Y-coordinate for the start of the menu. */
    static constexpr int MENU_ITEM_HEIGHT = 20;
        /** Height of each menu item. */
    static constexpr int STATUS_OFFSET_Y = 40;
        /** Y-offset for status display. */
};

}  // namespace Arcade

#endif  // SRC_CORE_GAMELOOP_GAMELOOP_HPP_
