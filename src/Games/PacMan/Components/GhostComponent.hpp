// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Ghost Component
*/

#ifndef SRC_GAMES_PACMAN_COMPONENTS_GHOSTCOMPONENT_HPP_
#define SRC_GAMES_PACMAN_COMPONENTS_GHOSTCOMPONENT_HPP_

#include <string>
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Models/ComponentType.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"

namespace Arcade {
namespace PacMan {

enum class GhostType {
    RED,   // Blinky - pursues PacMan
    PINK,  // Pinky - ambushes in front of PacMan
    BLUE,  // Inky - unpredictable
    ORANGE // Clyde - random movement
};

enum class GhostState {
    NORMAL,
    SCARED,
    RETURNING
};

class GhostComponent : public Arcade::IComponent {
public:
    GhostComponent(GhostType type = GhostType::RED);
    ~GhostComponent() = default;

    ComponentType getType() const override {
        return static_cast<ComponentType>(1002); // Custom component type
    }

    GhostType getGhostType() const { return _ghostType; }
    GhostState getState() const { return _state; }
    void setState(GhostState state) { _state = state; }
    Direction getCurrentDirection() const { return _currentDirection; }
    void setCurrentDirection(Direction dir) { _currentDirection = dir; }
    void setName(const std::string& name) { _name = name; }
    const std::string& getName() const { return _name; }
    size_t getGridX() const { return _gridX; }
    size_t getGridY() const { return _gridY; }
    void setGridPosition(size_t x, size_t y) { _gridX = x; _gridY = y; }
    float getStateTimer() const { return _stateTimer; }
    void resetStateTimer() { _stateTimer = 0; }
    void updateStateTimer(float deltaTime);
    bool canMove() const { return _canMove; }
    void setCanMove(bool canMove) { _canMove = canMove; }
    void resetMovementTimer() { _movementTimer = 0; }
    void updateMovementTimer(float deltaTime);
    void chooseNextDirection();

private:
    std::string _name;
    GhostType _ghostType;
    GhostState _state;
    Direction _currentDirection;
    size_t _gridX;
    size_t _gridY;
    float _stateTimer;
    float _scaredDuration;
    bool _canMove;
    float _movementTimer;
    float _movementCooldown;
    static constexpr float SCARED_DURATION = 10.0f; // seconds
};

} // namespace PacMan
} // namespace Arcade

#endif // SRC_GAMES_PACMAN_COMPONENTS_GHOSTCOMPONENT_HPP_