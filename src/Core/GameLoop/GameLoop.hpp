// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GameLoop class
*/

#ifndef SRC_CORE_GAMELOOP_GAMELOOP_HPP_
#define SRC_CORE_GAMELOOP_GAMELOOP_HPP_

#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <utility>
#include <algorithm>
#include <thread>
#include <chrono>
#include <map>
#include <cstring>
#include "Shared/Interface/Display/IDisplayModule.hpp"
#include "Shared/Interface/Game/IGameModule.hpp"
#include "Shared/Interface/Core/IStateManager.hpp"
#include "Window/Window.hpp"
#include "Menu/Menu.hpp"
#include "DLLoader/DLLoader.hpp"
#include "Core/Score/ScoreManager.hpp"

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
        NAME_INPUT,          /** New state for name input. */
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
    DIR* openLibraryDirectory();
    void processLibraryEntry(struct dirent* entry);
    bool isIgnoredFile(const std::string& filename);
    bool processLibraryHandle(const std::string& path, void* handle);
    bool processLibraryInstance(const std::string& path,
        void* handle, IArcadeModule* (*entryPoint)());
    void handleEvents(std::shared_ptr<bool> running);
    void handleState();
    void displayMainMenu();
    void displayGameSelection();
    void displayGraphicsSelection();
    void cleanup();
    void updateGame();
    void subscribeNum1Event();
    void subscribeNum2Event();
    void subscribeNum3Event();
    void subscribeNum4Event();
    void subscribeNum5Event();
    void subscribeGKeyEvent();
    void subscribeHKeyEvent();
    void switchGameInGame();
    void subscribeNKeyEvent();
    void subscribePKeyEvent();
    void subscribeUpEvent();
    void subscribeDownEvent();
    void subscribeEnterEvent();
    void subscribeEscEvent();
    void subscribeMouseMoveEvent();
    void subscribeRightClickEvent();
    void handleMouseMoveGameSelection(int x, int y, int centerX);
    void handleMouseMoveGraphicsSelection(int x, int y, int centerX);
    void handleLeftClickMainMenu(int x, int y, int centerX);
    void handleLeftClickGameSelection(int x, int y, int centerX);
    void handleLeftClickGraphicsSelection(int x, int y, int centerX);
    void subscribeLeftClickEvent();
    void loadCommonComponents();
    void displayNameInput();
    void subscribeNameInputEvents();
    void switchGraphicsInGame();

    std::shared_ptr<IEventManager> _eventManager;
        /** The event manager for handling input events. */
    std::shared_ptr<Window> _window; /** The window instance for rendering. */
    std::shared_ptr<ScoreManager> _scoreManager;
    std::shared_ptr<IMenu> _menu; /** The menu instance for user interaction. */
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
    std::vector<std::string> _componentsLibs;
    size_t _selectedGraphics;
        /** Index of the currently selected graphics library. */
    size_t _selectedGame;
        /** Index of the currently selected game library. */
    const std::string _libDir = "./lib/";
        /** Directory containing the libraries. */
    std::map<std::string, void*> _loadedLibHandles;
        /** Map of loaded library handles. **/
    std::vector<std::shared_ptr<IComponent>> _commonComponents;
        /** List of components used in the game. */

    std::shared_ptr<IEntityManager> _entityManager;
        /** The entity manager for managing game entities. */
    std::shared_ptr<IComponentManager> _componentManager;
        /** The component manager for managing game components. */
    DLLoader<IMenu> _menuLoader;
    std::shared_ptr<IScoreProvider> _scoreProvider;

    std::string _inputPlayerName; /** Current input buffer for player name. */


    static constexpr int TITLE_Y = 50;
        /** Y-coordinate for the title display. */
    static constexpr int MENU_START_Y = 100;
        /** Y-coordinate for the start of the menu. */
    static constexpr int MENU_ITEM_HEIGHT = 20;
        /** Height of each menu item. */
    static constexpr int STATUS_OFFSET_Y = 40;
        /** Y-offset for status display. */
    bool _gameSwitch = false;

    // Performance optimization - cache for drawable components
    std::vector<std::shared_ptr<IDrawableComponent>> _cachedTextComponents;
    std::vector<std::shared_ptr<IDrawableComponent>> _cachedTextureComponents;
    std::vector<std::shared_ptr<IDrawableComponent>> _cachedCharacterComponents;
    bool _needComponentRefresh = true;
    std::chrono::high_resolution_clock::time_point _lastFrameTime;
    std::chrono::high_resolution_clock::time_point _lastPerformanceReport;
    size_t _frameCount = 0;
    double _totalFrameTime = 0;
    void updateDrawableCache();
    void renderCachedComponents();
    static constexpr int TARGET_FPS = 60;
    static constexpr std::chrono::duration<double,
        std::milli> FRAME_TIME{1000.0 / TARGET_FPS};
    bool _needMenuRefresh = true;
    void logEventSubscriptionStatus();
};

}  // namespace Arcade

#endif  // SRC_CORE_GAMELOOP_GAMELOOP_HPP_
