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
#include <map>
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "Games/Snake/Components/GridComponent.hpp"
#include "Games/Snake/Components/SnakeHeadComponent.hpp"
#include "Games/Snake/Components/FoodComponent.hpp"

namespace Arcade {
namespace Snake {

/**
 * @class SnakeFactory
 * @brief Factory class for creating Snake game entities
 */
class SnakeFactory {
 public:
    /**
     * @brief Constructor
     * @param entityManager Entity manager
     * @param componentManager Component manager
     * @param assets Game assets
     */
    SnakeFactory(
        std::shared_ptr<Arcade::IEntityManager> entityManager,
        std::shared_ptr<Arcade::IComponentManager> componentManager,
        const std::map<std::string, DrawableComponent>& assets)
        : _entityManager(entityManager), _componentManager(componentManager),
        _assets(assets) {}

    /**
     * @brief Creates the game grid
     * @param width Grid width
     * @param height Grid height
     * @return Entity ID
     */
    Arcade::Entity createGrid(size_t width, size_t height);

    /**
     * @brief Creates the snake head entity
     * @param x X position
     * @param y Y position
     * @param gridX Grid X position
     * @param gridY Grid Y position
     * @return Entity ID
     */
    Arcade::Entity createSnakeHead(float x, float y, size_t gridX, size_t gridY);

    /**
     * @brief Creates a food entity
     * @param x X position
     * @param y Y position
     * @param gridX Grid X position
     * @param gridY Grid Y position
     * @param type Food type
     * @return Entity ID
     */
    Arcade::Entity createFood(float x, float y, size_t gridX, size_t gridY,
                             FoodType type);

    /**
     * @brief Initializes the game
     * @param cellSize Size of each grid cell
     */
    void initializeGame(float cellSize);

 private:
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    const std::map<std::string, DrawableComponent>& _assets;

    /**
     * @brief Gets a drawable asset
     * @param key Asset key
     * @return DrawableComponent pointer
     */
    std::shared_ptr<DrawableComponent>
        getDrawableAsset(const std::string& key) const;
};

}  // namespace Snake
}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SNAKEFACTORY_HPP_