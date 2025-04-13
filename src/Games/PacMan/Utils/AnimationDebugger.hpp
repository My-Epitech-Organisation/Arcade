// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Utility for debugging animation issues
*/

#ifndef SRC_GAMES_PACMAN_UTILS_ANIMATIONDEBUGGER_HPP_
#define SRC_GAMES_PACMAN_UTILS_ANIMATIONDEBUGGER_HPP_

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace PacMan {

class AnimationDebugger {
public:
    static void dumpAssetKeys(const std::map<std::string, DrawableComponent>& assets) {
        std::ofstream file("pacman_assets.log");
        if (!file.is_open()) {
            std::cerr << "Failed to open debug log file" << std::endl;
            return;
        }
        
        file << "Available asset keys (" << assets.size() << " total):" << std::endl;
        
        // Group by categories for easier reading
        std::vector<std::string> pacmanKeys;
        std::vector<std::string> ghostKeys;
        std::vector<std::string> mapKeys;
        std::vector<std::string> otherKeys;
        
        for (const auto& [key, _] : assets) {
            if (key.find("pacman") != std::string::npos) {
                pacmanKeys.push_back(key);
            } else if (key.find("ghost") != std::string::npos) {
                ghostKeys.push_back(key);
            } else if (key.find("map") != std::string::npos) {
                mapKeys.push_back(key);
            } else {
                otherKeys.push_back(key);
            }
        }
        
        // Sort each category
        std::sort(pacmanKeys.begin(), pacmanKeys.end());
        std::sort(ghostKeys.begin(), ghostKeys.end());
        std::sort(mapKeys.begin(), mapKeys.end());
        std::sort(otherKeys.begin(), otherKeys.end());
        
        // Write to file
        file << "\n=== PACMAN KEYS (" << pacmanKeys.size() << ") ===" << std::endl;
        for (const auto& key : pacmanKeys) {
            file << key << std::endl;
        }
        
        file << "\n=== GHOST KEYS (" << ghostKeys.size() << ") ===" << std::endl;
        for (const auto& key : ghostKeys) {
            file << key << std::endl;
        }
        
        file << "\n=== MAP KEYS (" << mapKeys.size() << ") ===" << std::endl;
        for (const auto& key : mapKeys) {
            file << key << std::endl;
        }
        
        file << "\n=== OTHER KEYS (" << otherKeys.size() << ") ===" << std::endl;
        for (const auto& key : otherKeys) {
            file << key << std::endl;
        }
        
        file.close();
        std::cout << "Asset keys dumped to pacman_assets.log" << std::endl;
        
        // Also print to console for immediate feedback
        std::cout << "Pacman keys:" << std::endl;
        for (const auto& key : pacmanKeys) {
            std::cout << " - " << key << std::endl;
        }
    }
    
    static std::string getDirectionalSpriteAlternative(const PacmanComponent* pacman) {
        // Basic closed mouth sprite (frame 0) is the same for all directions
        if (pacman->getAnimationFrame() == 0 || !pacman->isMoving()) {
            return "pacman.default";
        }
        
        // Alternative key formats to try
        std::string dir;
        switch (pacman->getCurrentDirection()) {
            case Direction::RIGHT: dir = "right"; break;
            case Direction::LEFT: dir = "left"; break;
            case Direction::UP: dir = "up"; break;
            case Direction::DOWN: dir = "down"; break;
            default: return "pacman.default";
        }
        
        int frame = pacman->getAnimationFrame() - 1; // 0-indexed
        
        // Try different formats - add more as needed
        std::vector<std::string> formats = {
            "pacman." + dir + "[" + std::to_string(frame) + "]",
            "pacman." + dir + "." + std::to_string(frame + 1),
            "pacman." + dir + std::to_string(frame + 1),
            "pacman_" + dir + "_" + std::to_string(frame + 1),
            "textures.pacman." + dir + "[" + std::to_string(frame) + "]"
        };
        
        return formats[0]; // Return the first format by default
    }
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_UTILS_ANIMATIONDEBUGGER_HPP_
