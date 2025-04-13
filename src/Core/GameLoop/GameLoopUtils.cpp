// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GameLoop Utils
*/

#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include "GameLoop/GameLoop.hpp"
#include "ECS/Entity/EntityManager.hpp"
#include "ECS/Components/ComponentManager.hpp"
#include "EventManager/KeyEvent/KeyEvent.hpp"
#include "EventManager/KeyEvent/MouseEvent.hpp"
#include "Shared/Models/EventType.hpp"

namespace Arcade {

void GameLoop:: subscribeEvents() {
    subscribeNum1Event();
    subscribeNum2Event();
    subscribeNum3Event();
    subscribeNum4Event();
    subscribeNum5Event();
    subscribeGKeyEvent();
    subscribeHKeyEvent();
    subscribeNKeyEvent();
    subscribePKeyEvent();
}

void GameLoop::logEventSubscriptionStatus() {
}

void GameLoop::subscribeGKeyEvent() {
    KeyEvent gKeyEvent(Keys::G, EventType::KEY_RELEASED);
    _eventManager->subscribe(gKeyEvent, [this](const IEvent& event) {
        (void)event;
        if (_state == GAME_PLAYING && _gameLibs.size() > 1 && !_gameSwitch) {
            _selectedGame = (_selectedGame + 1) % _gameLibs.size();
            _gameSwitch = true;
        }
    });
}

void GameLoop::subscribeHKeyEvent() {
    KeyEvent hKeyEvent(Keys::H, EventType::KEY_RELEASED);
    _eventManager->subscribe(hKeyEvent, [this](const IEvent& event) {
        (void)event;
        if (_state == GAME_PLAYING && _gameLibs.size() > 1 && !_gameSwitch) {
            _selectedGame = (_selectedGame == 0) ?
                _gameLibs.size() - 1 : _selectedGame - 1;
            _gameSwitch = true;
        }
    });
}

void GameLoop::subscribeNKeyEvent() {
    KeyEvent nKeyEvent(Keys::N, EventType::KEY_RELEASED);
    _eventManager->subscribe(nKeyEvent, [this](const IEvent& event) {
        (void)event;
        if (_state == GAME_PLAYING && _graphicsLibs.size() > 1) {
            _selectedGraphics = (_selectedGraphics + 1) % _graphicsLibs.size();
            switchGraphicsInGame();
        }
    });
}

void GameLoop::subscribePKeyEvent() {
    KeyEvent pKeyEvent(Keys::P, EventType::KEY_RELEASED);
    _eventManager->subscribe(pKeyEvent, [this](const IEvent& event) {
        (void)event;
        if (_state == GAME_PLAYING && _graphicsLibs.size() > 1) {
            _selectedGraphics = (_selectedGraphics == 0) ?
                _graphicsLibs.size() - 1 : _selectedGraphics - 1;
            switchGraphicsInGame();
        }
    });
}

void GameLoop::subscribeNameInputEvents() {
    for (int key = Keys::A; key <= Keys::Z; key++) {
        KeyEvent letterEvent(static_cast<Keys>(key), EventType::KEY_PRESSED);
        _eventManager->subscribe(letterEvent, [this, key](const IEvent& event) {
            (void)event;
            if (_state == NAME_INPUT && _inputPlayerName.length() < 15) {
                char c = 'A' + (key - Keys::A);
                _inputPlayerName += c;
            }
        });
    }

    for (int key = Keys::NUM6; key <= Keys::NUM9; key++) {
        KeyEvent numEvent(static_cast<Keys>(key), EventType::KEY_PRESSED);
        _eventManager->subscribe(numEvent, [this, key](const IEvent& event) {
            (void)event;
            if (_state == NAME_INPUT && _inputPlayerName.length() < 15) {
                char c = '0' + (key - Keys::NUM0);
                _inputPlayerName += c;
            }
        });
    }

    KeyEvent backspaceEvent(Keys::BACKSPACE, EventType::KEY_PRESSED);
    _eventManager->subscribe(backspaceEvent, [this](const IEvent& event) {
        (void)event;
        if (_state == NAME_INPUT && !_inputPlayerName.empty()) {
            _inputPlayerName.pop_back();
        }
    });

    KeyEvent enterEvent(Keys::ENTER, EventType::KEY_PRESSED);
    _eventManager->subscribe(enterEvent, [this](const IEvent& event) {
        (void)event;
        if (_state == NAME_INPUT) {
            if (!_inputPlayerName.empty()) {
                _scoreManager->setCurrentPlayerName(_inputPlayerName);
                std::cout << "Player name set to: " <<
                    _inputPlayerName << std::endl;
            }
            _state = MAIN_MENU;
        }
    });

    KeyEvent escEvent(Keys::ESC, EventType::KEY_PRESSED);
    _eventManager->subscribe(escEvent, [this](const IEvent& event) {
        (void)event;
        if (_state == NAME_INPUT) {
            _inputPlayerName = "";
            _state = MAIN_MENU;
        }
    });
}

void GameLoop::subscribeNum1Event() {
    KeyEvent num1Event(Keys::NUM1, EventType::KEY_PRESSED);
    _eventManager->subscribe(num1Event, [this](const IEvent& event) {
        (void)event;
        if (_state == MAIN_MENU && !_gameLibs.empty()) {
            loadAndStartGame();
        }
    });
}

void GameLoop::subscribeNum2Event() {
    KeyEvent num2Event(Keys::NUM2, EventType::KEY_PRESSED);
    _eventManager->subscribe(num2Event, [this](const IEvent& event) {
        (void)event;
        if (_state == MAIN_MENU) {
            _state = GRAPHICS_SELECTION;
        }
    });
}

void GameLoop::subscribeNum3Event() {
    KeyEvent num3Event(Keys::NUM3, EventType::KEY_PRESSED);
    _eventManager->subscribe(num3Event, [this](const IEvent& event) {
        (void)event;
        if (_state == MAIN_MENU) {
            _state = GAME_SELECTION;
        }
    });
}

void GameLoop::subscribeNum4Event() {
    KeyEvent num4Event(Keys::NUM4, EventType::KEY_PRESSED);
    _eventManager->subscribe(num4Event, [this](const IEvent& event) {
        (void)event;
        if (_state == MAIN_MENU) {
            _window->closeWindow();
        }
    });
}


void GameLoop::subscribeNum5Event() {
    KeyEvent num5Event(Keys::NUM5, EventType::KEY_PRESSED);
    _eventManager->subscribe(num5Event, [this](const IEvent& event) {
        (void)event;
        if (_state == MAIN_MENU) {
            _inputPlayerName = "";
            _state = NAME_INPUT;
        }
    });
}

void GameLoop::subscribeNavEvents() {
    subscribeUpEvent();
    subscribeDownEvent();
    subscribeEnterEvent();
    subscribeEscEvent();
}

void GameLoop::subscribeUpEvent() {
    KeyEvent upEvent(Keys::UP, EventType::KEY_PRESSED);
    _eventManager->subscribe(upEvent, [this](const IEvent& event) {
        (void)event;
        
        // Add debounce logic to prevent too rapid selection changes
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - _lastKeyNavigation).count();
            
        if (elapsed < KEY_NAVIGATION_COOLDOWN_MS) {
            return;  // Skip this key press if too soon after the last one
        }
        
        if (_state == GAME_SELECTION && _gameLibs.size() > 0 && _selectedGame > 0) {
            _selectedGame--;
            _needMenuRefresh = true;
            _lastKeyNavigation = now;
        } else if (_state == GRAPHICS_SELECTION && _graphicsLibs.size() > 0 && _selectedGraphics > 0) {
            size_t oldSelection = _selectedGraphics;
            _selectedGraphics--;
            std::cout << "Graphics selection moved up: " << oldSelection 
                      << " -> " << _selectedGraphics << std::endl;
            _needMenuRefresh = true;
            _lastKeyNavigation = now;
        }
    });
}

