/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Base Exception class for Arcade project
*/

#ifndef ARCADE_ARCADEEXCEPTION_HPP_
#define ARCADE_ARCADEEXCEPTION_HPP_

#include <exception>
#include <string>

namespace Arcade {

/**
 * @brief Base exception class for all Arcade-related exceptions
 */
class ArcadeException : public std::exception {
 public:
    /**
     * @brief Constructor with error message
     * @param message The error message
     */
    explicit ArcadeException(const std::string& message);

    /**
     * @brief Get the error message
     * @return The error message
     */
    const char* what() const noexcept override;

 private:
    std::string message_;
};

}  // namespace Arcade

#endif  // ARCADE_ARCADEEXCEPTION_HPP_
