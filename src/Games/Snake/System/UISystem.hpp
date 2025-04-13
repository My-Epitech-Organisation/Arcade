// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** UI System for Snake game
*/

#ifndef SRC_GAMES_SNAKE_SYSTEM_UISYSTEM_HPP_
#define SRC_GAMES_SNAKE_SYSTEM_UISYSTEM_HPP_

#include <memory>
#include <map>
#include <string>
#include <utility>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Games/Snake/Components/SnakeHeadComponent.hpp"
#include "Games/Snake/Components/GridComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace Snake {

/**
 * @class UISystem
 * @brief Manages the UI elements of the Snake game
 */
class UISystem : public Arcade::ISystem {
 public:
    /**
     * @brief Constructor
     * @param componentManager Component manager
     * @param entityManager Entity manager
     * @param assets Game assets
     */
    UISystem(std::shared_ptr<Arcade::IComponentManager> componentManager,
            std::shared_ptr<Arcade::IEntityManager> entityManager,
            const std::map<std::string, DrawableComponent>& assets);

    /**
     * @brief Updates the UI elements
     */
    void update() override;

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    const std::map<std::string, DrawableComponent>& _assets;
    Arcade::Entity _scoreEntity;
    Arcade::Entity _gameOverEntity;

    /**
     * @brief Creates the score text entity
     */
    void createScoreEntity();

    /**
     * @brief Creates the game over text entity
     */
    void createGameOverEntity();

    /**
     * @brief Updates the score text
     * @param score Current score
     */
    void updateScore(int score);

    /**
     * @brief Shows or hides the game over message
     * @param isGameOver Whether the game is over
     */
    void updateGameOverVisibility(bool isGameOver);

    /**
     * @brief Gets the snake head and grid components
     * @return Pair of components
     */
    std::pair<std::shared_ptr<SnakeHeadComponent>,
              std::shared_ptr<GridComponent>> getSnakeAndGridComponents();

    /**
     * @brief Finds the snake head entity
     * @return Entity ID
     */
    Arcade::Entity findSnakeHeadEntity() const;

    /**
     * @brief Finds the grid entity
     * @return Entity ID
     */
    Arcade::Entity findGridEntity() const;
};

}  // namespace Snake
}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SYSTEM_UISYSTEM_HPP_
