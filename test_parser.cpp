#include <iostream>
#include "Shared/JSONParser/JSONParser.hpp"

int main() {
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
        return 1;
    }
    
    return 0;
}