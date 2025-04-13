// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GameLoop
*/

#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <chrono>
#include <thread>
#include <memory>
#include <string>
#include "GameLoop/GameLoop.hpp"
#include "Core/Score/ScoreManager.hpp"
#include "Core/EventManager/EventManager.hpp"
#include "Shared/Interface/IArcadeModule.hpp"
#include "Shared/Interface/Display/IDisplayModule.hpp"
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Interface/Game/IGameModule.hpp"
#include "ECS/Entity/EntityManager.hpp"
#include "ECS/Components/ComponentManager.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "ECS/Components/Text/TextComponent.hpp"
#include "Shared/Exceptions/Exceptions.hpp"
#include "Shared/Models/KeysType.hpp"
#include "Shared/ScoreProvider/ScoreProvider.hpp"

namespace Arcade {
GameLoop::GameLoop(const std::string& initialLib)
: _state(MAIN_MENU),
_selectedGraphics(0),
_selectedGame(0),
_graphicsLoader(initialLib),
_gameLoader("."),
_menuLoader("./lib/arcade_menu.so"),
_inputPlayerName(""),
_scoreManager(std::make_shared<ScoreManager>()),
_gameSwitch(false),
_needComponentRefresh(true),
_lastFrameTime(std::chrono::high_resolution_clock::now()),
_lastPerformanceReport(std::chrono::high_resolution_clock::now()),
_lastKeyNavigation(std::chrono::high_resolution_clock::now()),
_frameCount(0),
_totalFrameTime(0) {
    _eventManager = std::make_shared<EventManager>();
    _entityManager = std::make_shared<EntityManager>();
    _componentManager = std::make_shared<ComponentManager>();
    _scoreProvider = std::make_shared<ScoreProvider>();
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
        _menu = _menuLoader.getInstanceUPtr("entryPoint");
        if (!_menu)
            throw std::runtime_error("Failed to load menu library");
        _menu->setWindow(_window);
        _menu->setScoreManager(_scoreManager);
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
    _currentGame.reset();
    _menu.reset();

    if (_window) {
        if (_eventManager)
            _eventManager->unsubscribeAll();

        _window->setDisplayModule(nullptr);
        _window.reset();
    }

    _componentManager.reset();
    _entityManager.reset();

    _eventManager.reset();
}

void GameLoop::handleEvents(std::shared_ptr<bool> running) {
    _window->pollEvents();
    if (!_window->isWindowOpen()) {
        *running = false;
    }
}

void GameLoop::handleState() {
    // Mark component cache for refresh when state changes
    static auto previousState = MAIN_MENU;
    if (_state != previousState) {
        _needComponentRefresh = true;
        if (previousState == GAME_PLAYING) {
            if (_currentGame) {
                try {
                    _currentGame->stop();
                    _currentGame = nullptr;
                } catch (...) {
                    std::cerr << "Error stopping current game" << std::endl;
                }
            }
            _eventManager->unsubscribeAll();
        }

        switch (_state) {
            case NAME_INPUT:
                subscribeNameInputEvents();
                break;
            case GRAPHICS_SELECTION:
                subscribeEvents();
                subscribeNavEvents();
                subscribeMouseEvents();
                subscribeEscEvent();
                break;
            case GAME_SELECTION:
                subscribeEvents();
                subscribeNavEvents();
                subscribeMouseEvents();
                break;
            case MAIN_MENU:
                subscribeEvents();
                subscribeNavEvents();
                subscribeMouseEvents();
                break;
            case GAME_PLAYING:
                subscribeEvents();
                subscribeEscEvent();
                break;
            default:
                subscribeNavEvents();
                subscribeMouseEvents();
                break;
        }
        previousState = _state;
    }
    switch (_state) {
        case MAIN_MENU:
            displayMainMenu();
            break;
        case GRAPHICS_SELECTION:
            displayGraphicsSelection();
            break;
        case GAME_SELECTION:
            displayGameSelection();
            break;
        case GAME_PLAYING:
            updateGame();
            break;
        case NAME_INPUT:
            displayNameInput();
            break;
        default:
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
    if (!_graphicsLibs.empty() && _selectedGraphics >= _graphicsLibs.size()) {
        _selectedGraphics = _graphicsLibs.size() - 1;
    }
    _menu->displayGraphicsSelection(_graphicsLibs, _selectedGraphics);
}
void GameLoop::updateGame() {
    // Check if we need to switch games
    if (_gameSwitch) {
        switchGameInGame();
        _gameSwitch = false;
        return;
    }
    if (_currentGame) {
        _currentGame->update(0);
        updateDrawableCache();
        renderCachedComponents();
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

    try {
        if (!processLibraryHandle(path, handle)) {
            dlclose(handle);
        }
    } catch (const LibraryLoadException& e) {
        std::cerr << "Library error: " << e.what() << std::endl;
    } catch (const ArcadeException& e) {
        std::cerr << "Error processing library: " << e.what() << std::endl;
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
        _gameLibs.push_back(path);
    } else if (dynamic_cast<IComponent*>(instance)) {
        _componentsLibs.push_back(path);
    } else {
        if (path != "./lib/arcade_menu.so")
            std::cerr << "Unknown module type in " << path << std::endl;
    }

    delete instance;
    _loadedLibHandles[path] = handle;
    return true;
}

void GameLoop::loadAndStartGame() {
    try {
        if (_currentGame) {
            auto score = _currentGame->getScore();
            _scoreManager->addScore(_gameLibs[_selectedGame], score);
            _currentGame->stop();
            _currentGame.reset();
        }
        _needComponentRefresh = true;
        _cachedTextComponents.clear();
        _cachedTextureComponents.clear();
        _cachedCharacterComponents.clear();
        subscribeEvents();
        subscribeNavEvents();
        subscribeMouseEvents();
        _gameLoader.setLibPath(_gameLibs[_selectedGame]);
        typedef IArcadeModule* (*EntryPointFunc)(
            std::shared_ptr<IEventManager>,
            std::shared_ptr<IComponentManager>,
            std::shared_ptr<IEntityManager>);
        auto handle = dlopen(_gameLibs[_selectedGame].c_str(), RTLD_LAZY);
        if (!handle) {
            throw std::runtime_error(dlerror());
        }
        auto entryPoint = reinterpret_cast<EntryPointFunc>(
            dlsym(handle, "entryPoint"));
        if (!entryPoint) {
            dlclose(handle);
            throw std::runtime_error(dlerror());
        }
        if (_entityManager) {
            _entityManager.reset();
            _entityManager = std::make_shared<EntityManager>();
        }
        if (_componentManager) {
            _componentManager.reset();
            _componentManager = std::make_shared<ComponentManager>();
        }
        _currentGame = std::shared_ptr<IGameModule>(
            static_cast<IGameModule*>(entryPoint(_eventManager,
                _componentManager, _entityManager)),
            [handle](IGameModule* ptr) {
                typedef void (*DestroyFunc)(IGameModule*);
                auto destroy = reinterpret_cast<DestroyFunc>(
                    dlsym(handle, "destroy"));
                if (destroy) destroy(ptr);
                dlclose(handle);
            });
        if (_currentGame) {
            _currentGame->init(_eventManager,
                _componentManager, _entityManager, _scoreProvider);
            _state = GAME_PLAYING;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
    }
}

void GameLoop::loadGraphicsLibraries() {
    try {
        if (_currentGame) {
            auto score = _currentGame->getScore();
            _scoreManager->addScore(_gameLibs[_selectedGame], score);
            _currentGame->stop();
            _currentGame.reset();
        }

        if (_entityManager) {
            auto entities = _entityManager->getEntitiesMap();
            for (const auto& entity : entities) {
                _entityManager->destroyEntity(entity.first);
            }
        }

        if (_componentManager)
            _componentManager = std::make_shared<ComponentManager>();

        if (_eventManager)
            _eventManager->unsubscribeAll();

        std::string newLibPath = _graphicsLibs[_selectedGraphics];
        _graphicsLoader.setLibPath(newLibPath);
        auto newGraphics = _graphicsLoader.getInstanceUPtr("entryPoint");
        auto sharedPtr = std::shared_ptr<IDisplayModule>(
            std::move(newGraphics));
        if (sharedPtr) {
            _currentGraphics = sharedPtr;

            _window->setDisplayModule(sharedPtr);

            subscribeEvents();
            subscribeNavEvents();
            subscribeMouseEvents();
            _state = MAIN_MENU;
        }
    } catch (const LibraryLoadException& e) {
        std::cerr << "Failed to load graphics library: "
                  << e.what() << std::endl;
    } catch (const GraphicsException& e) {
        std::cerr << "Graphics error: " << e.what() << std::endl;
    } catch (const ArcadeException& e) {
        std::cerr << "Error loading graphics libraries: "
                  << e.what() << std::endl;
    }
}

void GameLoop::loadCommonComponents() {
    try {
        for (auto libPath = _componentsLibs.begin();
            libPath != _componentsLibs.end(); ++libPath) {
            auto handle = dlopen(libPath->c_str(), RTLD_LAZY);
            if (!handle) {
                throw LibraryLoadException(
                    "Error loading " + *libPath + ": " + dlerror());
            }
            auto entryPoint = reinterpret_cast<IArcadeModule* (*)()>
                (dlsym(handle, "entryPoint"));
            if (!entryPoint) {
                std::string error =
                    "Error finding entryPoint in " +
                    *libPath + ": " + dlerror();
                dlclose(handle);
                throw LibraryLoadException(error);
            }
            IArcadeModule* instance = entryPoint();
            if (instance) {
                IComponent* componentPtr = static_cast<IComponent*>(instance);
                _commonComponents.push_back
                    (std::shared_ptr<IComponent>(componentPtr));
            }
        }
    } catch (const LibraryLoadException& e) {
        std::cerr << "Library loading error: " << e.what() << std::endl;
    } catch (const ArcadeException& e) {
        std::cerr <<
            "Error loading common components: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error loading common components: "
            << e.what() << std::endl;
    }
}
void GameLoop::displayNameInput() {
    int centerX = _window->getWidth() / 2;
    int centerY = _window->getHeight() / 2;

    auto titleText = std::make_shared<DrawableComponent>();
    titleText->setAsText("ENTER YOUR NAME",
        "assets/fonts/arial.ttf", 30);
    titleText->setPosition(centerX - 80, TITLE_Y);
    titleText->setColor(Color::WHITE);
    titleText->setVisibility(true);
    _currentGraphics->drawDrawable(titleText);

    auto inputText = std::make_shared<DrawableComponent>();
    inputText->setAsText(_inputPlayerName + "_",
        "assets/fonts/arial.ttf", 30);
    // Calculate position based on text length
    inputText->setPosition(centerX - (_inputPlayerName.length() * 5), centerY);
    inputText->setVisibility(true);
    inputText->setText(_inputPlayerName + "_");
    inputText->setColor(Color::GREEN);
    _currentGraphics->drawDrawable(inputText);

    auto instructionsText = std::make_shared<DrawableComponent>();
    instructionsText->setAsText("Press ENTER to confirm, ESC to cancel",
        "assets/fonts/arial.ttf", 20);
    instructionsText->setPosition(centerX - 150, centerY + 60);
    instructionsText->setVisibility(true);
    instructionsText->setText("Press ENTER to confirm, ESC to cancel");
    instructionsText->setColor(Color::WHITE);
    _currentGraphics->drawDrawable(instructionsText);
}
void GameLoop::changeState(std::shared_ptr<IGameState> newState) {
    // Implementation of state management
    // This would be used for more complex state transitions
}

void GameLoop::updateDrawableCache() {
    if (_state == GAME_PLAYING) {
        _needComponentRefresh = true;
    }
    if (!_needComponentRefresh) {
        return;
    }
    _cachedTextComponents.clear();
    _cachedTextureComponents.clear();
    _cachedCharacterComponents.clear();
    auto drawableComponents
        = _componentManager->getAllComponentsByType(ComponentType::DRAWABLE);
    for (const auto& component : drawableComponents) {
        auto drawableComp
            = std::dynamic_pointer_cast<IDrawableComponent>(component);
        if (!drawableComp || !drawableComp->isRenderable()) continue;
        if (drawableComp->shouldRenderAsText()) {
            _cachedTextComponents.push_back(drawableComp);
        } else if (drawableComp->shouldRenderAsTexture()) {
            _cachedTextureComponents.push_back(drawableComp);
        } else if (drawableComp->shouldRenderAsCharacter()) {
            _cachedCharacterComponents.push_back(drawableComp);
        }
    }
    _needComponentRefresh = false;
}

void GameLoop::renderCachedComponents() {
    for (const auto& texture : _cachedTextureComponents) {
        _currentGraphics->drawDrawable(texture);
    }
    for (const auto& character : _cachedCharacterComponents) {
        _currentGraphics->drawDrawable(character);
    }
    for (const auto& text : _cachedTextComponents) {
        _currentGraphics->drawDrawable(text);
    }
}

void GameLoop::subscribeEscEvent() {
    KeyEvent escEvent(Arcade::Keys::ESC, Arcade::EventType::KEY_PRESSED);
    _eventManager->subscribe(escEvent, [this](const IEvent& event) {
        (void)event;
        switch (_state) {
            case GAME_PLAYING:
                _state = MAIN_MENU;
                if (_currentGame) {
                    try {
                        _currentGame->stop();
                    } catch (...) {
                        std::cerr << "Error stopping game" << std::endl;
                    }
                    _currentGame = nullptr;
                }
                _needComponentRefresh = true;
                subscribeEvents();
                subscribeNavEvents();
                subscribeMouseEvents();
                break;
            case GRAPHICS_SELECTION:
            case GAME_SELECTION:
                _state = MAIN_MENU;
                break;
            case NAME_INPUT:
                break;
            default:
                break;
        }
    });
}



bool _needMenuRefresh = false;

void GameLoop::run() {
    std::cout << "GameLoop: Starting run method" << std::endl;

    subscribeEvents();
    subscribeNavEvents();
    subscribeMouseEvents();
    subscribeEscEvent();
    _state = MAIN_MENU;
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    auto running = std::make_shared<bool>(true);
    _lastGraphicsSwitch = std::chrono::high_resolution_clock::now();
    while (*running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime
            - lastFrameTime).count();
        lastFrameTime = currentTime;

        // Process events
        handleEvents(running);
        if (!*running) break;
        // Clear the display
        if (_currentGraphics != nullptr)
            _currentGraphics->clearScreen();
        handleState();
        if (_currentGraphics != nullptr)
            _currentGraphics->refreshScreen();

        // Cap frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    cleanup();
}

}  // namespace Arcade
