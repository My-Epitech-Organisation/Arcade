// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Main entry point for Arcade
*/

#include <iostream>
#include <string>
#include <exception>
#include "GameLoop/GameLoop.hpp"

void printUsage(const std::string& programName) {
    std::cerr << "Usage: " << programName
        << " <graphics_library_path>" << std::endl;
    std::cerr << "Example: " << programName
        << " ./lib/arcade_sdl2_graphics.so" << std::endl;
}

int main(int argc, char *argv[]) {
    try {
        if (argc < 2) {
            printUsage(argv[0]);
            return 84;
        }

        std::string graphicsLibPath = argv[1];
        Arcade::GameLoop gameLoop(graphicsLibPath);
        gameLoop.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 84;
    }
}
