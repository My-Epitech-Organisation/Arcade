// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Entity Interface
*/

#ifndef SRC_SHARED_INTERFACE_ECS_IENTITY_HPP_
    #define SRC_SHARED_INTERFACE_ECS_IENTITY_HPP_

namespace Arcade {
class IEntity {
 public:
    virtual ~IEntity() = default;
    virtual int getId() const = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ECS_IENTITY_HPP_
