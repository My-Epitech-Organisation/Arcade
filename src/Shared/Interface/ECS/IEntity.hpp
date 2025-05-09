// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Entity Interface
*/

#ifndef SRC_SHARED_INTERFACE_ECS_IENTITY_HPP_
    #define SRC_SHARED_INTERFACE_ECS_IENTITY_HPP_
    #include <string>
    #include <memory>

namespace Arcade {
class IEntity {
 public:
    virtual ~IEntity() = default;
    virtual int getId() const = 0;
    virtual const std::string getName() const = 0;
    virtual void setName(const std::string& name) = 0;
    virtual void setId(int id) = 0;
};
}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_ECS_IENTITY_HPP_
