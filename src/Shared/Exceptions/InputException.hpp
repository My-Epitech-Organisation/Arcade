// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Exception class for input handling errors
*/

#ifndef SRC_SHARED_EXCEPTIONS_INPUTEXCEPTION_HPP_
#define SRC_SHARED_EXCEPTIONS_INPUTEXCEPTION_HPP_

#include <string>
#include "Core/Exceptions/ArcadeException.hpp"

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

#endif  // SRC_SHARED_EXCEPTIONS_INPUTEXCEPTION_HPP_
