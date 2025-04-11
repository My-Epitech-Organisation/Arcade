// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** TextComponent
*/
#include "ECS/Components/Text/TextComponent.hpp"
#include "Shared/Models/ComponentType.hpp"
#include "Shared/Models/ModuleInfos.hpp"

ComponentType TextComponent::getType() const {
    return ComponentType::TEXT;
}

extern "C" {
    __attribute__((constructor))
    const char *init_text(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_ncurses(void) {
    }

    Arcade::IArcadeModule* entryPoint(void) {
        return new TextComponent("Default Text", 0, 0, Arcade::Color::WHITE);
    }

    Arcade::ModuleInfos module_infos() {
        return {"TextComponent", "IDK",
            "IDK",
            "./lib/arcade_text.so", Arcade::ModuleType::GAME_COMPONENT};
    }
}
