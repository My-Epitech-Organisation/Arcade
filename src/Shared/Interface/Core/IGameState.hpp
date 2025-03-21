// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game state interface
*/

#ifndef SRC_SHARED_INTERFACE_CORE_IGAMESTATE_HPP_
    #define SRC_SHARED_INTERFACE_CORE_IGAMESTATE_HPP_

namespace Arcade {
class IGameState {
 public:
    virtual ~IGameState() = default;
    virtual void onEnter() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onExit() = 0;
};
}  // namespace Arcade
#endif  // SRC_SHARED_INTERFACE_CORE_IGAMESTATE_HPP_
