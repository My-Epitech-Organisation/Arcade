/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Temp test
*/
#include <gtest/gtest.h>
#include "../src/temp/add.hpp"

TEST(AddTest, HandlesPositiveInput) {
    Add addObj;
    EXPECT_EQ(addObj.add(1, 2), 3);
    EXPECT_EQ(addObj.add(10, 20), 30);
}

TEST(AddTest, HandlesNegativeInput) {
    Add addObj;
    EXPECT_EQ(addObj.add(-1, -2), -3);
    EXPECT_EQ(addObj.add(-10, -20), -30);
}

TEST(AddTest, HandlesMixedInput) {
    Add addObj;
    EXPECT_EQ(addObj.add(-1, 2), 1);
    EXPECT_EQ(addObj.add(10, -20), -10);
}