// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** core
*/

#ifndef SRC_CORE_CORE_HPP_
    #define SRC_CORE_CORE_HPP_
    #include <memory>
    #include <string>
    #include "../Shared/Interface/IGameModule.hpp"
    #include "../Shared/Interface/IDisplayModule.hpp"

namespace Arcade {
class Core {
 public:
    Core() : _currentGame(nullptr), _currentDisplay(nullptr) {}
    ~Core() {}
    void run();
    void loadGame(const std::string &gameName);
    void loadDisplay(const std::string &displayName);
    void unloadGame();
    void unloadDisplay();
    void printHelp();

 private:
    std::unique_ptr<IGameModule> _currentGame;
    std::unique_ptr<IDisplayModule> _currentDisplay;
};
}  // namespace Arcade
#endif  // SRC_CORE_CORE_HPP_
