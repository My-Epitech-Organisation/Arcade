// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** TextComponent
*/

#ifndef SRC_ECS_COMPONENTS_TEXT_TEXTCOMPONENT_HPP_
    #define SRC_ECS_COMPONENTS_TEXT_TEXTCOMPONENT_HPP_
    #include <string>
    #include <memory>
    #include "Shared/Interface/ECS/IComponent.hpp"
    #include "Shared/Models/ComponentType.hpp"
    #include "Shared/Models/ColorType.hpp"

class TextComponent : public Arcade::IComponent {
 public:
    TextComponent(const std::string& text, int x, int y, Arcade::Color color)
        : text(text), x(x), y(y), color(color), visible(true) {}

    ComponentType getType() const override;

    std::string text;
    int x;
    int y;
    Arcade::Color color;
    bool visible;
};

#endif  // SRC_ECS_COMPONENTS_TEXT_TEXTCOMPONENT_HPP_