void GameLoop::subscribeDownEvent() {
    KeyEvent downEvent(Keys::DOWN, EventType::KEY_PRESSED);
    _eventManager->subscribe(downEvent, [this](const IEvent& event) {
        (void)event;
        
        // Add debounce logic to prevent too rapid selection changes
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - _lastKeyNavigation).count();
            
        if (elapsed < KEY_NAVIGATION_COOLDOWN_MS) {
            return;  // Skip this key press if too soon after the last one
        }
        
        if (_state == GAME_SELECTION && _gameLibs.size() > 0 && _selectedGame < _gameLibs.size() - 1) {
            _selectedGame++;
            _needMenuRefresh = true;
            _lastKeyNavigation = now;
        } else if (_state == GRAPHICS_SELECTION && _graphicsLibs.size() > 0 && 
                  _selectedGraphics < _graphicsLibs.size() - 1) {
            size_t oldSelection = _selectedGraphics;
            _selectedGraphics++;
            std::cout << "Graphics selection moved down: " << oldSelection 
                      << " -> " << _selectedGraphics << std::endl;
            _needMenuRefresh = true;
            _lastKeyNavigation = now;
        }
    });
}

void GameLoop::subscribeEnterEvent() {
    KeyEvent enterEvent(Keys::ENTER, EventType::KEY_PRESSED);
    _eventManager->subscribe(enterEvent, [this](const IEvent& event) {
        (void)event;
        if (_state == GAME_SELECTION && !_gameLibs.empty()) {
            loadAndStartGame();
        } else if (_state == GRAPHICS_SELECTION && !_graphicsLibs.empty()) {
            loadGraphicsLibraries();
        }
    });
}

