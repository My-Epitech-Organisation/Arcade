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
#include "Shared/Exceptions/Exceptions.hpp"
#include "Shared/JSONParser/JSONParser.hpp"

void printUsage(const std::string& programName) {
    std::cerr << "Usage: " << programName
        << " <graphics_library_path>" << std::endl;
    std::cerr << "Example: " << programName
        << " ./lib/arcade_sdl2_graphics.so" << std::endl;
}

void test_parser(const std::string& jsonFilePath) {
    Arcade::JSONParser parser;
    
    try {
        // Convert minesweeper.json to graphical elements
        auto elements = parser.convertAssetsToGraphicalElements("config/minesweeper.json");
        
        // Print all loaded elements
        std::cout << "Loaded " << elements.size() << " graphical elements:" << std::endl;
        for (const auto& [name, element] : elements) {
            std::cout << "Element: " << name << std::endl;
            std::cout << "  Path: " << element.path << std::endl;
            std::cout << "  Character: '" << element.character << "'" << std::endl;
            std::cout << "----------------------------" << std::endl;
        }
        
        // Access specific elements
        if (elements.count("minesweeper.flag") > 0) {
            auto& flag = elements["minesweeper.flag"];
            std::cout << "Flag character: " << flag.character << std::endl;
        }
        
        if (elements.count("minesweeper.numbers.1") > 0) {
            auto& number1 = elements["minesweeper.numbers.1"];
            std::cout << "Number 1 path: " << number1.path << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    try {
        if (argc < 2) {
            printUsage(argv[0]);
            return 84;
        }

        std::string graphicsLibPath = argv[1];
        test_parser("config/minesweeper.json");
        Arcade::GameLoop gameLoop(graphicsLibPath);
        gameLoop.run();
        return 0;
    } catch (const Arcade::LibraryLoadException& e) {
        std::cerr << "Library loading error: " << e.what() << std::endl;
        return 84;
    } catch (const Arcade::GraphicsException& e) {
        std::cerr << "Graphics error: " << e.what() << std::endl;
        return 84;
    } catch (const Arcade::GameException& e) {
        std::cerr << "Game error: " << e.what() << std::endl;
        return 84;
    } catch (const Arcade::InputException& e) {
        std::cerr << "Input handling error: " << e.what() << std::endl;
        return 84;
    } catch (const Arcade::ResourceException& e) {
        std::cerr << "Resources error: " << e.what() << std::endl;
        return 84;
    } catch (const Arcade::ArcadeException& e) {
        std::cerr << "Arcade error: " << e.what() << std::endl;
        return 84;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 84;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 84;
    }
}
