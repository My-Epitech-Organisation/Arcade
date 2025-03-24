// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Window Module Interface
*/

#ifndef SRC_SHARED_INTERFACE_CORE_IINPUTMODULE_HPP_
    #define SRC_SHARED_INTERFACE_CORE_IINPUTMODULE_HPP_
    #include <string>


class IInputModule {
 public:
    virtual ~IInputModule() = default;
    virtual bool isKeyPressed(int keyCode) = 0;
};

#endif  // SRC_SHARED_INTERFACE_CORE_IINPUTMODULE_HPP_