void GameLoop::subscribeMouseEvents() {
    subscribeMouseMoveEvent();
    subscribeLeftClickEvent();
    subscribeRightClickEvent();
}

void GameLoop::subscribeRightClickEvent() {
    MouseEvent rightClickEvent(MouseButton::RIGHT,
        EventType::MOUSE_BUTTON_PRESSED, 0, 0);
    _eventManager->subscribe(rightClickEvent, [this](const IEvent& event) {
        (void)event;
        if (_state == GAME_SELECTION || _state == GRAPHICS_SELECTION) {
            _state = MAIN_MENU;
        }
    });
}

void GameLoop::subscribeMouseMoveEvent() {
    MouseEvent mouseMoveEvent(MouseButton::NONE, EventType::MOUSE_MOVED, 0, 0);
    _eventManager->subscribe(mouseMoveEvent, [this](const IEvent& event) {
        (void)event;
        auto [x, y] = _eventManager->getMousePosition();
        int centerX = _window->getWidth() / 2;
        if (_state == GAME_SELECTION && !_gameLibs.empty()) {
            handleMouseMoveGameSelection(x, y, centerX);
        } else if (_state == GRAPHICS_SELECTION && !_graphicsLibs.empty()) {
            handleMouseMoveGraphicsSelection(x, y, centerX);
        }
    });
}

void GameLoop::handleMouseMoveGameSelection(int x, int y, int centerX) {
    for (size_t i = 0; i < _gameLibs.size(); i++) {
        if (x >= centerX - 100 && x <= centerX + 100 &&
            y >= MENU_START_Y + i * MENU_ITEM_HEIGHT &&
            y <= MENU_START_Y + (i + 1) * MENU_ITEM_HEIGHT) {
            _selectedGame = i;
        }
    }
}

void GameLoop::handleMouseMoveGraphicsSelection(int x, int y, int centerX) {
    if (_graphicsLibs.empty()) return;
    
    bool selectionChanged = false;
    size_t oldSelection = _selectedGraphics;
    
    for (size_t i = 0; i < _graphicsLibs.size(); i++) {
        int itemTop = MENU_START_Y + static_cast<int>(i) * MENU_ITEM_HEIGHT;
        int itemBottom = MENU_START_Y + static_cast<int>(i + 1) * MENU_ITEM_HEIGHT;
        
        if (x >= centerX - 150 && x <= centerX + 150 &&
            y >= itemTop && y <= itemBottom) {
            
            if (_selectedGraphics != i) {
                _selectedGraphics = i;
                selectionChanged = true;
            }
            break;
        }
    }
    
    // If selection changed, we should trigger a refresh
    if (selectionChanged) {
        _needMenuRefresh = true;
        std::cout << "Graphics selection changed from " << oldSelection 
                  << " to " << _selectedGraphics << std::endl;
    }
}

