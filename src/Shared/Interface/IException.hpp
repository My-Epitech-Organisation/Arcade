// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Exception Module interface
*/

/**
 * @file IException.hpp
 * @brief Interface for exception handling in the Arcade project
 */

#ifndef SRC_SHARED_INTERFACE_IEXCEPTION_HPP_
#define SRC_SHARED_INTERFACE_IEXCEPTION_HPP_

#include <string>
#include <exception>

namespace Arcade {

/**
 * @class IException
 * @brief Interface for all exceptions in the Arcade project
 *
 * This interface ensures all exceptions provide consistent functionality
 * for error reporting and handling throughout the application.
 */
class IException : public std::exception {
 public:
  /**
   * @brief Virtual destructor to ensure proper cleanup of derived classes
   */
  virtual ~IException() noexcept = default;

  /**
   * @brief Returns the error message associated with this exception
   * @return const char* containing the error message
   */
  const char* what() const noexcept override = 0;

  /**
   * @brief Returns the name of the exception type
   * @return std::string containing the exception type name
   */
  virtual std::string getType() const noexcept = 0;

  /**
   * @brief Returns the original error message passed to the constructor
   * @return std::string containing the original error message
   */
  virtual std::string getMessage() const noexcept = 0;

  /**
   * @brief Formats a full error message with type and details
   * @return std::string containing the formatted error message
   */
  virtual std::string getFormattedMessage() const noexcept = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_IEXCEPTION_HPP_
