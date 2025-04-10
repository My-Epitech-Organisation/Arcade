# Arcade - A Retro Gaming Platform

## Overview

Arcade is a gaming platform that allows users to choose from multiple games and dynamically load different graphical libraries. The project is designed to be modular and extensible, making it easy to integrate new games and graphical interfaces.

## Features

- Dynamically loadable graphical libraries
- Support for multiple games
- Score tracking
- Modular architecture with shared libraries

## Installation

### Prerequisites

- C++ Compiler (g++)
- Make or CMake
- SDL2, ncurses, and any additional required libraries

### Build Instructions

Using **CMake**:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Adding New Games or Graphical Libraries

To add a new game or graphical library, follow these steps:

1. Create a new directory for your game or graphical library in the `src/Games` or `src/Graphics` directory, respectively.
2. Implement the required interfaces defined in the `src/Shared` directory.
3. Update the `CMakeLists.txt` files to include your new game or graphical library.
4. Rebuild the project using the build instructions above.

## Documentation

The project documentation is available in the `docs` directory. You can find the class diagrams, annotated source code, and other relevant documentation there.

## Collaboration team

### Pixel Computer Club

- Samuel Tesson (<samuel.tesson@epitech.eu>)
- Clément Talneau (<clement.talneau@epitech.eu>)
- Paul-Antoine Salmon (<paul-antoine.salmon@epitech.eu>)

### Neo-Score Duelist Arcade Players

- Marin Lamy (<marin.lamy@epitech.eu>)
- Tristan Masse (<tristan.masse@epitech.eu>)
- Santiago Pidcova (<santiago.pidcova@epitech.eu>)