void GameLoop::subscribeLeftClickEvent() {
    MouseEvent leftClickEvent(MouseButton::LEFT,
        EventType::MOUSE_BUTTON_PRESSED, 0, 0);
    _eventManager->subscribe(leftClickEvent, [this](const IEvent& event) {
        (void)event;
        auto [x, y] = _eventManager->getMousePosition();
        int centerX = _window->getWidth() / 2;
        if (_state == MAIN_MENU) {
            handleLeftClickMainMenu(x, y, centerX);
        } else if (_state == GAME_SELECTION) {
            handleLeftClickGameSelection(x, y, centerX);
        } else if (_state == GRAPHICS_SELECTION) {
            handleLeftClickGraphicsSelection(x, y, centerX);
        } else if (_state == GAME_PLAYING) {
            // Let the game handle mouse clicks while playing
        }
    });
}

void GameLoop::handleLeftClickMainMenu(int x, int y, int centerX) {
    if (x >= centerX - 100 && x <= centerX + 100) {
        if (y >= MENU_START_Y && y <= MENU_START_Y + MENU_ITEM_HEIGHT) {
            if (!_gameLibs.empty()) {
                loadAndStartGame();
            }
        } else if (y >= MENU_START_Y + MENU_ITEM_HEIGHT &&
                   y <= MENU_START_Y + 2 * MENU_ITEM_HEIGHT) {
            _state = GRAPHICS_SELECTION;
        } else if (y >= MENU_START_Y + 2 * MENU_ITEM_HEIGHT &&
                   y <= MENU_START_Y + 3 * MENU_ITEM_HEIGHT) {
            _state = GAME_SELECTION;
        } else if (y >= MENU_START_Y + 3 * MENU_ITEM_HEIGHT &&
                   y <= MENU_START_Y + 4 * MENU_ITEM_HEIGHT) {
            _window->closeWindow();
        } else if (y >= MENU_START_Y + 4 * MENU_ITEM_HEIGHT &&
                   y <= MENU_START_Y + 5 * MENU_ITEM_HEIGHT) {
            _inputPlayerName = "";
            _state = NAME_INPUT;
        }
    }
}

void GameLoop::handleLeftClickGameSelection(int x, int y, int centerX) {
    bool clickedOnGame = false;
    for (size_t i = 0; i < _gameLibs.size(); i++) {
        if (x >= centerX - 100 && x <= centerX + 100 &&
            y >= MENU_START_Y + i * MENU_ITEM_HEIGHT &&
            y <= MENU_START_Y + (i + 1) * MENU_ITEM_HEIGHT) {
            _selectedGame = i;
            clickedOnGame = true;
            loadAndStartGame();
            break;
        }
    }
    if (!clickedOnGame && y >= MENU_START_Y
        + _gameLibs.size() * MENU_ITEM_HEIGHT + 40) {
        _state = MAIN_MENU;
    }
}

void GameLoop::handleLeftClickGraphicsSelection(int x, int y, int centerX) {
    if (_graphicsLibs.empty()) return;
    
    bool clickedOnGraphics = false;
    for (size_t i = 0; i < _graphicsLibs.size(); i++) {
        int itemTop = MENU_START_Y + static_cast<int>(i) * MENU_ITEM_HEIGHT;
        int itemBottom = MENU_START_Y + static_cast<int>(i + 1) * MENU_ITEM_HEIGHT;
        
        if (x >= centerX - 150 && x <= centerX + 150 &&
            y >= itemTop && y <= itemBottom) {
            
            _selectedGraphics = i;
            clickedOnGraphics = true;
            std::cout << "Selected graphics library: " << _graphicsLibs[i] << std::endl;
            loadGraphicsLibraries();
            break;
        }
    }
    
    // Back to main menu button
    int backButtonY = MENU_START_Y + static_cast<int>(_graphicsLibs.size()) * MENU_ITEM_HEIGHT + 40;
    if (!clickedOnGraphics && y >= backButtonY && y <= backButtonY + MENU_ITEM_HEIGHT &&
        x >= centerX - 100 && x <= centerX + 100) {
        _state = MAIN_MENU;
    }
}

