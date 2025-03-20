// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Component Manager
*/

#include "Models/EntityComponentSystem/ComponentManager.hpp"
#include <unordered_map>

template <typename T>
void ComponentManager<T>::addComponent(Entity entity, T component) {
    components[entity] = component;
}

template <typename T>
void ComponentManager<T>::removeComponent(Entity entity) {
    components.erase(entity);
}

template <typename T>
T* ComponentManager<T>::getComponent(Entity entity) {
    auto it = components.find(entity);
    return (it != components.end()) ? &it->second : nullptr;
}

template <typename T>
std::unordered_map<Entity, T>& ComponentManager<T>::getAll() {
    return components;
}
