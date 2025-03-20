// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Temp test
*/
#include <gtest/gtest.h>
#include "../../../../src/Shared/Components/Position.hpp"

class PositionTest : public ::testing::Test {
 protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PositionTest, DefaultConstructor) {
    PositionComponent position;
    EXPECT_EQ(0, position.x);
    EXPECT_EQ(0, position.y);
}

TEST_F(PositionTest, ParameterizedConstructor) {
    PositionComponent position(5, 10);
    EXPECT_EQ(5, position.x);
    EXPECT_EQ(10, position.y);
}

TEST_F(PositionTest, MemberAccess) {
    PositionComponent position;
    position.x = 15;
    position.y = 25;
    EXPECT_EQ(15, position.x);
    EXPECT_EQ(25, position.y);
}

TEST_F(PositionTest, NegativeValues) {
    PositionComponent position(-3, -7);
    EXPECT_EQ(-3, position.x);
    EXPECT_EQ(-7, position.y);
}
