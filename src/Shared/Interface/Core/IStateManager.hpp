// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game state interface
*/

#ifndef SRC_SHARED_INTERFACE_CORE_ISTATEMANAGER_HPP_
    #define SRC_SHARED_INTERFACE_CORE_ISTATEMANAGER_HPP_
    #include <memory>
    #include "Core/IGameState.hpp"

namespace Arcade {
class IStateManager {
 public:
    virtual ~IStateManager() = default;
    virtual void changeState(std::shared_ptr<IGameState> newState) = 0;
};
}  // namespace Arcade
#endif  // SRC_SHARED_INTERFACE_CORE_ISTATEMANAGER_HPP_
