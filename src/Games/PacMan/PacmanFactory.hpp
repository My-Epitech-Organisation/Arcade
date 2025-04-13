// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Factory for creating Pacman game entities
*/

#ifndef SRC_GAMES_PACMAN_PACMANFACTORY_HPP_
#define SRC_GAMES_PACMAN_PACMANFACTORY_HPP_

#include <memory>
#include <string>
#include <map>
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Sprite/SpriteComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GhostComponent.hpp"
#include "Games/PacMan/Components/FoodComponent.hpp"

namespace Arcade {
namespace PacMan {

class PacmanFactory {
 public:
    PacmanFactory(
        std::shared_ptr<Arcade::IEntityManager> entityManager,
        std::shared_ptr<Arcade::IComponentManager> componentManager,
        const std::map<std::string, DrawableComponent>& assets)
        : _entityManager(entityManager), _componentManager(componentManager),
        _assets(assets) {}

    std::shared_ptr<Arcade::IEntity> createGrid(size_t width, size_t height);
    std::shared_ptr<Arcade::IEntity>
        createPacman(float x, float y, size_t gridX, size_t gridY);
    std::shared_ptr<Arcade::IEntity>
        createGhost(float x, float y, size_t gridX, size_t gridY,
        GhostType type);
    std::shared_ptr<Arcade::IEntity>
        createFood(float x, float y, size_t gridX, size_t gridY,
        FoodType type);
    std::shared_ptr<Arcade::IEntity>
        createWall(float x, float y, size_t gridX, size_t gridY);
    void initializeGame(float cellSize);

 private:
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    const std::map<std::string, DrawableComponent>& _assets;
    std::string getGhostSpritePath(GhostType type);
    char getGhostCharacter(GhostType type);
    void createFoodEntities(std::shared_ptr<GridComponent> grid,
        float cellSize, std::shared_ptr<Arcade::IEntity> gridEntity);
    void createWallEntities(std::shared_ptr<GridComponent> grid,
        float cellSize, std::shared_ptr<Arcade::IEntity> gridEntity);
    void createGhostEntities(std::shared_ptr<GridComponent> grid,
        float cellSize, std::shared_ptr<Arcade::IEntity> gridEntity);
    void createPacmanEntity(std::shared_ptr<GridComponent> grid,
        float cellSize, std::shared_ptr<Arcade::IEntity> gridEntity);
    std::shared_ptr<IDrawableComponent>
        getDrawableAsset(const std::string& key) const;
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_PACMANFACTORY_HPP_
