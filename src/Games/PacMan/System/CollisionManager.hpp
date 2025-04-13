// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Collision Manager for Pacman
*/

#ifndef SRC_GAMES_PACMAN_SYSTEM_COLLISIONMANAGER_HPP_
#define SRC_GAMES_PACMAN_SYSTEM_COLLISIONMANAGER_HPP_

#include <memory>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <functional>
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"
#include "Games/PacMan/Components/FoodComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace PacMan {

class GameStateManager;

struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

enum class EntityType {
    GHOST,
    FOOD,
    PACMAN,
    WALL
};

class CollisionManager {
 public:
    CollisionManager(
        std::shared_ptr<Arcade::IComponentManager> componentManager,
        std::shared_ptr<Arcade::IEntityManager> entityManager,
        const std::map<std::string, DrawableComponent>& assets);
    void checkCollisions(std::shared_ptr<PacmanComponent> pacman,
                        std::shared_ptr<GridComponent> grid,
                        std::shared_ptr<IEntity> pacmanEntity,
                        std::shared_ptr<IEntity> gridEntity);
    void checkCollisionsWithGhosts(std::shared_ptr<PacmanComponent> pacman,
                                  std::shared_ptr<GridComponent> grid);
    void checkCollisionsWithFood(std::shared_ptr<PacmanComponent> pacman,
                               std::shared_ptr<GridComponent> grid);
    void checkFoodCollision(std::shared_ptr<PacmanComponent> pacman,
                          std::shared_ptr<GridComponent> grid);
    std::shared_ptr<IDrawableComponent> getDrawableAsset
        (const std::string& key) const;

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    const std::map<std::string, DrawableComponent>& _assets;
    std::map<std::string, std::shared_ptr<IDrawableComponent>> _cachedAssets;
    std::vector<std::shared_ptr<IEntity>> _cachedGhostEntities;
    std::unordered_map<std::pair<size_t, size_t>,
                      std::vector<std::pair<std::shared_ptr<IEntity>,
                      EntityType>>,
                      PairHash> _entityGrid;
    static constexpr float COLLISION_GRID_REFRESH_TIME = 0.05f;
    float _rebuildCollisionGridTimer;
    void rebuildCollisionGrid();
    void updateGhostsForPowerPill();
    void findAndSetPacmanSpawn(std::shared_ptr<PacmanComponent> pacman,
                             std::shared_ptr<GridComponent> grid);
    std::shared_ptr<GameStateManager> _cachedGameStateManager;
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_SYSTEM_COLLISIONMANAGER_HPP_
