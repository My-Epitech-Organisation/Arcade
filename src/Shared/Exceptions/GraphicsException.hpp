// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Exception class for graphics rendering errors
*/

#ifndef SRC_SHARED_EXCEPTIONS_GRAPHICSEXCEPTION_HPP_
#define SRC_SHARED_EXCEPTIONS_GRAPHICSEXCEPTION_HPP_

#include <string>
#include "Core/Exceptions/Exception.hpp"

namespace Arcade {

/**
 * @brief Exception class for graphics rendering and initialization errors
 */
class GraphicsException : public ArcadeException {
 public:
    /**
     * @brief Constructor with error message
     * @param message The error message
     */
    explicit GraphicsException(const std::string& message)
        : ArcadeException("Graphics Error: " + message) {}
};

}  // namespace Arcade

#endif  // SRC_SHARED_EXCEPTIONS_GRAPHICSEXCEPTION_HPP_
