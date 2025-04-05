/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Exception class for game logic errors
*/

#ifndef ARCADE_GAMEEXCEPTION_HPP_
#define ARCADE_GAMEEXCEPTION_HPP_

#include "ArcadeException.hpp"

namespace Arcade {

/**
 * @brief Exception class for game logic and rule violations
 */
class GameException : public ArcadeException {
 public:
    /**
     * @brief Constructor with error message
     * @param message The error message
     */
    explicit GameException(const std::string& message)
        : ArcadeException("Game Error: " + message) {}
};

}  // namespace Arcade

#endif  // ARCADE_GAMEEXCEPTION_HPP_
