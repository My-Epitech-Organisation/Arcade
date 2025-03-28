// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GameLoop Utils
*/

#include <iostream>
#include "GameLoop/GameLoop.hpp"
#include "EventManager/KeyEvent/KeyEvent.hpp"
#include "Models/EventType.hpp"

namespace Arcade {

void GameLoop::subscribeEvents() {
    subscribeNum1Event();
    subscribeNum2Event();
    subscribeNum3Event();
    subscribeNum4Event();
}

void GameLoop::subscribeNum1Event() {
    KeyEvent num1Event(Keys::NUM1, EventType::KEY_PRESSED);
    _eventManager->subscribe(num1Event, [this]() {
        if (_state == MAIN_MENU && !_gameLibs.empty()) {
            loadAndStartGame();
        }
    });
}

void GameLoop::subscribeNum2Event() {
    KeyEvent num2Event(Keys::NUM2, EventType::KEY_PRESSED);
    _eventManager->subscribe(num2Event, [this]() {
        if (_state == MAIN_MENU) {
            _state = GRAPHICS_SELECTION;
        }
    });
}

void GameLoop::subscribeNum3Event() {
    KeyEvent num3Event(Keys::NUM3, EventType::KEY_PRESSED);
    _eventManager->subscribe(num3Event, [this]() {
        if (_state == MAIN_MENU) {
            _state = GAME_SELECTION;
        }
    });
}

void GameLoop::subscribeNum4Event() {
    KeyEvent num4Event(Keys::NUM4, EventType::KEY_PRESSED);
    _eventManager->subscribe(num4Event, [this]() {
        if (_state == MAIN_MENU) {
            _window->closeWindow();
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
    _eventManager->subscribe(upEvent, [this]() {
        if (_state == GAME_SELECTION && _selectedGame > 0) {
            _selectedGame--;
        } else if (_state == GRAPHICS_SELECTION && _selectedGraphics > 0) {
            _selectedGraphics--;
        }
    });
}

void GameLoop::subscribeDownEvent() {
    KeyEvent downEvent(Keys::DOWN, EventType::KEY_PRESSED);
    _eventManager->subscribe(downEvent, [this]() {
        if (_state == GAME_SELECTION && _selectedGame < _gameLibs.size() - 1) {
            _selectedGame++;
        } else if (_state == GRAPHICS_SELECTION
            && _selectedGraphics < _graphicsLibs.size() - 1) {
            _selectedGraphics++;
        }
    });
}

void GameLoop::subscribeEnterEvent() {
    KeyEvent enterEvent(Keys::ENTER, EventType::KEY_PRESSED);
    _eventManager->subscribe(enterEvent, [this]() {
        if (_state == GAME_SELECTION && !_gameLibs.empty()) {
            loadAndStartGame();
        } else if (_state == GRAPHICS_SELECTION && !_graphicsLibs.empty()) {
            loadGraphicsLibraries();
        }
    });
}

void GameLoop::subscribeEscEvent() {
    KeyEvent escEvent(Keys::ESC, EventType::KEY_PRESSED);
    _eventManager->subscribe(escEvent, [this]() {
        if (_state == GAME_SELECTION || _state == GRAPHICS_SELECTION) {
            _state = MAIN_MENU;
        } else if (_state == GAME_PLAYING) {
            _state = MAIN_MENU;
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
    _eventManager->subscribe(rightClickEvent, [this]() {
        if (_state == GAME_SELECTION || _state == GRAPHICS_SELECTION) {
            _state = MAIN_MENU;
        } else if (_state == GAME_PLAYING) {
            _state = MAIN_MENU;
        }
    });
}

void GameLoop::subscribeMouseMoveEvent() {
    MouseEvent mouseMoveEvent(MouseButton::NONE, EventType::MOUSE_MOVED, 0, 0);
    _eventManager->subscribe(mouseMoveEvent, [this]() {
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
    for (size_t i = 0; i < _graphicsLibs.size(); i++) {
        if (x >= centerX - 100 && x <= centerX + 100 &&
            y >= MENU_START_Y + i * MENU_ITEM_HEIGHT &&
            y <= MENU_START_Y + (i + 1) * MENU_ITEM_HEIGHT) {
            _selectedGraphics = i;
        }
    }
}

void GameLoop::subscribeLeftClickEvent() {
    MouseEvent leftClickEvent(MouseButton::LEFT,
        EventType::MOUSE_BUTTON_PRESSED, 0, 0);
    _eventManager->subscribe(leftClickEvent, [this]() {
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
    bool clickedOnGraphics = false;
    for (size_t i = 0; i < _graphicsLibs.size(); i++) {
        if (x >= centerX - 100 && x <= centerX + 100 &&
            y >= MENU_START_Y + i * MENU_ITEM_HEIGHT &&
            y <= MENU_START_Y + (i + 1) * MENU_ITEM_HEIGHT) {
            _selectedGraphics = i;
            clickedOnGraphics = true;
            loadGraphicsLibraries();
            break;
        }
    }
    if (!clickedOnGraphics && y >= MENU_START_Y
        + _graphicsLibs.size() * MENU_ITEM_HEIGHT + 40) {
        _state = MAIN_MENU;
    }
}

}  // namespace Arcade
