// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Render System
*/

#ifndef SRC_GAMES_MINESWEEPER_SYSTEM_RENDERSYSTEM_HPP_
    #define SRC_GAMES_MINESWEEPER_SYSTEM_RENDERSYSTEM_HPP_
    #include <memory>
    #include <utility>
    #include "Shared/Interface/ECS/ISystem.hpp"
    #include "Shared/Interface/ECS/IComponentManager.hpp"
    #include "Shared/Interface/ECS/IEntityManager.hpp"

class RenderSystem : public Arcade::ISystem {
 public:
    RenderSystem(std::shared_ptr<Arcade::IComponentManager> componentManager,
        std::shared_ptr<Arcade::IEntityManager> entityManager
        , std::pair<std::size_t, std::size_t> windowSize)
        : _componentManager(componentManager), _entityManager(entityManager) {}
    void update() override;

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    std::pair<std::size_t, std::size_t> _windowSize;
};

#endif  // SRC_GAMES_MINESWEEPER_SYSTEM_RENDERSYSTEM_HPP_
