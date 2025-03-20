// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Temp test
*/
#include <gtest/gtest.h>
#include "../../../../src/Shared/Components/Sprite.hpp"

class SpritetTest : public ::testing::Test {
 protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SpritetTest, Constructor) {
    SpriteComponent sprite("assets/player.png");
    EXPECT_EQ("assets/player.png", sprite.path);
}

TEST_F(SpritetTest, EmptyPath) {
    SpriteComponent sprite("");
    EXPECT_EQ("", sprite.path);
}

TEST_F(SpritetTest, PathModification) {
    SpriteComponent sprite("original.png");
    sprite.path = "modified.png";
    EXPECT_EQ("modified.png", sprite.path);
}

TEST_F(SpritetTest, MoveSemantics) {
    std::string originalPath = "original_sprite.png";
    SpriteComponent sprite(std::move(originalPath));
    EXPECT_EQ("original_sprite.png", sprite.path);
    EXPECT_TRUE(originalPath.empty());
}
