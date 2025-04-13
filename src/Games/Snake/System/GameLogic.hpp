// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Game Logic System for Snake game
*/

#ifndef SRC_GAMES_SNAKE_SYSTEM_GAMELOGIC_HPP_
#define SRC_GAMES_SNAKE_SYSTEM_GAMELOGIC_HPP_

#include <memory>
#include <map>
#include <chrono>
#include <utility>
#include <string>
#include <vector>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Games/Snake/Components/SnakeHeadComponent.hpp"
#include "Games/Snake/Components/GridComponent.hpp"
#include "Games/Snake/Components/FoodComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace Snake {

/**
 * @class GameLogic
 * @brief Main system for handling Snake game logic
 */
class GameLogic : public Arcade::ISystem {
 public:
    /**
     * @brief Constructor
     * @param componentManager Component manager
     * @param entityManager Entity manager
     * @param assets Game assets
     */
    GameLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
             std::shared_ptr<Arcade::IEntityManager> entityManager,
             const std::map<std::string, DrawableComponent>& assets);

    /**
     * @brief Updates the game state
     */
    void update() override;

    /**
     * @brief Resets the game to initial state
     */
    void resetGame();

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    std::chrono::high_resolution_clock::time_point _lastUpdateTime;
    const std::map<std::string, DrawableComponent>& _assets;
    float _currentDeltaTime = 0.0f;
    bool _bonusFoodActive = false;
    float _bonusFoodTimer = 0.0f;
    float _difficultyTimer = 0.0f;

    /**
     * @brief Finds the snake head entity
     * @return Entity ID
     */
    Arcade::Entity findSnakeHeadEntity();

    /**
     * @brief Finds the grid entity
     * @return Entity ID
     */
    Arcade::Entity findGridEntity();

    /**
     * @brief Gets the snake head and grid components
     * @return Pair of components
     */
    std::pair<std::shared_ptr<SnakeHeadComponent>,
              std::shared_ptr<GridComponent>> getSnakeAndGridComponents();

    /**
     * @brief Gets the asset for drawing
     * @param key Asset key
     * @return DrawableComponent pointer
     */
    std::shared_ptr<DrawableComponent>
        getDrawableAsset(const std::string& key) const;

    /**
     * @brief Updates the delta time for frame-rate independence
     */
    void updateDeltaTime();

    /**
     * @brief Gets the current delta time
     * @return Delta time in seconds
     */
    float getDeltaTime();

    /**
     * @brief Tries to change the snake's direction
     * @param snake Snake head component
     * @param grid Grid component
     */
    void tryChangeDirection(std::shared_ptr<SnakeHeadComponent> snake,
                           std::shared_ptr<GridComponent> grid);

    /**
     * @brief Moves the snake
     * @param snake Snake head component
     * @param grid Grid component
     * @return True if the snake moved
     */
    bool moveSnake(std::shared_ptr<SnakeHeadComponent> snake,
                  std::shared_ptr<GridComponent> grid);

    /**
     * @brief Updates the drawable positions
     * @param snake Snake head component
     * @param grid Grid component
     */
    void updateDrawablePositions(std::shared_ptr<SnakeHeadComponent> snake,
                               std::shared_ptr<GridComponent> grid);

    /**
     * @brief Checks for collisions with food
     * @param snake Snake head component
     * @param grid Grid component
     */
    void checkFoodCollision(std::shared_ptr<SnakeHeadComponent> snake,
                          std::shared_ptr<GridComponent> grid);

    /**
     * @brief Checks if the snake hit itself or a wall
     * @param snake Snake head component
     * @param grid Grid component
     * @return True if collision occurred
     */
    bool checkSnakeCollision(std::shared_ptr<SnakeHeadComponent> snake,
                            std::shared_ptr<GridComponent> grid);

    /**
     * @brief Creates a new food entity
     * @param grid Grid component
     * @param type Food type
     */
    void createFood(std::shared_ptr<GridComponent> grid, FoodType type);

    /**
     * @brief Finds all food entities
     * @return Vector of food entity IDs
     */
    std::vector<Arcade::Entity> findFoodEntities();

    /**
     * @brief Updates the snake body positions
     * @param snake Snake head component
     * @param grid Grid component
     * @param newHeadX New head X position
     * @param newHeadY New head Y position
     */
    void updateSnakeBody(std::shared_ptr<SnakeHeadComponent> snake,
                       std::shared_ptr<GridComponent> grid,
                       size_t newHeadX, size_t newHeadY);

    /**
     * @brief Increases game speed over time
     * @param snake Snake head component
     */
    void increaseDifficulty(std::shared_ptr<SnakeHeadComponent> snake);

    /**
     * @brief Met à jour ou crée les entités visuelles pour chaque segment du corps
     * @param segments Les segments du corps du serpent
     * @param startX Position X de départ de la grille
     * @param startY Position Y de départ de la grille
     * @param cellSize Taille d'une cellule
     */
    void updateBodySegmentEntities(const std::vector<std::pair<size_t, size_t>>& segments, 
                                  float startX, float startY, float cellSize);
};

}  // namespace Snake
}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SYSTEM_GAMELOGIC_HPP_