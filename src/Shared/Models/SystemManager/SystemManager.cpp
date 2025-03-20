// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** System Manager
*/
#include "Models/SystemManager/SystemManager.hpp"

void SystemManager::addSystem(Arcade::ISystem* system) {
    _systems.push_back(system);
}

void SystemManager::updateSystems() {
    for (Arcade::ISystem* system : _systems) {
        system->update();
    }
}
