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
#include <chrono>
#include <thread>
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

void GameLoop::updateGame() {
    if (_currentGame) {
        _currentGame->update(0.016f);  // Fixed time step, ideally should use actual delta time
        auto entities = _currentGame->getEntities();
        
        // Use the current graphics module to draw entities
        if (_window && _window->getDisplayModule()) {
            for (const auto& entity : entities) {
                // Draw each entity using the display module
                _window->getDisplayModule()->drawEntity(
                    entity->getX(), 
                    entity->getY(), 
                    entity->getSymbol()
                );
            }
        }
        
        // Check game state
        if (_currentGame->isGameOver() || _currentGame->hasWon()) {
            // Optional: Add a delay or wait for user input before returning to menu
            if (_eventManager->isKeyPressed(Keys::ESC)) {
                _state = MAIN_MENU;
            }
        }
    }
}

void GameLoop::run() {
    auto running = std::make_shared<bool>(true);
    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    while (*running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;
        
        handleEvents(running);
        if (!*running) break;
        
        _window->clearScreen();
        handleState();
        _window->refreshScreen();
        
        // Simple frame rate limiting
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
    cleanup();
}

void GameLoop::handleEvents(std::shared_ptr<bool> running) {
    _window->pollEvents();
    if (!_window->isWindowOpen()) {
        *running = false;
    }
}

void GameLoop::handleState() {
    switch (_state) {
        case MAIN_MENU:
            displayMainMenu();
            break;
        case GAME_SELECTION:
            displayGameSelection();
            break;
        case GRAPHICS_SELECTION:
            displayGraphicsSelection();
            break;
        case GAME_PLAYING:
            updateGame();
            break;
    }
}

void GameLoop::displayMainMenu() {
    _menu->displayMainMenu(_graphicsLibs, _gameLibs,
        _selectedGraphics, _selectedGame);
}

void GameLoop::displayGameSelection() {
    _menu->displayGameSelection(_gameLibs, _selectedGame);
}

void GameLoop::displayGraphicsSelection() {
    _menu->displayGraphicsSelection(_graphicsLibs, _selectedGraphics);
}

void GameLoop::updateGame() {
    if (_currentGame) {
        _currentGame->update(0.016f);
        auto entities = _currentGame->getEntities();
        for (const auto& entity : entities) {
            // Render each entity (implementation depends on your entity system)
        }
    }
}

void GameLoop::cleanup() {
    _currentGame.reset();
    if (_window) {
        _window->setDisplayModule(nullptr);
        _window.reset();
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

    DIR* dir = openLibraryDirectory();
    if (!dir) return;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        processLibraryEntry(entry);
    }

    closedir(dir);
}

DIR* GameLoop::openLibraryDirectory() {
    DIR* dir = opendir(_libDir.c_str());
    if (!dir) {
        std::cerr << "Error opening directory: " << _libDir << std::endl;
    }
    return dir;
}

void GameLoop::processLibraryEntry(struct dirent* entry) {
    std::string filename(entry->d_name);
    if (isIgnoredFile(filename)) return;

    std::string path = _libDir + "/" + filename;
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error loading " << path << ": " << dlerror() << std::endl;
        return;
    }

    if (!processLibraryHandle(path, handle)) {
        dlclose(handle);
    }
}

bool GameLoop::isIgnoredFile(const std::string& filename) {
    return filename == "." || filename == ".."
        || filename.rfind('.') == std::string::npos ||
        filename.substr(filename.rfind('.')) != ".so"
        || filename.find("arcade_") == std::string::npos;
}

bool GameLoop::processLibraryHandle(const std::string& path, void* handle) {
    using EntryPointFunc = IArcadeModule* (*)();
    auto* entryPoint
        = reinterpret_cast<EntryPointFunc>(dlsym(handle, "entryPoint"));
    if (!entryPoint) {
        std::cerr << "Error finding entryPoint in "
        << path << ": " << dlerror() << std::endl;
        return false;
    }

    return processLibraryInstance(path, handle, entryPoint);
}

bool GameLoop::processLibraryInstance(const std::string& path,
void* handle, IArcadeModule* (*entryPoint)()) {
    IArcadeModule* instance = nullptr;
    try {
        instance = entryPoint();
    } catch (...) {
        std::cerr << "Exception while creating instance from "
            << path << std::endl;
        return false;
    }

    if (dynamic_cast<IDisplayModule*>(instance)) {
        _graphicsLibs.push_back(path);
    } else if (dynamic_cast<IGameModule*>(instance)) {
        _gameLibs.push_back(path);
    } else {
        std::cerr << "Unknown module type in " << path << std::endl;
    }

    delete instance;
    _loadedLibHandles[path] = handle;
    return true;
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
