// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400-NAN-4-1-arcade-samuel.tesson
** File description:
** DrawableComponent
*/

#ifndef SRC_ECS_COMPONENTS_DRAWABLE_DRAWABLECOMPONENT_HPP_
#define SRC_ECS_COMPONENTS_DRAWABLE_DRAWABLECOMPONENT_HPP_

#include <string>
#include "Interface/ECS/IComponent.hpp"
#include "Interface/Display/IDrawableComponent.hpp"
#include "Models/ComponentType.hpp"
#include "Models/ColorType.hpp"

namespace Arcade {

class DrawableComponent : public IDrawableComponent {
 public:
    DrawableComponent();
    ~DrawableComponent() override = default;
    ComponentType getType() const override;

    // Setup methods implementation
    void setAsTexture(const std::string& texturePath,
        float w, float h) override;
    void setAsText(const std::string& textContent,
        const std::string& fontPath, float fontSize) override;
    void setAsCharacter(char c) override;

    // Rendering state checks implementation
    bool shouldRenderAsText() const override;
    bool shouldRenderAsTexture() const override;
    bool shouldRenderAsCharacter() const override;

    // Property accessors implementation
    std::string getPath() const override;
    std::string getFont() const override;
    std::string getText() const override;
    char getCharacter() const override;
    Color getColor() const override;
    float getScale() const override;
    float getPositionX() const override;
    float getPositionY() const override;
    float getRotation() const override;
    bool isRenderable() const override;
    float getWidth() const override;
    float getHeight() const override;

    // Property mutators implementation
    void setPath(const std::string& path) override;
    void setFont(const std::string& font) override;
    void setText(const std::string& text) override;
    void setCharacter(char c) override;
    void setColor(const Color& color) override;
    void setScale(float scale) override;
    void setPosition(float x, float y) override;
    void setRotation(float rotation) override;
    void setVisibility(bool visible) override;
    void setDimensions(float width, float height) override;

 private:
    std::string path;       // Path to the resource file (image, texture, etc.)
    std::string font;       // Font to use for text elements
    std::string text;       // Text content to display when isText is true
    bool isText;            // Flag to determine if the element is text
    char character;  // Character to display (for text or tile-based graphics)
    Color color;            // Color of the element
    float scale;            // Scale factor for the element
    float posX;             // X position on screen
    float posY;             // Y position on screen
    float rotation;         // Rotation angle in degrees
    bool isVisible;    // Flag to determine if the element should be rendered
    float width;            // Width of the element
    float height;           // Height of the element
};

}  // namespace Arcade

#endif  // SRC_ECS_COMPONENTS_DRAWABLE_DRAWABLECOMPONENT_HPP_
