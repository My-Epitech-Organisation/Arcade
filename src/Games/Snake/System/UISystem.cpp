// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** UI system implementation for Snake game
*/

#include <sstream>
#include <iomanip>
#include "Games/Snake/System/UISystem.hpp"
#include "Games/Snake/Components/GameStats.hpp"
#include "ECS/Components/Text.hpp"
#include "ECS/Components/Position.hpp"
#include "Shared/Interface/Core/Clock.hpp"

namespace Arcade {

UISystem::UISystem(std::shared_ptr<IComponentManager> componentManager,
                 std::shared_ptr<IEntityManager> entityManager)
    : _componentManager(componentManager),
      _entityManager(entityManager) {
    // Create score text entity
    _scoreTextEntity = _entityManager->createEntity();
    Text scoreText;
    scoreText.content = "Score: 0";
    scoreText.fontSize = 20;
    scoreText.fontPath = "assets/fonts/arial.ttf";
    Position scorePos;
    scorePos.x = 10;
    scorePos.y = 10;

    _componentManager->addComponent(_scoreTextEntity, scoreText);
    _componentManager->addComponent(_scoreTextEntity, scorePos);

    // Create time text entity
    _timeTextEntity = _entityManager->createEntity();
    Text timeText;
    timeText.content = "Time: 0:00";
    timeText.fontSize = 20;
    timeText.fontPath = "assets/fonts/arial.ttf";
    Position timePos;
    timePos.x = 10;
    timePos.y = 40;

    _componentManager->addComponent(_timeTextEntity, timeText);
    _componentManager->addComponent(_timeTextEntity, timePos);
}

void UISystem::update() {
    updateScoreDisplay();
    updateTimeDisplay();
}

void UISystem::updateScoreDisplay() {
    auto statsEntities = _entityManager->getEntitiesWithComponent<GameStats>();
    if (statsEntities.empty()) return;

    auto stats = _componentManager->getComponent<GameStats>(statsEntities[0]);
    auto scoreText = _componentManager->getComponent<Text>(_scoreTextEntity);

    std::stringstream ss;
    ss << "Score: " << stats->score;
    scoreText->content = ss.str();
}

void UISystem::updateTimeDisplay() {
    auto statsEntities = _entityManager->getEntitiesWithComponent<GameStats>();
    if (statsEntities.empty()) return;

    auto stats = _componentManager->getComponent<GameStats>(statsEntities[0]);
    auto timeText = _componentManager->getComponent<Text>(_timeTextEntity);

    // Update time in stats
    stats->time += Clock::getDeltaTime();

    // Format time as minutes:seconds
    int minutes = static_cast<int>(stats->time) / 60;
    int seconds = static_cast<int>(stats->time) % 60;

    std::stringstream ss;
    ss << "Time: " << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
    timeText->content = ss.str();
}

}  // namespace Arcade
