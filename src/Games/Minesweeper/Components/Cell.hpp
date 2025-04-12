// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Minesweeper Cell class
*/

#ifndef SRC_GAMES_MINESWEEPER_COMPONENTS_CELL_HPP_
    #define SRC_GAMES_MINESWEEPER_COMPONENTS_CELL_HPP_
    #include <string>
    #include <memory>
    #include "Shared/Interface/ECS/IComponent.hpp"
    #include "Shared/Models/ComponentType.hpp"

namespace Arcade {
namespace Minesweeper {

class Cell : public Arcade::IComponent {
 public:
    explicit Cell(bool hasMine = false, int adjacentMines = 0)
        : _state(HIDDEN), _adjacentMines(adjacentMines), _hasMine(hasMine) {}
    ~Cell() = default;

    enum State {
        HIDDEN,
        REVEALED,
        FLAGGED
    };
    size_t _gridX;
    size_t _gridY;
    void setName(const std::string& name) { _name = name; }
    std::string getName() const { return _name; }
    ComponentType getType() const override {
        return ComponentType::CELL;
    }
    void setAdjacentMines(int count) { _adjacentMines = count; }
    int getAdjacentMines() const { return _adjacentMines; }
    void setState(State state) { _state = state; }
    State getState() const { return _state; }
    bool isRevealed() const { return _state == REVEALED; }
    bool isFlagged() const { return _state == FLAGGED; }
    bool isHidden() const { return _state == HIDDEN; }
    bool hasMine() const { return _hasMine; }
    void setHasMine(bool hasMine) { _hasMine = hasMine; }

 private:
    std::string _name;
    State _state;
    int _adjacentMines;
    bool _hasMine;
};

}  // namespace Minesweeper
}  // namespace Arcade

#endif  // SRC_GAMES_MINESWEEPER_COMPONENTS_CELL_HPP_
