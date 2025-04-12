// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400-NAN-4-1-arcade-marin.lamy
** File description:
** DrawableComponent implementation
*/

#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "Shared/Models/ModuleInfos.hpp"
#include <string>

namespace Arcade {

DrawableComponent::DrawableComponent()
: path(""), font("./assets/fonts/Arial.ttf"),
text(""), isText(false), character('\0'),
color(Arcade::BLACK), scale(1.0f), posX(0.0f), posY(0.0f), rotation(0.0f),
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

// Property accessors implementation
std::string DrawableComponent::getPath() const {
    return path;
}

std::string DrawableComponent::getFont() const {
    return font;
}

std::string DrawableComponent::getText() const {
    return text;
}

char DrawableComponent::getCharacter() const {
    return character;
}

Color DrawableComponent::getColor() const {
    return color;
}

float DrawableComponent::getScale() const {
    return scale;
}

float DrawableComponent::getPositionX() const {
    return posX;
}

float DrawableComponent::getPositionY() const {
    return posY;
}

float DrawableComponent::getRotation() const {
    return rotation;
}

bool DrawableComponent::isRenderable() const {
    return isVisible;
}

float DrawableComponent::getWidth() const {
    return width;
}

float DrawableComponent::getHeight() const {
    return height;
}

// Property mutators implementation
void DrawableComponent::setPath(const std::string& newPath) {
    path = newPath;
}

void DrawableComponent::setFont(const std::string& newFont) {
    font = newFont;
}

void DrawableComponent::setText(const std::string& newText) {
    text = newText;
    isText = !text.empty();
}

void DrawableComponent::setCharacter(char c) {
    character = c;
}

void DrawableComponent::setColor(const Color& newColor) {
    color = newColor;
}

void DrawableComponent::setScale(float newScale) {
    scale = newScale;
}

void DrawableComponent::setPosition(float x, float y) {
    posX = x;
    posY = y;
}

void DrawableComponent::setRotation(float newRotation) {
    rotation = newRotation;
}

void DrawableComponent::setVisibility(bool visible) {
    isVisible = visible;
}

void DrawableComponent::setDimensions(float w, float h) {
    width = w;
    height = h;
}
}  // namespace Arcade
