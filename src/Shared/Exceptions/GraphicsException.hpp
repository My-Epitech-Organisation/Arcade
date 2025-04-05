/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Exception class for graphics rendering errors
*/

#ifndef ARCADE_GRAPHICSEXCEPTION_HPP_
#define ARCADE_GRAPHICSEXCEPTION_HPP_

#include "ArcadeException.hpp"

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

#endif  // ARCADE_GRAPHICSEXCEPTION_HPP_
