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
cmake .. -DENABLE_CORE=ON
cmake --build .
```

## Collaboration team

### Pixel Computer Club

- Samuel Tesson (<samuel.tesson@epitech.eu>)
- Clément Talneau (<clement.talneau@epitech.eu>)
- Paul-Antoine Salmon (<paul-antoine.salmon@epitech.eu>)

### Neo-Score Duelist Arcade Players

- Marin Lamy (<marin.lamy@epitech.eu>)
- Tristan Masse (<tristan.masse@epitech.eu>)
- Santiago Pidcova (<santiago.pidcova@epitech.eu>)
