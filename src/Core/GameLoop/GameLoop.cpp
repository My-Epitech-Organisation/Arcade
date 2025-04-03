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
#include "Core/EventManager/EventManager.hpp"
#include "Shared/Interface/IArcadeModule.hpp"
#include "Shared/Interface/Display/IDisplayModule.hpp"
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Interface/Game/IGameModule.hpp"
#include "ECS/Entity/EntityManager.hpp"
#include "ECS/Components/ComponentManager.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"

namespace Arcade {
GameLoop::GameLoop(const std::string& initialLib)
: _state(MAIN_MENU),
_selectedGraphics(0),
_selectedGame(0),
_graphicsLoader(initialLib),
_gameLoader(".") {
    _eventManager = std::make_shared<EventManager>();
    _entityManager = std::make_shared<EntityManager>();
    _componentManager = std::make_shared<ComponentManager>();
    subscribeEvents();
    subscribeNavEvents();
    subscribeMouseEvents();
    try {
        _graphicsLoader.setLibPath(initialLib);
        _currentGraphics = _graphicsLoader.getInstanceUPtr("entryPoint");
        loadCommonComponents();
        if (!_currentGraphics)
            throw std::runtime_error("Failed to load initial graphics library");
        _window = std::make_shared<Window>(_currentGraphics,
            _eventManager);
        _menu = std::make_unique<Menu>(_window);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
    loadAvailableLibraries();
    std::string initialLibFormatted = "./" + initialLib;
    auto it = std::find(_graphicsLibs.begin(),
        _graphicsLibs.end(), initialLibFormatted);
    if (it != _graphicsLibs.end())
        _selectedGraphics = std::distance(_graphicsLibs.begin(), it);
}


GameLoop::~GameLoop() {
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
        _currentGame->update();
        auto entities = _entityManager->getEntities();
        for (const auto& [entityId, entityName] : entities) {
            auto position = _componentManager->getComponentByType(entityId, ComponentType::POSITION);
            auto positionComponent = std::dynamic_pointer_cast<PositionComponent>(position);
            if (!positionComponent) {
                continue;
            }

            int x = static_cast<int>(positionComponent->x);
            int y = static_cast<int>(positionComponent->y);

            auto sprite = _componentManager->getComponentByType(entityId, ComponentType::SPRITE);
            auto spriteComponent = std::dynamic_pointer_cast<SpriteComponent>(sprite);
            if (spriteComponent) {
                _currentGraphics->drawTexture(x, y, spriteComponent->spritePath);
                continue;
            }

            std::string specialCompo = _currentGame->getSpecialCompSprite(entityId);
            if (specialCompo != "") {
                _currentGraphics->drawTexture(x, y, specialCompo);
                continue;
            }

            _currentGraphics->drawEntity(x, y, '?');
        }

        if (_currentGame->isGameOver()) {
            std::string message = _currentGame->hasWon() ? "YOU WIN!" : "GAME OVER";
            _window->drawText(message, _window->getWidth() / 2 - 50, _window->getHeight() / 2, Color::WHITE);

            _window->drawText("Press ESC to return to menu",
                _window->getWidth() / 2 - 100, _window->getHeight() / 2 + 30, Color::WHITE);
        }
    } else {
        std::cout << "[DEBUG] No current game to update" << std::endl;
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

    std::string path = _libDir + filename;
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
    std::string initialLibFormatted = "./" + _graphicsLoader.getLibPath();
    if (path == initialLibFormatted) {
        _graphicsLibs.push_back(path);
        _loadedLibHandles[path] = handle;
        return true;
    }

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
        std::cout << "PK tu segfault fils du pute ?" << std::endl;
        _gameLibs.push_back(path);
    } else if (dynamic_cast<IComponent*>(instance)) {
        _componentsLibs.push_back(path);
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
        // Pass the entity manager, component manager, and event manager to the game module
        typedef IArcadeModule* (*EntryPointFunc)(
            std::shared_ptr<IEventManager>,
            std::shared_ptr<IComponentManager>,
            std::shared_ptr<IEntityManager>);
        auto handle = dlopen(_gameLibs[_selectedGame].c_str(), RTLD_LAZY);
        if (!handle) {
            throw std::runtime_error(dlerror());
        }
        auto entryPoint = reinterpret_cast<EntryPointFunc>(dlsym(handle, "entryPoint"));
        if (!entryPoint) {
            dlclose(handle);
            throw std::runtime_error(dlerror());
        }
        _currentGame = std::shared_ptr<IGameModule>(
            static_cast<IGameModule*>(entryPoint(_eventManager, _componentManager, _entityManager)),
            [handle](IGameModule* ptr) {
                typedef void (*DestroyFunc)(IGameModule*);
                auto destroy = reinterpret_cast<DestroyFunc>(dlsym(handle, "destroy"));
                if (destroy) destroy(ptr);
                dlclose(handle);
            }
        );
        if (_currentGame) {
            _currentGame->init(_eventManager, _componentManager, _entityManager);
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

void GameLoop::loadCommonComponents() {
    try {
        for (auto libPath = _componentsLibs.begin();
            libPath != _componentsLibs.end(); ++libPath) {
            auto handle = dlopen(libPath->c_str(), RTLD_LAZY);
            if (!handle) {
                std::cerr << "Error loading " << *libPath << ": " << dlerror() << std::endl;
                continue;
            }
            auto entryPoint = reinterpret_cast<IArcadeModule* (*)()>(dlsym(handle, "entryPoint"));
            if (!entryPoint) {
                std::cerr << "Error finding entryPoint in "
                    << *libPath << ": " << dlerror() << std::endl;
                dlclose(handle);
                continue;
            }
            IArcadeModule* instance = entryPoint();
            if (instance) {
                // Create a shared pointer to the component
                IComponent* componentPtr = static_cast<IComponent*>(instance);
                _commonComponents.push_back(std::shared_ptr<IComponent>(componentPtr));
                // Don't delete the instance as it's now managed by the shared pointer
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading common components: " << e.what() << std::endl;
    }
}

void GameLoop::changeState(std::shared_ptr<IGameState> newState) {
    // Implementation of state management
    // This would be used for more complex state transitions
}

}  // namespace Arcade
