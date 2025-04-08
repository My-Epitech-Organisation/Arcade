// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** IMenu
*/

#ifndef SRC_SHARED_INTERFACE_CORE_IMENU_HPP_
    #define SRC_SHARED_INTERFACE_CORE_IMENU_HPP_
    #include <vector>
    #include <string>
    #include <memory>
    #include "Interface/Core/IWindowModule.hpp"

namespace Arcade {

class IMenu {
 public:
    virtual ~IMenu() = default;

    virtual void displayMainMenu(const std::vector<std::string> &graphicsLibs,
        const std::vector<std::string> &gameLibs, size_t selectedGraphics,
            size_t selectedGame) = 0;

    virtual void displayGameSelection(const std::vector<std::string> &gameLibs,
        size_t selectedGame) = 0;

    virtual void displayGraphicsSelection(const std::vector<std::string>
        &graphicsLibs, size_t selectedGraphics) = 0;

    virtual void setWindow(std::shared_ptr<IWindowModule> window) = 0;
};

}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_CORE_IMENU_HPP_
