// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400-NAN-4-1-arcade-marin.lamy
** File description:
** DrawableComponent
*/

#ifndef SRC_ECS_COMPONENTS_DRAWABLE_DRAWABLECOMPONENT_HPP_
#define SRC_ECS_COMPONENTS_DRAWABLE_DRAWABLECOMPONENT_HPP_

#include <string>
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Models/ComponentType.hpp"
#include "Shared/Models/ColorType.hpp"

namespace Arcade {

class DrawableComponent : public IComponent {
 public:
    DrawableComponent()
        : path(""), font("./assets/fonts/Arial.ttf"),
          text(""), isText(false), character('\0'),
          color(Arcade::BLACK), scale(1.0f), rotation(0.0f),
          isVisible(true), width(0.0f), height(0.0f) {}
    ~DrawableComponent() override = default;
    ComponentType getType() const override { return ComponentType::DRAWABLE; }
    // Helper methods to set up common configurations
    void setAsTexture(const std::string& texturePath, float w, float h) {
        path = texturePath;
        isText = false;
        text = "";     // Clear any text content
        width = w;
        height = h;
    }
    void setAsText(const std::string& textContent,
        const std::string& fontPath, float fontSize) {
        text = textContent;
        font = fontPath;
        isText = true;  // Explicitly set to true for text
        path = "";      // Clear any texture path to avoid confusion
        scale = fontSize;
    }
    void setAsCharacter(char c) {
        character = c;
        isText = false;
        text = "";     // Clear any text content
    }
    // Check if this component should be rendered as text
    bool shouldRenderAsText() const {
        return isText && !text.empty();
    }
    // Check if this component should be rendered as a texture
    bool shouldRenderAsTexture() const {
        return !isText && !path.empty();
    }
    // Check if this component should be rendered as a character
    bool shouldRenderAsCharacter() const {
        return !isText && character != '\0';
    }
    std::string path;       // Path to the resource file (image, texture, etc.)
    std::string font;       // Font to use for text elements
    std::string text;       // Text content to display when isText is true
    bool isText;            // Flag to determine if the element is text
    char character;   // Character to display (for text or tile-based graphics)
    Color color;            // Color of the element
    float scale;            // Scale factor for the element
    float posX;             // X position on screen
    float posY;             // Y position on screen
    float rotation;         // Rotation angle in degrees
    bool isVisible;   // Flag to determine if the element should be rendered
    float width;            // Width of the element
    float height;           // Height of the element
};

}  // namespace Arcade

#endif  // SRC_ECS_COMPONENTS_DRAWABLE_DRAWABLECOMPONENT_HPP_

