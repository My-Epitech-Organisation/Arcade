// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Temp test
*/

#include <gtest/gtest.h>
#include "../../../../src/Shared/Components/Velocity.hpp"

class VelocityTest : public ::testing::Test {
 protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(VelocityTest, DefaultConstructor) {
    VelocityComponent velocity;
    EXPECT_EQ(0, velocity.dx);
    EXPECT_EQ(0, velocity.dy);
}

TEST_F(VelocityTest, ParameterizedConstructor) {
    VelocityComponent velocity(3, 5);
    EXPECT_EQ(3, velocity.dx);
    EXPECT_EQ(5, velocity.dy);
}

TEST_F(VelocityTest, MemberAccess) {
    VelocityComponent velocity;
    velocity.dx = 10;
    velocity.dy = 20;
    EXPECT_EQ(10, velocity.dx);
    EXPECT_EQ(20, velocity.dy);
}

TEST_F(VelocityTest, NegativeValues) {
    VelocityComponent velocity(-2, -8);
    EXPECT_EQ(-2, velocity.dx);
    EXPECT_EQ(-8, velocity.dy);
}

TEST_F(VelocityTest, ZeroVelocity) {
    VelocityComponent velocity(0, 0);
    EXPECT_EQ(0, velocity.dx);
    EXPECT_EQ(0, velocity.dy);
}

