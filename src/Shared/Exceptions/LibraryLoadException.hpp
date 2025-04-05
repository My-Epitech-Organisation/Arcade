/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Exception class for library loading errors
*/

#ifndef ARCADE_LIBRARYLOADEXCEPTION_HPP_
#define ARCADE_LIBRARYLOADEXCEPTION_HPP_

#include "ArcadeException.hpp"

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
        : ArcadeException("Library Error: " + message) {}
};

}  // namespace Arcade

#endif  // ARCADE_LIBRARYLOADEXCEPTION_HPP_
