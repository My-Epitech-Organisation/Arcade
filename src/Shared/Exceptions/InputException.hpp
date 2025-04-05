/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Exception class for input handling errors
*/

#ifndef ARCADE_INPUTEXCEPTION_HPP_
#define ARCADE_INPUTEXCEPTION_HPP_

#include "ArcadeException.hpp"

namespace Arcade {

/**
 * @brief Exception class for input handling errors
 */
class InputException : public ArcadeException {
 public:
    /**
     * @brief Constructor with error message
     * @param message The error message
     */
    explicit InputException(const std::string& message)
        : ArcadeException("Input Error: " + message) {}
};

}  // namespace Arcade

#endif  // ARCADE_INPUTEXCEPTION_HPP_
