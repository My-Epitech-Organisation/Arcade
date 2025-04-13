// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** UI System implementation for Snake game
*/

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <map>
#include "Games/Snake/System/UISystem.hpp"
#include "ECS/Components/Position/PositionComponent.hpp"
#include "ECS/Components/Text/TextComponent.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace Snake {

UISystem::UISystem(std::shared_ptr<Arcade::IComponentManager> componentManager,
std::shared_ptr<Arcade::IEntityManager> entityManager,
const std::map<std::string, DrawableComponent>& assets)
: _componentManager(componentManager), _entityManager(entityManager),
_assets(assets), _scoreEntity(0), _gameOverEntity(0) {
    createScoreEntity();
    createGameOverEntity();
}

void UISystem::update() {
    auto [snake, grid] = getSnakeAndGridComponents();
    if (!snake || !grid)
        return;

    updateScore(snake->getScore());

    updateGameOverVisibility(grid->isGameOver());
}

void UISystem::createScoreEntity() {
    _scoreEntity = _entityManager->createEntity("ScoreText");

    auto posComp = std::make_shared<PositionComponent>(10.0f, 10.0f);
    _componentManager->registerComponent(_scoreEntity, posComp);

    auto textComp = std::make_shared<TextComponent>("Score: 0",
        10, 10, Arcade::Color::WHITE);
    _componentManager->registerComponent(_scoreEntity, textComp);

    auto drawableComp = std::make_shared<DrawableComponent>();
    drawableComp->setAsText("Score: 0", "./assets/fonts/Arial.ttf", 20);
    drawableComp->posX = 10.0f;
    drawableComp->posY = 10.0f;
    drawableComp->isVisible = true;
    drawableComp->color = Arcade::Color::WHITE;
    _componentManager->registerComponent(_scoreEntity, drawableComp);
}

void UISystem::createGameOverEntity() {
    _gameOverEntity = _entityManager->createEntity("GameOverText");

    auto posComp = std::make_shared<PositionComponent>(250.0f, 300.0f);
    _componentManager->registerComponent(_gameOverEntity, posComp);

    auto textComp = std::make_shared<TextComponent>(
        "GAME OVER!\nPress 'R' to restart",
        250, 300, Arcade::Color::RED);
    _componentManager->registerComponent(_gameOverEntity, textComp);

    auto drawableComp = std::make_shared<DrawableComponent>();
    drawableComp->setAsText("GAME OVER!\nPress 'R' to restart",
        "./assets/fonts/Arial.ttf", 30);
    drawableComp->posX = 250.0f;
    drawableComp->posY = 300.0f;
    drawableComp->isVisible = false;
    drawableComp->color = Arcade::Color::RED;
    _componentManager->registerComponent(_gameOverEntity, drawableComp);
}

void UISystem::updateScore(int score) {
    auto textComp = std::dynamic_pointer_cast<TextComponent>(
        _componentManager->getComponentByType(_scoreEntity,
            ComponentType::TEXT));
    if (textComp) {
        textComp->text = "Score: " + std::to_string(score);
    }

    auto drawableComp = std::dynamic_pointer_cast<DrawableComponent>(
        _componentManager->getComponentByType(_scoreEntity,
            ComponentType::DRAWABLE));
    if (drawableComp) {
        drawableComp->text = "Score: " + std::to_string(score);
    }
}

void UISystem::updateGameOverVisibility(bool isGameOver) {
    auto drawableComp = std::dynamic_pointer_cast<DrawableComponent>(
        _componentManager->getComponentByType(_gameOverEntity,
            ComponentType::DRAWABLE));
    if (drawableComp) {
        drawableComp->isVisible = isGameOver;
    }
}

std::pair<std::shared_ptr<SnakeHeadComponent>, std::shared_ptr<GridComponent>>
UISystem::getSnakeAndGridComponents() {
    Arcade::Entity snakeEntity = findSnakeHeadEntity();
    Arcade::Entity gridEntity = findGridEntity();

    auto snakeComp = std::dynamic_pointer_cast<SnakeHeadComponent>(
        _componentManager->getComponentByType(snakeEntity,
            static_cast<ComponentType>(1001)));
    auto gridComp = std::dynamic_pointer_cast<GridComponent>(
        _componentManager->getComponentByType(gridEntity,
            static_cast<ComponentType>(1000)));

    return std::make_pair(snakeComp, gridComp);
}

Arcade::Entity UISystem::findSnakeHeadEntity() const {
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto component = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1001));
        if (component)
            return entity;
    }
    return 0;
}

Arcade::Entity UISystem::findGridEntity() const {
    for (const auto& [entity, name] : _entityManager->getEntities()) {
        auto component = _componentManager->getComponentByType(entity,
            static_cast<ComponentType>(1000));
        if (component)
            return entity;
    }
    return 0;
}

}  // namespace Snake
}  // namespace Arcade
