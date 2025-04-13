// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Event Subsystem for Snake game
*/

#ifndef SRC_GAMES_SNAKE_SYSTEM_EVENTSUBSYSTEM_HPP_
#define SRC_GAMES_SNAKE_SYSTEM_EVENTSUBSYSTEM_HPP_

#include <memory>
#include <map>
#include <string>
#include <vector>
#include "Shared/Interface/ECS/ISystem.hpp"
#include "Shared/Interface/ECS/IComponentManager.hpp"
#include "Shared/Interface/ECS/IEntityManager.hpp"
#include "Shared/Interface/Core/IEventManager.hpp"
#include "Games/Snake/Components/SnakeHeadComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"
#include "Shared/EventManager/KeyEvent/KeyEvent.hpp"

namespace Arcade {
namespace Snake {
class GameLogic;
}
}

namespace Arcade {
namespace Snake {

/**
 * @class EventSubSystem
 * @brief Handles input events for the Snake game
 */
class EventSubSystem : public Arcade::ISystem {
 public:
    /**
     * @brief Constructor
     * @param componentManager Component manager
     * @param entityManager Entity manager
     * @param eventManager Event manager
     * @param assets Game assets
     * @param gameLogic Pointer to the GameLogic system
     */
    EventSubSystem(std::shared_ptr<Arcade::IComponentManager> componentManager,
                 std::shared_ptr<Arcade::IEntityManager> entityManager,
                 std::shared_ptr<Arcade::IEventManager> eventManager,
                 const std::map<std::string, DrawableComponent>& assets,
                 std::shared_ptr<GameLogic> gameLogic);

    /**
     * @brief Destructor
     */
    ~EventSubSystem() override;

    /**
     * @brief Updates the system
     */
    void update() override;

    /**
     * @brief Unsubscribes from all events
     */
    void unsubscribeEvents();

 private:
    std::shared_ptr<Arcade::IComponentManager> _componentManager;
    std::shared_ptr<Arcade::IEntityManager> _entityManager;
    std::shared_ptr<Arcade::IEventManager> _eventManager;
    const std::map<std::string, DrawableComponent>& _assets;
    std::shared_ptr<GameLogic> _gameLogic;

    /**
     * @brief Subscribes to relevant events
     */
    void subscribeToEvents();

    /**
     * @brief Handles arrow key up input
     */
    void handleKeyUpPressed();

    /**
     * @brief Handles arrow key down input
     */
    void handleKeyDownPressed();

    /**
     * @brief Handles arrow key left input
     */
    void handleKeyLeftPressed();

    /**
     * @brief Handles arrow key right input
     */
    void handleKeyRightPressed();

    /**
     * @brief Handles restart game input
     */
    void handleRestartPressed();

    /**
     * @brief Handles quit game input
     */
    void handleQuitPressed();

    /**
     * @brief Finds the snake head entity
     * @return Entity ID
     */
    Arcade::Entity findSnakeHeadEntity() const;

    /**
     * @brief List to store subscribed events
     */
    std::vector<Arcade::KeyEvent> _subscribedEvents;
};

}  // namespace Snake
}  // namespace Arcade

#endif  // SRC_GAMES_SNAKE_SYSTEM_EVENTSUBSYSTEM_HPP_
