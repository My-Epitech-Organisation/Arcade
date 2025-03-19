// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** ScoreManager
*/

#ifndef SRC_CORE_SCOREMANAGER_SCOREMANAGER_HPP_
    #define SRC_CORE_SCOREMANAGER_SCOREMANAGER_HPP_
    #include <string>
    #include <vector>
    #include "../../Shared/Interface/IGameModule.hpp"

    class scoreManager{
     public:
        scoreManager() {}
        ~scoreManager() {}
        //! call at start of program
        std::vector<std::pair<std::string, std::size_t>> getAllScore();
        //! call at end of game to store score
        void storeScore(std::unique_ptr<Arcade::IGameModule> game);
        std::size_t getScore(std::unique_ptr<Arcade::IGameModule> game);
     private:
        //! another vector with the game name i.e('arcade_minesweeper.so')
        //! as the first parameter of the pair and teh score as the second
        //! parameter of the pair
        std::vector<std::pair<std::string, std::size_t>> _scores;
    };

#endif  // SRC_CORE_SCOREMANAGER_SCOREMANAGER_HPP_
