// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** B-OOP-400 Arcade
*/

#include "Models/WindowManager/WindowManager.hpp"
#include <string>

int Arcade::WindowManager::getHeight() const {
    return _height;
}
int Arcade::WindowManager::getWidth() const {
    return _width;
}
float Arcade::WindowManager::getFps() const {
    return _fps;
}
std::string Arcade::WindowManager::getTitle() const {
    return _title;
}
std::string Arcade::WindowManager::getIconPath() const {
    return _iconPath;
}
Arcade::Vector2i Arcade::WindowManager::getWindowSize() const {
    return Vector2i(_width, _height);
}

void Arcade::WindowManager::setHeight(int height) {
    height = height;
}
void Arcade::WindowManager::setWidth(int width) {
    _width = width;
}
void Arcade::WindowManager::setFps(float fps) {
    _fps = fps;
}
void Arcade::WindowManager::setTitle(const std::string &title) {
    _title = title;
}
void Arcade::WindowManager::setIconPath(const std::string &iconPath) {
    _iconPath = iconPath;
}
void Arcade::WindowManager::setWindowSize(Vector2i vec) {
    setWidth(vec.x);
    setHeight(vec.y);
}
