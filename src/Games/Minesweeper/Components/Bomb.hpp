// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Bomb Component for Minesweeper
*/

#ifndef SRC_GAMES_MINESWEEPER_COMPONENTS_BOMB_HPP_
    #define SRC_GAMES_MINESWEEPER_COMPONENTS_BOMB_HPP_
    #include <memory>
    #include <string>
    #include "Shared/Interface/ECS/IComponent.hpp"
    #include "Shared/Models/ComponentType.hpp"
    #include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {
namespace Minesweeper {

class BombComponent : public Arcade::IComponent {
 public:
    BombComponent(std::shared_ptr<DrawableComponent> hiddenSprite,
                 std::shared_ptr<DrawableComponent> revealedSprite)
        : _hiddenSprite(hiddenSprite),
          _revealedSprite(revealedSprite),
          _isRevealed(false) {}

    BombComponent(const std::string& hiddenPath,
        const std::string& revealedPath) {
        _hiddenSprite = std::make_shared<DrawableComponent>();
        _hiddenSprite->setAsTexture(hiddenPath, 100.0f, 100.0f);
        _revealedSprite = std::make_shared<DrawableComponent>();
        _revealedSprite->setAsTexture(revealedPath, 100.0f, 100.0f);
        _isRevealed = false;
    }

    ComponentType getType() const {
        return ComponentType::BOMB;
    }

    std::shared_ptr<DrawableComponent> getActiveSprite() const {
        return _isRevealed ? _revealedSprite : _hiddenSprite;
    }

    void reveal() {
        _isRevealed = true;
    }

    bool isRevealed() const {
        return _isRevealed;
    }

 private:
    std::shared_ptr<DrawableComponent> _hiddenSprite;
    std::shared_ptr<DrawableComponent> _revealedSprite;
    bool _isRevealed;
};

}  // namespace Minesweeper
}  // namespace Arcade

#endif  // SRC_GAMES_MINESWEEPER_COMPONENTS_BOMB_HPP_
