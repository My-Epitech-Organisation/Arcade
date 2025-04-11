// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Base Exception class for Arcade project
*/

#ifndef SRC_CORE_EXCEPTIONS_EXCEPTION_HPP_
#define SRC_CORE_EXCEPTIONS_EXCEPTION_HPP_

#include <string>
#include "Shared/Interface/IException.hpp"

namespace Arcade {

/**
 * @brief Base exception class for all Arcade-related exceptions
 */
class ArcadeException : public IException {
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

    /**
     * @brief Returns the name of the exception type
     * @return std::string containing the exception type name
     */
    std::string getType() const noexcept override;

    /**
     * @brief Returns the original error message passed to the constructor
     * @return std::string containing the original error message
     */
    std::string getMessage() const noexcept override;

    /**
     * @brief Formats a full error message with type and details
     * @return std::string containing the formatted error message
     */
    std::string getFormattedMessage() const noexcept override;

 protected:
    std::string _message;
    std::string _type;
};

}  // namespace Arcade

#endif  // SRC_CORE_EXCEPTIONS_EXCEPTION_HPP_
