// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Window Module Interface
*/

#ifndef SRC_SHARED_INTERFACE_CORE_IWINDOWMODULE_HPP_
    #define SRC_SHARED_INTERFACE_CORE_IWINDOWMODULE_HPP_
    #include <string>

namespace Arcade {
class IWindowModule {
 public:
    virtual ~IWindowModule() = default;
    virtual void createWindow(int width, int height,
        const std::string &title) = 0;
    virtual void resizeWindow(int width, int height) = 0;
    virtual void closeWindow() = 0;
    virtual bool isWindowOpen() const = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_CORE_IWINDOWMODULE_HPP_
