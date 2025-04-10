# Arcade Project: Creating Custom Libraries

This document explains how to extend the Arcade platform by implementing your own custom games, graphical libraries, or components.

## Tables of Contents

- 1.Architecture Overview
- 2.Implementing a Game Library
- 3.Implementing a Graphical Library
- 4.Implementing a Component Library
- 5.Building and Integrating Your Library

## Architecutre Overview

The Arcade project uses a modular architecture based on dynamic libraries and an Entity Component System (ECS). This design allows for:

- Loading games and graphical libraries at runtime
- Switching between different graphical libraries while playing the same game
- Creating reusable components that can be shared across games

Key interfaces in the architecture include:

- `IGameModule`: For implementing games
- `IDisplayModule`: For implementing graphical libraries
- `IComponent`: For implementing custom components

## Implementing a Game Library

To create a new game for Arcade, follow these steps:

1.Create a directory structure

```bash
src/Games/YourGame/
├── Components/           # Game-specific components
├── System/               # Game-specific systems
├── YourGame.cpp          # Main implementation
├── YourGame.hpp          # Main header
├── YourGameFactory.cpp   # Factory for creating game entities
├── YourGameFactory.hpp   # Factory header
└── CMakeLists.txt        # Build configuration
```

2.Create your game class header

```c++
#ifndef SRC_GAMES_YOURGAME_YOURGAME_HPP_
#define SRC_GAMES_YOURGAME_YOURGAME_HPP_

#include <memory>
#include <string>
#include "Shared/Interface/Game/IGameModule.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"

class YourGame : public Arcade::IGameModule {
 public:
    YourGame();
    ~YourGame() override = default;

    // Required interface methods
    void init(std::shared_ptr<Arcade::IEventManager> eventManager,
        std::shared_ptr<Arcade::IComponentManager> componentManager,
        std::shared_ptr<Arcade::IEntityManager> entityManager) override;
    void update() override;
    void stop() override;
    bool isGameOver() const override;
    bool hasWon() const override;
    std::string getSpecialCompSprite(size_t id) const override;

 private:
    // Your game-specific member variables
    std::shared_ptr<Arcade::IEventManager> _eventManager;
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    bool _gameOver;
    bool _hasWon;
};

#endif  // SRC_GAMES_YOURGAME_YOURGAME_HPP_
```

3.Implement your game logic

```c++
#include "Games/YourGame/YourGame.hpp"
#include <iostream>

YourGame::YourGame() : _gameOver(false), _hasWon(false) {}

void YourGame::init(std::shared_ptr<Arcade::IEventManager> eventManager,
    std::shared_ptr<Arcade::IComponentManager> componentManager,
    std::shared_ptr<Arcade::IEntityManager> entityManager) {
    _eventManager = eventManager;
    _componentManager = componentManager;
    _entityManager = entityManager;

    // Initialize your game entities and systems here
    // Example: Create game board, player, enemies, etc.
}

void YourGame::update() {
    // Update game state based on input and game rules
    // Process game logic, collision detection, etc.
}

void YourGame::stop() {
    // Clean up any resources
}

bool YourGame::isGameOver() const {
    return _gameOver;
}

bool YourGame::hasWon() const {
    return _hasWon;
}

std::string YourGame::getSpecialCompSprite(size_t id) const {
    // Return custom sprite paths for special entities if needed
    return "";
}

// Required entry point function
extern "C" {
    __attribute__((constructor))
    const char* init_yourgame(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_yourgame(void) {
    }

    Arcade::IArcadeModule* entryPoint(void) {
        return new YourGame();
    }
}
```

4.Create CMakeLists.txt for your game

```txt
set(NAME "arcade_yourgame")

set(SRC
    YourGame.cpp
    YourGameFactory.cpp
    Components/YourComponent1.cpp
    System/YourSystem1.cpp
)

add_library(${NAME} SHARED ${SRC})
target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(${NAME} PROPERTIES PREFIX "")

if(ENABLE_COVERAGE)
    target_compile_options(${NAME} PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(${NAME} PRIVATE arcade_ecs --coverage)
else()
    target_link_libraries(${NAME} PRIVATE arcade_ecs)
endif()
```

## Implementing a Graphical Library

To implement a new graphical library:

1.Create directory structure

```bash
src/Graphics/YourGraphicsLib/
├── YourGraphicsLibUtils/    # Helper classes for your library
├── YourGraphicsLib.cpp      # Main implementation
├── YourGraphicsLib.hpp      # Main header
├── YourGraphicsLibKeys.hpp  # Key mapping for your library
├── YourGraphicsLibColor.hpp # Color mapping
└── CMakeLists.txt           # Build configuration
```

