/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Texture Modulme
*/

#ifndef SRC_SHARED_INTERFACE_DISPLAY_ITEXTUREMODULE_HPP_
    #define SRC_SHARED_INTERFACE_DISPLAY_ITEXTUREMODULE_HPP_
    #include <string>

namespace Arcade {
class TextureModule {
 public:
    virtual ~TextureModule() = default;
    virtual void drawText(int x, int y, const std::string &text) = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_DISPLAY_ITEXTUREMODULE_HPP_
