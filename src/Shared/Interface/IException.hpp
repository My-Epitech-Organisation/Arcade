// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** ARcade Module interface
*/

#ifndef ARCADE_IEXCEPTION_HPP_
#define ARCADE_IEXCEPTION_HPP_

#include <string>
#include <exception>

namespace arcade {

class IException : public std::exception {
 public:
  virtual ~IException() noexcept = default;
  virtual const char* what() const noexcept override = 0;
  virtual std::string getType() const noexcept = 0;
  virtual std::string getMessage() const noexcept = 0;
  virtual std::string getFormattedMessage() const noexcept = 0;
};
}  // namespace arcade

#endif  // ARCADE_IEXCEPTION_HPP_
