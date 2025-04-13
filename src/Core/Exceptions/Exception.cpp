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
: _message(message), _type("ArcadeException") {
}

const char* ArcadeException::what() const noexcept {
    return _message.c_str();
}

std::string ArcadeException::getType() const noexcept {
    return _type;
}

std::string ArcadeException::getMessage() const noexcept {
    return _message;
}

std::string ArcadeException::getFormattedMessage() const noexcept {
    return getType() + ": " + getMessage();
}

}  // namespace Arcade
