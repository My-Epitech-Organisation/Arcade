// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Temp test
*/
#include <gtest/gtest.h>
#include "../src/Games/Game1/Game1.hpp"

class Game1Test : public ::testing::Test, public Game1 {};

TEST_F(Game1Test, printHelp) {
    testing::internal::CaptureStdout();
    printHelp();
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "USAGE: ./arcade lib_name\n");
}
