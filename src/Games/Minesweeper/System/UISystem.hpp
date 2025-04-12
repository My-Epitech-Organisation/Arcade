// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** UISystem
*/

#ifndef SRC_GAMES_MINESWEEPER_SYSTEM_UISYSTEM_HPP_
    #define SRC_GAMES_MINESWEEPER_SYSTEM_UISYSTEM_HPP_
    #include <memory>
    #include <vector>
    #include "Shared/Interface/ECS/ISystem.hpp"
    #include "Shared/Interface/ECS/IComponentManager.hpp"
    #include "Shared/Interface/ECS/IEntityManager.hpp"
    #include "Shared/Models/ColorType.hpp"

namespace Arcade {
class UISystem : public Arcade::ISystem {
 public:
    UISystem(std::shared_ptr<Arcade::IComponentManager> componentManager,
             std::shared_ptr<Arcade::IEntityManager> entityManager)
        : _componentManager(componentManager),
          _entityManager(entityManager) {
        createUIEntities();
    }

    void update() override;

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;

    std::shared_ptr<IEntity> _minesTextEntity;
    std::shared_ptr<IEntity> _timeTextEntity;
    std::shared_ptr<IEntity> _scoreTextEntity;
    std::shared_ptr<IEntity> _gameOverTextEntity;
    std::shared_ptr<IEntity> _gameResultTextEntity;
    std::shared_ptr<IEntity> _restartTextEntity;

    void createUIEntities();
    void updateUIEntities();
};
}  // namespace Arcade

#endif  // SRC_GAMES_MINESWEEPER_SYSTEM_UISYSTEM_HPP_
