// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** UI System for Pacman
*/

#ifndef SRC_GAMES_PACMAN_SYSTEM_UISYSTEM_HPP_
#define SRC_GAMES_PACMAN_SYSTEM_UISYSTEM_HPP_

#include <memory>
#include <vector>
#include <map>
#include <string>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Models/ColorType.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "Games/PacMan/Components/PacmanComponent.hpp"
#include "Games/PacMan/Components/GridComponent.hpp"

namespace Arcade {
namespace PacMan {

class UISystem : public Arcade::ISystem {
 public:
    UISystem(std::shared_ptr<Arcade::IComponentManager> componentManager,
             std::shared_ptr<Arcade::IEntityManager> entityManager,
             const std::map<std::string, DrawableComponent>& assets);

    ~UISystem() = default;

    void update() override;

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    const std::map<std::string, DrawableComponent>& _assets;
    std::shared_ptr<IEntity> _scoreTextEntity;
    std::shared_ptr<IEntity> _livesTextEntity;
    std::shared_ptr<IEntity> _levelTextEntity;
    std::shared_ptr<IEntity> _gameOverTextEntity;
    std::shared_ptr<IEntity> _gameWinTextEntity;
    std::shared_ptr<IEntity> _restartTextEntity;
    std::vector<std::shared_ptr<IEntity>> _livesIconEntities;
    void createUIEntities();
    void updateUITexts();
    void updateLivesIcons();
    void updateGameOverState();
    std::shared_ptr<IEntity> _cachedPacmanEntity;
    std::shared_ptr<IEntity> _cachedGridEntity;
    std::shared_ptr<PacmanComponent> _cachedPacmanComponent;
    std::shared_ptr<GridComponent> _cachedGridComponent;
};

}  // namespace PacMan
}  // namespace Arcade

#endif  // SRC_GAMES_PACMAN_SYSTEM_UISYSTEM_HPP_
