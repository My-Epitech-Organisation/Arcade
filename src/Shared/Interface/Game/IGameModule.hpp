// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** IGameModule interface
*/
/**
 * @file IGameModule.hpp
 * @brief Interface for game modules in the Arcade project.
 *
 * This interface defines the necessary methods for implementing
 * a game module in the Arcade project.
 */

#ifndef SRC_SHARED_INTERFACE_GAME_IGAMEMODULE_HPP_
    #define SRC_SHARED_INTERFACE_GAME_IGAMEMODULE_HPP_
    #include <vector>
    #include <memory>
    #include "Interface/ECS/IEntity.hpp"
    #include "Interface/IArcadeModule.hpp"

namespace Arcade {
/**
 * @class IGameModule
 * @brief Interface for game logic modules.
 *
 * The IGameModule interface provides a set of pure virtual functions
 * that must be implemented by any game module in the Arcade project.
 * It includes methods for starting, updating, rendering, and handling input.
 */
class IGameModule : public IArcadeModule {
 public:
    virtual ~IGameModule() = default;
    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void stop() = 0;
    virtual std::vector<std::shared_ptr<IEntity>>
        getEntities() const = 0;
    virtual bool isGameOver() const = 0;
    virtual bool hasWon() const = 0;
};
}  // namespace Arcade
#endif  // SRC_SHARED_INTERFACE_GAME_IGAMEMODULE_HPP_
