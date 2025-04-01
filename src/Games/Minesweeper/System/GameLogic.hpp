/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** GameLogic
*/

#ifndef GAMELOGIC_HPP_
#define GAMELOGIC_HPP_
#include <memory>
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/ECS/ISystem.hpp"

class GameLogic : public Arcade::ISystem {
    public:
        GameLogic(std::shared_ptr<Arcade::IComponentManager> componentManager,
            std::shared_ptr<Arcade::IEntityManager> entityManager)
            : _componentManager(componentManager), _entityManager(entityManager) {};
        void update() override;

    private:
        std::shared_ptr<Arcade::IComponentManager> _componentManager;
        std::shared_ptr<Arcade::IEntityManager> _entityManager;
};

#endif /* !GAMELOGIC_HPP_ */
