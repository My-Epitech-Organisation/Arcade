// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** ITextModule
*/

#ifndef SRC_SHARED_INTERFACE_DISPLAY_ITEXTMODULE_HPP_
    #define SRC_SHARED_INTERFACE_DISPLAY_ITEXTMODULE_HPP_
    #include <string>

namespace Arcade {
class ITextModule {
 public:
    virtual ~ITextModule() = default;
    virtual void drawText(int x, int y, const std::string &text) = 0;
};
}  // namespace Arcade
#endif  // SRC_SHARED_INTERFACE_DISPLAY_ITEXTMODULE_HPP_
