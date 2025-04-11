// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Exception class for resource loading errors
*/

#ifndef SRC_SHARED_EXCEPTIONS_RESOURCEEXCEPTION_HPP_
#define SRC_SHARED_EXCEPTIONS_RESOURCEEXCEPTION_HPP_

#include <string>
#include "Core/Exceptions/Exception.hpp"

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
        : ArcadeException("Resource Error: " + message) {
        _type = "ResourceException";
    }
};

}  // namespace Arcade

#endif  // SRC_SHARED_EXCEPTIONS_RESOURCEEXCEPTION_HPP_
