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
#include <memory>
#include <vector>
#include "Shared/Interface/ECS/IComponent.hpp"
#include "Shared/Models/ComponentType.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"

namespace Arcade {
namespace PacMan {

enum class GhostType {
    RED,   // Blinky - pursues PacMan
    PINK,  // Pinky - ambushes in front of PacMan
    BLUE,  // Inky - unpredictable
    ORANGE  // Clyde - random movement
};

enum class GhostState {
    NORMAL,
    SCARED,
    RETURNING
};

enum class GhostMode {
    CHASE,
    SCATTER,
    FRIGHTENED
};

class GhostComponent : public Arcade::IComponent {
 public:
    explicit GhostComponent(GhostType type = GhostType::RED);
    ~GhostComponent() = default;

    ComponentType getType() const override {
        return static_cast<ComponentType>(1002);  // Custom component type
    }

    GhostType getGhostType() const { return _ghostType; }
    GhostState getState() const { return _state; }
    GhostMode getMode() const { return _mode; }
    void setMode(GhostMode mode) { _mode = mode; }
    void setHomeCorner(size_t x, size_t y) { _homeCornerX = x;
        _homeCornerY = y; }
    size_t getHomeCornerX() const { return _homeCornerX; }
    size_t getHomeCornerY() const { return _homeCornerY; }
    void setTarget(size_t x, size_t y) { _targetX = x;
        _targetY = y; }
    size_t getTargetX() const { return _targetX; }
    size_t getTargetY() const { return _targetY; }
    Direction getOppositeDirection() const;
    void updateModeTimer(float deltaTime);
    static bool isGhostAtPosition(size_t x, size_t y,
        const std::vector<std::shared_ptr<GhostComponent>>& ghosts,
        GhostComponent* self);
    void setState(GhostState state) { _state = state; }
    Direction getCurrentDirection() const { return _currentDirection; }
    void setCurrentDirection(Direction dir) { _currentDirection = dir; }
    void setName(const std::string& name) { _name = name; }
    const std::string& getName() const { return _name; }
    size_t getGridX() const { return _gridX; }
    size_t getGridY() const { return _gridY; }
    void setGridPosition(size_t x, size_t y) { _gridX = x;
        _gridY = y; }
    float getStateTimer() const { return _stateTimer; }
    void resetStateTimer() { _stateTimer = 0; }
    void updateStateTimer(float deltaTime);
    bool canMove() const { return _canMove; }
    void setCanMove(bool canMove) { _canMove = canMove; }
    void updateMovementTimer(float deltaTime);
    void chooseNextDirection();
    float getMovementTimer() const { return _movementTimer; }
    float getMovementThreshold() const { return _movementThreshold; }
    void resetMovementTimer() { _movementTimer = 0.0f; }
    void addGameTime(float deltaTime) { _totalGameTime += deltaTime; }
    bool canLeaveBox() const { return _releaseTimer >= RELEASE_DELAY; }
    void setMovementThreshold(float threshold) {
        _movementThreshold = threshold; }
    void updateReleaseTimer(float deltaTime);
    void resetReleaseTimer() { _releaseTimer = 0.0f; }

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
    static constexpr float SCARED_DURATION = 10.0f;
    GhostMode _mode;
    size_t _homeCornerX;
    size_t _homeCornerY;
    size_t _targetX;
    size_t _targetY;
    float _modeTimer;
    float _totalGameTime = 0.0f;
    float _movementThreshold = 0.25f;
    float _releaseTimer = 0.0f;
    static constexpr float RELEASE_DELAY = 10.0f;

    static constexpr float CHASE_DURATION = 20.0f;
    static constexpr float SCATTER_DURATION = 5.0f;
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_COMPONENTS_GHOSTCOMPONENT_HPP_