2.Create your graphics library header

```c++
#ifndef SRC_GRAPHICS_YOURGRAPHICSLIB_YOURGRAPHICSLIB_HPP_
#define SRC_GRAPHICS_YOURGRAPHICSLIB_YOURGRAPHICSLIB_HPP_

#include <memory>
#include <string>
#include <utility>
#include "Interface/Display/IDisplayModule.hpp"
#include "Models/ColorType.hpp"

class YourGraphicsLibModule : public Arcade::IDisplayModule {
 private:
    std::string _name;
    int _windowWidth;
    int _windowHeight;
    bool _running;
    // Your library-specific members

 public:
    YourGraphicsLibModule() : _name("YourGraphicsLib"), _windowWidth(0), _windowHeight(0), _running(true) {}
    ~YourGraphicsLibModule() override;

    // Required interface methods
    void init(float width = 800.f, float height = 600.f) override;
    void stop() override;
    void clearScreen() override;
    void refreshScreen() override;
    void drawEntity(int x, int y, char symbol) override;
    void drawTexture(int x, int y, const std::string &textureId) override;
    void drawText(const std::string &text, int x, int y, Arcade::Color color) override;
    void pollEvents() override;
    bool isOpen() const override;
    const std::string& getName() const override;
    int getWidth() const override;
    int getHeight() const override;
    bool isKeyPressed(int keyCode) override;
    bool isMouseButtonPressed(int button) const override;
    std::pair<size_t, size_t> getMousePosition() const override;
};

#endif  // SRC_GRAPHICS_YOURGRAPHICSLIB_YOURGRAPHICSLIB_HPP_
```

3.Implement your graphics library

```c++
#include <stdexcept>
#include <memory>
#include <string>
#include <iostream>
#include "YourGraphicsLib/YourGraphicsLib.hpp"
#include "Interface/IArcadeModule.hpp"

YourGraphicsLibModule::~YourGraphicsLibModule() {
    // Clean up resources
}

void YourGraphicsLibModule::init(float x, float y) {
    int width = static_cast<int>(x);
    int height = static_cast<int>(y);

    // Initialize your graphics library here
    // Create window, init renderer, etc.

    _windowWidth = width;
    _windowHeight = height;
}

void YourGraphicsLibModule::stop() {
    // Cleanup before shutdown
}

void YourGraphicsLibModule::clearScreen() {
    // Clear screen for next frame
}

void YourGraphicsLibModule::refreshScreen() {
    // Present rendered content to screen
}

void YourGraphicsLibModule::drawEntity(int x, int y, char symbol) {
    // Draw a character at position x,y
}

void YourGraphicsLibModule::drawTexture(int x, int y, const std::string& texturePath) {
    // Load and draw a texture at position x,y
}

void YourGraphicsLibModule::drawText(const std::string& text, int x, int y, Arcade::Color color) {
    // Render text with specified color at position x,y
}

void YourGraphicsLibModule::pollEvents() {
    // Check for window events (close, resize, etc.)
}

bool YourGraphicsLibModule::isOpen() const {
    return _running;
}

const std::string& YourGraphicsLibModule::getName() const {
    return _name;
}

int YourGraphicsLibModule::getWidth() const {
    return _windowWidth;
}

int YourGraphicsLibModule::getHeight() const {
    return _windowHeight;
}

bool YourGraphicsLibModule::isKeyPressed(int keyCode) {
    // Check if a key is pressed
    return false;
}

bool YourGraphicsLibModule::isMouseButtonPressed(int button) const {
    // Check if a mouse button is pressed
    return false;
}

std::pair<size_t, size_t> YourGraphicsLibModule::getMousePosition() const {
    // Get current mouse position
    return {0, 0};
}

// Required entry point function
extern "C" {
    __attribute__((constructor))
    const char* init_yourgraphicslib(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_yourgraphicslib(void) {
    }

    Arcade::IArcadeModule* entryPoint(void) {
        return new YourGraphicsLibModule();
    }
}
```

4.Create color mapping for your library

```c++
#ifndef SRC_GRAPHICS_YOURGRAPHICSLIB_YOURGRAPHICSLIBCOLOR_HPP_
#define SRC_GRAPHICS_YOURGRAPHICSLIB_YOURGRAPHICSLIBCOLOR_HPP_

#include "Models/ColorType.hpp"

class YourGraphicsLibColor {
 public:
    static YourColorType convertColor(Arcade::Color color) {
        switch (color) {
            case Arcade::Color::RED:
                return YOUR_RED_COLOR;
            case Arcade::Color::GREEN:
                return YOUR_GREEN_COLOR;
            // Handle other colors
            default:
                return YOUR_DEFAULT_COLOR;
        }
    }
};

#endif
```

