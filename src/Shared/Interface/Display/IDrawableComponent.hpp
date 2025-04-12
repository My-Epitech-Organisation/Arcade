// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400-NAN-4-1-arcade-marin.lamy
** File description:
** IDrawableComponent Interface
*/

#ifndef SRC_SHARED_INTERFACE_DISPLAY_IDRAWABLECOMPONENT_HPP_
#define SRC_SHARED_INTERFACE_DISPLAY_IDRAWABLECOMPONENT_HPP_

#include <string>
#include "Interface/ECS/IComponent.hpp"
#include "Models/ComponentType.hpp"
#include "Models/ColorType.hpp"

namespace Arcade {

class IDrawableComponent : public IComponent {
 public:
    virtual ~IDrawableComponent() = default;

    // Setup methods
    virtual void setAsTexture(const std::string& texturePath,
        float w, float h) = 0;
    virtual void setAsText(const std::string& textContent,
        const std::string& fontPath, float fontSize) = 0;
    virtual void setAsCharacter(char c) = 0;

    // Rendering state checks
    virtual bool shouldRenderAsText() const = 0;
    virtual bool shouldRenderAsTexture() const = 0;
    virtual bool shouldRenderAsCharacter() const = 0;

    // Property accessors
    virtual std::string getPath() const = 0;
    virtual std::string getFont() const = 0;
    virtual std::string getText() const = 0;
    virtual char getCharacter() const = 0;
    virtual Color getColor() const = 0;
    virtual float getScale() const = 0;
    virtual float getPositionX() const = 0;
    virtual float getPositionY() const = 0;
    virtual float getRotation() const = 0;
    virtual bool isRenderable() const = 0;
    virtual float getWidth() const = 0;
    virtual float getHeight() const = 0;

    // Property mutators
    virtual void setPath(const std::string& path) = 0;
    virtual void setFont(const std::string& font) = 0;
    virtual void setText(const std::string& text) = 0;
    virtual void setCharacter(char c) = 0;
    virtual void setColor(const Color& color) = 0;
    virtual void setScale(float scale) = 0;
    virtual void setPosition(float x, float y) = 0;
    virtual void setRotation(float rotation) = 0;
    virtual void setVisibility(bool visible) = 0;
    virtual void setDimensions(float width, float height) = 0;
};

}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_DISPLAY_IDRAWABLECOMPONENT_HPP_
