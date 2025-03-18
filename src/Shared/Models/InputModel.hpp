// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Input Model
*/
/**
 * @file InputModel.hpp
 * @brief Defines the InputModel class for handling user input in the Arcade project.
 *
 * This class provides an enumeration of possible user inputs and a method to retrieve them.
 */
#ifndef SRC_SHARED_MODELS_INPUTMODEL_HPP_
    #define SRC_SHARED_MODELS_INPUTMODEL_HPP_

/**
 * @class InputModel
 * @brief Represents a model for user input handling.
 *
 * The InputModel class defines an enumeration of possible inputs
 * and provides a method to retrieve the current input.
 */
class InputModel {
 public:
    /**
    * @enum Input
    * @brief Defines various user input types.
    *
    * This enumeration includes keyboard keys, mouse buttons,
    * and an UNKNOWN value for unrecognized inputs.
    */
    enum Input {
        UP, DOWN, LEFT, RIGHT,    ///< Directional movement keys
        ENTER, ESCAPE, SPACE,     ///< Special action keys
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O,
        P, Q, R, S, T, U, V, W, X, Y, Z,  ///< Alphabetic keys
        ZERO, ONE, TWO, THREE, FOUR, FIVE,
        SIX, SEVEN, EIGHT, NINE,  ///< Numeric keys
        UNKNOWN,                  ///< Unrecognized input
        RIGHT_CLICK, LEFT_CLICK, MIDDLE_CLICK  ///< Mouse buttons
    };
    /**
    * @brief Retrieves the current user input.
    *
    * This method returns the latest detected input from the user.
    * @return An Input enumeration value representing the current input.
    */
    Input getInput() const;
};

#endif  // SRC_SHARED_MODELS_INPUTMODEL_HPP_