5.Create CMakeLists.txt for your graphics library

```txt
find_package(YourGraphicsLibDependency REQUIRED)

set(UTILS_SRC
    YourGraphicsLibUtils/YourUtilClass1.cpp
    YourGraphicsLibUtils/YourUtilClass2.cpp
)

add_library(arcade_yourgraphicslib SHARED
    YourGraphicsLib.cpp
    ${UTILS_SRC}
)

set_target_properties(arcade_yourgraphicslib PROPERTIES PREFIX "" OUTPUT_NAME "arcade_yourgraphicslib")
target_include_directories(arcade_yourgraphicslib PUBLIC
    ${YOURGRAPHICSLIB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_ROOT}/src/Shared
)

if(ENABLE_COVERAGE)
    target_compile_options(arcade_yourgraphicslib PUBLIC -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_yourgraphicslib PUBLIC ${YOURGRAPHICSLIB_LIBRARIES} arcade_ecs --coverage)
else()
    target_link_libraries(arcade_yourgraphicslib PUBLIC ${YOURGRAPHICSLIB_LIBRARIES} arcade_ecs)
endif()
```

## Implementing a Component Library

To create a custom component:

1.Create directory structure

```bash
src/ECS/Components/YourComponent/
├── YourComponent.cpp
├── YourComponent.hpp
└── CMakeLists.txt
```

2.Create your component header

```c++
#ifndef SRC_ECS_COMPONENTS_YOURCOMPONENT_YOURCOMPONENT_HPP_
#define SRC_ECS_COMPONENTS_YOURCOMPONENT_YOURCOMPONENT_HPP_

#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Models/ComponentType.hpp"

class YourComponent : public Arcade::IComponent {
 public:
    // Constructor with necessary parameters
    YourComponent(/* your parameters */);

    // Required method to get component type
    ComponentType getType() const override;

    // Your component data members
    // Example: position, health, damage, etc.

 private:
    // Private member variables
};

#endif  // SRC_ECS_COMPONENTS_YOURCOMPONENT_YOURCOMPONENT_HPP_
```

3.Implement your component

```c++
#include "ECS/Components/YourComponent/YourComponent.hpp"
#include "Shared/Models/ComponentType.hpp"

YourComponent::YourComponent(/* your parameters */) {
    // Initialize your component
}

ComponentType YourComponent::getType() const {
    // Return an existing component type or use CUSTOM_BASE + your value
    return static_cast<ComponentType>(ComponentType::CUSTOM_BASE + 1);
}

// Required entry point function
extern "C" {
    __attribute__((constructor))
    const char *init_yourcomponent(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_yourcomponent(void) {
    }

    Arcade::IArcadeModule* entryPoint(void) {
        return new YourComponent(/* default parameters */);
    }
}
```

4.Create CMakeLists.txt for your component

```txt
add_library(arcade_yourcomponent SHARED YourComponent.cpp)

set_target_properties(arcade_yourcomponent PROPERTIES PREFIX "" OUTPUT_NAME "arcade_yourcomponent")
target_include_directories(arcade_yourcomponent PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_ROOT}/src/Shared)
if(ENABLE_COVERAGE)
    target_compile_options(arcade_yourcomponent PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_yourcomponent PRIVATE --coverage)
else ()
    target_link_libraries(arcade_yourcomponent PRIVATE)
endif()
```

## Building and Integrating Your Library

After creating your library, follow these steps to integrate it into the Arcade platform:

1. Update the main CMakeLists.txt
For a game:

```txt
# Add to src/Games/CMakeLists.txt
add_subdirectory(YourGame)
```txt
For a graphical library:

```txt
# Add to src/Graphics/CMakeLists.txt
add_subdirectory(YourGraphicsLib)
```

For a component:

```txt
# Add to src/ECS/CMakeLists.txt
add_subdirectory(Components/YourComponent)
```

2.Build your library

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

3.Run Arcade with your library

```bash
# Run with your graphics library
./arcade ./lib/arcade_yourgraphicslib.so
```

## Tips and Best Practices

1.Study Existing Implementations: Look at existing games (Minesweeper) and graphics libraries (SFML, SDL) for examples.

2.Handle Resources Properly: Always free resources in the destructor and be careful with memory management.

3.Error Handling: Use appropriate error handling (try/catch blocks, error checking) to ensure your library doesn't crash the entire application.

4.Follow Naming Conventions: Keep consistent with the existing codebase for better integration.

5.Document Your Code: Add comments and documentation to make your code more maintainable.

By following these guidelines, you should be able to create your own games, graphical libraries, or components for the Arcade platform.
