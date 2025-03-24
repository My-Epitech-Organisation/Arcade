// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Component Manager
*/

#include <unordered_map>
#include "Components/ComponentManager.hpp"
#include "Shared/Models/EntityType.hpp"

template <typename T>
void ComponentManager<T>::registerComponent(Arcade::Entity entity,
    T component) {
    components[entity] = component;
}

template <typename T>
void ComponentManager<T>::removeComponent(Arcade::Entity entity) {
    components.erase(entity);
}

template <typename T>
T* ComponentManager<T>::getComponent(Arcade::Entity entity) {
    auto it = components.find(entity);
    return (it != components.end()) ? &it->second : nullptr;
}

template <typename T>
std::unordered_map<Arcade::Entity, T>& ComponentManager<T>::getAll() {
    return components;
}
