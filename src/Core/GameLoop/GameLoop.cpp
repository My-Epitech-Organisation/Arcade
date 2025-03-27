// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GameLoop
*/

#include "GameLoop/GameLoop.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <utility>
#include <memory>
#include <string>
#include "EventManager/EventManager.hpp"
#include "Interface/IArcadeModule.hpp"

namespace Arcade {
GameLoop::GameLoop(const std::string& initialLib)
: _state(MAIN_MENU),
_selectedGraphics(0),
_selectedGame(0),
_graphicsLoader(initialLib),
_gameLoader(".") {
    _eventManager = std::make_shared<EventManager>();
    subscribeEvents();
    subscribeNavEvents();
    subscribeMouseEvents();
    try {
        _graphicsLoader.setLibPath(initialLib);
        _currentGraphics = _graphicsLoader.getInstanceUPtr("entryPoint");
        if (!_currentGraphics)
            throw std::runtime_error("Failed to load initial graphics library");
        _window = std::make_shared<Window>(std::move(_currentGraphics),
            _eventManager);
        _menu = std::make_unique<Menu>(_window);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
    loadAvailableLibraries();
    auto it = std::find(_graphicsLibs.begin(), _graphicsLibs.end(), initialLib);
    if (it != _graphicsLibs.end())
        _selectedGraphics = std::distance(_graphicsLibs.begin(), it);
}


GameLoop::~GameLoop() {
}

void GameLoop::run() {
    bool running = true;
    while (running) {
        _window->pollEvents();
        if (!_window->isWindowOpen()) {
            running = false;
            continue;
        }
        _window->clearScreen();
        switch (_state) {
            case MAIN_MENU:
                _menu->displayMainMenu(_graphicsLibs,
                    _gameLibs, _selectedGraphics, _selectedGame);
                break;
            case GAME_SELECTION:
                _menu->displayGameSelection(_gameLibs, _selectedGame);
                break;
            case GRAPHICS_SELECTION:
                _menu->displayGraphicsSelection(_graphicsLibs,
                    _selectedGraphics);
                break;
            case GAME_PLAYING:
                if (_currentGame) {
                    _currentGame->update(0.016f);
                    auto entities = _currentGame->getEntities();
                    for (const auto& entity : entities) {
                        // Render each entity (implementation
                        // depends on your entity system)
                    }
                }
                break;
        }
        _window->refreshScreen();
    }
    _currentGame.reset();  // Release current game first
    if (_window) {
        _window->setDisplayModule(nullptr);
        _window.reset();  // Now safe to reset window
    }
    for (auto& [path, handle] : _loadedLibHandles) {
        if (handle) {
            dlclose(handle);
        }
    }
}
void GameLoop::loadAvailableLibraries() {
    _graphicsLibs.clear();
    _gameLibs.clear();

    DIR* dir = opendir(_libDir.c_str());
    if (!dir) {
        std::cerr << "Error opening directory: " << _libDir << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename(entry->d_name);
        if (filename == "." || filename == "..")
            continue;
        size_t dot_pos = filename.rfind('.');
        if (dot_pos == std::string::npos)
            continue;
        std::string extension = filename.substr(dot_pos);
        if (extension != ".so" || filename.find("arcade_") == std::string::npos)
            continue;
        std::string path = _libDir + "/" + filename;
        void* handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "Error loading " << path
            << ": " << dlerror() << std::endl;
            continue;
        }
        using EntryPointFunc = IArcadeModule* (*)();
        auto* entryPoint = reinterpret_cast<EntryPointFunc>
            (dlsym(handle, "entryPoint"));
        if (!entryPoint) {
            std::cerr << "Error finding entryPoint in "
            << path << ": " << dlerror() << std::endl;
            dlclose(handle);
            continue;
        }
        IArcadeModule* instance = nullptr;
        try {
            instance = entryPoint();
        } catch (...) {
            std::cerr << "Exception while creating instance from "
            << path << std::endl;
            dlclose(handle);
            continue;
        }
        if (dynamic_cast<IDisplayModule*>(instance))
            _graphicsLibs.push_back(path);
        else if (dynamic_cast<IGameModule*>(instance))
            _gameLibs.push_back(path);
        else
            std::cerr << "Unknown module type in " << path << std::endl;
        delete instance;
        _loadedLibHandles[path] = handle;
    }
    closedir(dir);
}
void GameLoop::loadAndStartGame() {
    try {
        _gameLoader.setLibPath(_gameLibs[_selectedGame]);
        _currentGame = _gameLoader.getInstanceUPtr("entryPoint");
        if (_currentGame) {
            _currentGame->init();
            _state = GAME_PLAYING;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
    }
}

void GameLoop::loadGraphicsLibraries() {
    try {
        std::string newLibPath = _graphicsLibs[_selectedGraphics];
        std::cout << "Loading graphics library: " << newLibPath << std::endl;
        _graphicsLoader.setLibPath(newLibPath);
        auto newGraphics = _graphicsLoader.getInstanceUPtr("entryPoint");
        auto sharedPtr =
            std::shared_ptr<IDisplayModule>(std::move(newGraphics));
        if (sharedPtr) {
            _window->setDisplayModule(sharedPtr);
            _state = MAIN_MENU;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading graphics: " << e.what() << std::endl;
    }
}

void GameLoop::changeState(std::shared_ptr<IGameState> newState) {
    // Implementation of state management
    // This would be used for more complex state transitions
}

}  // namespace Arcade
