// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400-NAN-4-1-arcade-marin.lamy
** File description:
** DrawableComponent implementation
*/

#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include <string>

namespace Arcade {

DrawableComponent::DrawableComponent()
: path(""), font("./assets/fonts/Arial.ttf"),
text(""), isText(false), character('\0'),
color(Arcade::BLACK), scale(1.0f), rotation(0.0f),
isVisible(true), width(0.0f), height(0.0f) {}

ComponentType DrawableComponent::getType() const {
    return ComponentType::DRAWABLE;
}

void DrawableComponent::setAsTexture
(const std::string& texturePath, float w, float h) {
    path = texturePath;
    isText = false;
    text = "";     // Clear any text content
    width = w;
    height = h;
}

void DrawableComponent::setAsText(const std::string& textContent,
const std::string& fontPath, float fontSize) {
    text = textContent;
    font = fontPath;
    isText = true;  // Explicitly set to true for text
    path = "";      // Clear any texture path to avoid confusion
    scale = fontSize;
}

void DrawableComponent::setAsCharacter(char c) {
    character = c;
    isText = false;
    text = "";     // Clear any text content
}

bool DrawableComponent::shouldRenderAsText() const {
    return isText && !text.empty();
}

bool DrawableComponent::shouldRenderAsTexture() const {
    return !isText && !path.empty();
}

bool DrawableComponent::shouldRenderAsCharacter() const {
    return !isText && character != '\0';
}

extern "C" {
    __attribute__((constructor))
    const char *init_position(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_ncurses(void) {
    }

    Arcade::IArcadeModule* entryPoint(void) {
        return new DrawableComponent();
    }
}
}  // namespace Arcade
