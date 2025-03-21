/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Texture Modulme
*/

#ifndef SRC_SHARED_INTERFACE_DISPLAY_IEFFECTMODULE_HPP_
    #define SRC_SHARED_INTERFACE_DISPLAY_IEFFECTMODULE_HPP_
    #include <string>

namespace Arcade {
class IEffectModule {
 public:
    virtual ~IEffectModule() = default;
    virtual void applyEffect(const std::string &effectName) = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_DISPLAY_IEFFECTMODULE_HPP_
