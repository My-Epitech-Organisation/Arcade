// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Exception class for library loading errors
*/

#ifndef SRC_SHARED_EXCEPTIONS_LIBRARYLOADEXCEPTION_HPP_
#define SRC_SHARED_EXCEPTIONS_LIBRARYLOADEXCEPTION_HPP_

#include <string>
#include "Core/Exceptions/Exception.hpp"

namespace Arcade {

/**
 * @brief Exception class for errors related to dynamic library loading
 */
class LibraryLoadException : public ArcadeException {
 public:
    /**
     * @brief Constructor with error message
     * @param message The error message
     */
    explicit LibraryLoadException(const std::string& message)
        : ArcadeException("Library Error: " + message) {
        _type = "LibraryLoadException";
    }
};

}  // namespace Arcade

#endif  // SRC_SHARED_EXCEPTIONS_LIBRARYLOADEXCEPTION_HPP_
