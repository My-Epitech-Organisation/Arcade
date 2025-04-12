// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** System Manager
*/
#include <algorithm>
#include <memory>
#include "ECS/System/SystemManager.hpp"

namespace Arcade {
void SystemManager::registerSystem(std::shared_ptr<Arcade::ISystem> system) {
    _systems.push_back(system);
}

void SystemManager::removeSystem(std::shared_ptr<Arcade::ISystem> system) {
    _systems.erase(std::remove(_systems.begin(),
        _systems.end(), system), _systems.end());
}

void SystemManager::updateSystems() {
    for (std::shared_ptr<Arcade::ISystem> system : _systems) {
        system->update();
    }
}

void SystemManager::clearSystems() {
    _systems.clear();
}
}  // namespace Arcade
