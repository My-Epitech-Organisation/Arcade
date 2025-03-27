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

namespace Arcade {

class GameLoop : public IStateManager {
 public:
    explicit GameLoop(const std::string& initialLib);
    ~GameLoop();
    void run();
    void changeState(std::shared_ptr<IGameState> newState) override;

 private:
    enum State {
        MAIN_MENU,
        GAME_PLAYING,
        GRAPHICS_SELECTION,
        GAME_SELECTION
    };
    State _state;
    void loadAvailableLibraries();
    void subscribeEvents();
    void subscribeNavEvents();
    void subscribeMouseEvents();
    void loadAndStartGame();
    void loadGraphicsLibraries();
    std::shared_ptr<AEventManager> _eventManager;
    std::shared_ptr<Window> _window;
    std::unique_ptr<Menu> _menu;
    DLLoader<IDisplayModule> _graphicsLoader;
    DLLoader<IGameModule> _gameLoader;
    std::shared_ptr<IDisplayModule> _currentGraphics;
    std::shared_ptr<IGameModule> _currentGame;
    std::vector<std::string> _graphicsLibs;
    std::vector<std::string> _gameLibs;
    size_t _selectedGraphics;
    size_t _selectedGame;
    const std::string _libDir = "./lib/";
    std::map<std::string, void*> _loadedLibHandles;
    static constexpr int TITLE_Y = 50;
    static constexpr int MENU_START_Y = 100;
    static constexpr int MENU_ITEM_HEIGHT = 20;
    static constexpr int STATUS_OFFSET_Y = 40;
};

}  // namespace Arcade

#endif  // SRC_CORE_GAMELOOP_GAMELOOP_HPP_
