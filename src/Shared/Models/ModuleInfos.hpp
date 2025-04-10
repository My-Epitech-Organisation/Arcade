// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** arcade [WSL: Ubuntu]
** File description:
** ModuleInfos
*/

#ifndef SRC_SHARED_MODELS_MODULEINFOS_HPP_
    #define SRC_SHARED_MODELS_MODULEINFOS_HPP_
    #include <string>

namespace Arcade {

enum class ModuleType {
    GAME,
    GRAPHIC_LIB,
    GAME_COMPONENT,
};

struct ModuleInfos {
    std::string name;
    std::string description;
    std::string picturePath;
    std::string path;
    ModuleType type;
};
}  // namespace Arcade

#endif  // SRC_SHARED_MODELS_MODULEINFOS_HPP_
