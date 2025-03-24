// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Score provider interface
*/

#ifndef SRC_SHARED_INTERFACE_GAME_ISCOREPROVIDER_HPP_
    #define SRC_SHARED_INTERFACE_GAME_ISCOREPROVIDER_HPP_
    #include <string>

namespace Arcade {
class IScoreProvider {
 public:
    virtual ~IScoreProvider() = default;
    virtual int getScore() const = 0;
    virtual void resetScore() = 0;
    virtual void setScore(int score) = 0;
    virtual std::string getFormattedScore() const = 0;
};
}  // namespace Arcade
#endif  // SRC_SHARED_INTERFACE_GAME_ISCOREPROVIDER_HPP_
