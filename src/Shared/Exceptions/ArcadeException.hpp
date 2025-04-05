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

class ArcadeException : public std::exception {
 public:
    explicit ArcadeException(const std::string& message);
    const char* what() const noexcept override;

 private:
    std::string message_;
};

}  // namespace Arcade

#endif  // ARCADE_ARCADEEXCEPTION_HPP_
