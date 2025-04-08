// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** Implementation of Base Exception class for Arcade project
*/

#include "Core/Exceptions/Exception.hpp"
#include <string>

namespace Arcade {

ArcadeException::ArcadeException(const std::string& message)
: message_(message) {
}

const char* ArcadeException::what() const noexcept {
return message_.c_str();
}

}  // namespace Arcade
