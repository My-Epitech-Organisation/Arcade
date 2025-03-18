// Copyright 2024 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Main function
*/
#include <iostream>
#include <fstream>
#include "main.hpp"

static bool check_path(const std::string &path)
{
    std::ifstream lib(path);

    if (path.size() < 4)
        return false;
    if (path.substr(path.size() - 3) != ".so")
        return false;
    if (lib.is_open() == false)
        return false;
    lib.close();
    return true;
}

/**
 * @brief Main function
 *
 * This function is the entry point of the program.
 *
 * @param ac Number of arguments
 * @param av Arguments
 * @return int Return value
 */
int main(int ac, char **av)
{
    if (ac != 2) {
        std::cerr
            << "Usage: ./arcade path_to_graphical_library.so"
            << std::endl;
        return 84;
    }
    // if (check_path(av[1]) == false) {
    //     std::cerr
    //         << "Error: Invalid path to graphical library"
    //         << std::endl;
    //     return 84;
    // }
    launch_main_menu();
    return 0;
}
