// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Score provider implementation
*/

#ifndef SRC_SHARED_SCOREPROVIDER_SCOREPROVIDER_HPP_
    #define SRC_SHARED_SCOREPROVIDER_SCOREPROVIDER_HPP_

    #include <string>
    #include "Shared/Interface/Game/IScoreProvider.hpp"

namespace Arcade {
class ScoreProvider : public IScoreProvider {
 public:
    ScoreProvider();
    explicit ScoreProvider(const std::string& name);
    ~ScoreProvider() override = default;

    int getScore() const override;
    void resetScore() override;
    void setScore(int score) override;
    std::string getFormattedScore() const override;
    void addToScore(int add) override;
    void setName(const std::string& name) override;
    std::string getName() const override;
    void saveToFile(const std::string& filename,
        const std::string& gamename) const override {};

 private:
    int _score;
    std::string _playerName;
};
}  // namespace Arcade

#endif  // SRC_SHARED_SCOREPROVIDER_SCOREPROVIDER_HPP_