void GameLoop::switchGameInGame() {
    try {
        bool wasInGame = (_state == GAME_PLAYING);

        std::string previousGamePath = "";
        int previousScore = 0;

        if (_currentGame) {
            previousGamePath = _gameLibs[_selectedGame];
            previousScore = _currentGame->getScore();

            _currentGame->stop();
            _currentGame.reset();
        }
        _needComponentRefresh = true;
        _cachedTextComponents.clear();
        _cachedTextureComponents.clear();
        _cachedCharacterComponents.clear();
        _entityManager = std::make_shared<EntityManager>();

        _componentManager = std::make_shared<ComponentManager>();

        _eventManager->unsubscribeAll();
        subscribeEvents();
        subscribeNavEvents();
        subscribeMouseEvents();
        subscribeEscEvent();

        _gameLoader.setLibPath(_gameLibs[_selectedGame]);
        typedef IArcadeModule* (*EntryPointFunc)(
            std::shared_ptr<IEventManager>,
            std::shared_ptr<IComponentManager>,
            std::shared_ptr<IEntityManager>);

        auto handle = dlopen(_gameLibs[_selectedGame].c_str(), RTLD_LAZY);
        if (!handle)
            throw std::runtime_error(dlerror());

        auto entryPoint = reinterpret_cast<EntryPointFunc>(
            dlsym(handle, "entryPoint"));
        if (!entryPoint) {
            dlclose(handle);
            throw std::runtime_error(dlerror());
        }
        _entityManager.reset();
        _entityManager = std::make_shared<EntityManager>();
        _componentManager.reset();
        _componentManager = std::make_shared<ComponentManager>();
        auto newGame = std::shared_ptr<IGameModule>(
            static_cast<IGameModule*>(entryPoint(_eventManager,
                _componentManager, _entityManager)),
            [handle](IGameModule* ptr) {
                typedef void (*DestroyFunc)(IGameModule*);
                auto destroy = reinterpret_cast<DestroyFunc>(
                    dlsym(handle, "destroy"));
                if (destroy) destroy(ptr);
                dlclose(handle);
            });

        if (newGame) {
            _currentGame = newGame;

            if (!previousGamePath.empty()) {
                _scoreManager->addScore(previousGamePath, previousScore);
            }

            _window->clearScreen();

            _currentGame->init(_eventManager, _componentManager,
                _entityManager, _scoreProvider);
            _state = GAME_PLAYING;

            _window->refreshScreen();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error switching game: " << e.what() << std::endl;
    }
}

void GameLoop::switchGraphicsInGame() {
    try {
        bool wasInGame = (_state == GAME_PLAYING);
        
        // Save current game state
        std::shared_ptr<IGameModule> currentGameBackup = _currentGame;
        std::string newLibPath = _graphicsLibs[_selectedGraphics];
        
        // Prepare drawable components before switch
        auto drawableComponents = _componentManager->getAllComponentsByType(ComponentType::DRAWABLE);
        
        // Load new graphics library without disturbing game state
        _graphicsLoader.setLibPath(newLibPath);
        auto newGraphics = _graphicsLoader.getInstanceUPtr("entryPoint");
        auto sharedPtr = std::shared_ptr<IDisplayModule>(std::move(newGraphics));

        if (sharedPtr) {
            // Apply fade-out effect on current display (optional)
            if (_currentGraphics && _window) {
                _window->clearScreen();
                _window->refreshScreen();
            }
            
            // Switch to new graphics library
            _currentGraphics = sharedPtr;
            _window->setDisplayModule(sharedPtr);
            
            // Trigger a single refresh of component cache
            _needComponentRefresh = true;
            
            // Restore game state
            if (wasInGame && currentGameBackup) {
                _window->clearScreen();
                _state = GAME_PLAYING;
                _window->refreshScreen();
            }
        }
    } catch (const LibraryLoadException& e) {
        std::cerr << "Failed to load graphics library: " << e.what() << std::endl;
    } catch (const GraphicsException& e) {
        std::cerr << "Graphics error: " << e.what() << std::endl;
    } catch (const ArcadeException& e) {
        std::cerr << "Error loading graphics libraries: " << e.what() << std::endl;
    }
}
}  // namespace Arcade
