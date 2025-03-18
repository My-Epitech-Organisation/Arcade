// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Main function
*/
#include <iostream>

/**
 * @brief Main function
 *
 * This function is the entry point of the program.
 *
 * @param ac Number of arguments
 * @param av Arguments
 * @return int Return value
 */
int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr
            << "Usage: ./arcade path_to_graphical_library.so"
            << std::endl;
        return 84;
    }
    return 0;
}
