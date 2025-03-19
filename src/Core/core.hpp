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
    #include <vector>
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
    void loadAllLibrary();
    void printHelp();

 private:
    std::unique_ptr<IGameModule> _currentGame;
    std::unique_ptr<IDisplayModule> _currentDisplay;
    //! Contains all the library present in ./lib/ and stores them as string
    //! (they're path) first string is there type (Libe or Game)
    std::vector<std::pair<const std::string, const std::string>> _allLib;
};
}  // namespace Arcade
#endif  // SRC_CORE_CORE_HPP_
