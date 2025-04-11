// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Factory for creating Snake game entities
*/

#ifndef SRC_GAMES_SNAKE_SNAKEFACTORY_HPP_
#define SRC_GAMES_SNAKE_SNAKEFACTORY_HPP_

#include <memory>
#include <string>
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "Games/Snake/Types.hpp"

namespace Arcade {

class SnakeComponent;
class FoodComponent;

class SnakeFactory {
 public:
    SnakeFactory(
        std::shared_ptr<Arcade::IEntityManager> entityManager,
        std::shared_ptr<Arcade::IComponentManager> componentManager)
        : _entityManager(entityManager), _componentManager(componentManager) {}

    Arcade::Entity createSnake(float x, float y, Direction initialDirection);
    Arcade::Entity createSnakeSegment(float x, float y,
        bool isHead = false, bool isTail = false,
        Direction dir = Direction::RIGHT);
    Arcade::Entity createFood(float x, float y);
    Arcade::Entity createGrid(size_t width, size_t height, float cellSize = 20.0f);

 private:
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    std::shared_ptr<Arcade::IComponentManager> _componentManager;

    std::string getSnakeSegmentSprite(bool isHead, bool isTail, Direction dir);
};

}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SNAKEFACTORY_HPP_
