/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Exception class for resource loading errors
*/

#ifndef ARCADE_RESOURCEEXCEPTION_HPP_
#define ARCADE_RESOURCEEXCEPTION_HPP_

#include "ArcadeException.hpp"

namespace Arcade {

/**
 * @brief Exception class for resource (assets, files) loading errors
 */
class ResourceException : public ArcadeException {
 public:
    /**
     * @brief Constructor with error message
     * @param message The error message
     */
    explicit ResourceException(const std::string& message)
        : ArcadeException("Resource Error: " + message) {}
};

}  // namespace Arcade

#endif  // ARCADE_RESOURCEEXCEPTION_HPP_
