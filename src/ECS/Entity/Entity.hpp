// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Entity Implementation
*/

#ifndef SRC_ECS_ENTITY_ENTITY_HPP_
    #define SRC_ECS_ENTITY_ENTITY_HPP_

    #include <string>
    #include "Shared/Interface/ECS/IEntity.hpp"

namespace Arcade {
class Entity : public IEntity {
 public:
    Entity();
    Entity(int id, const std::string& name);
    ~Entity() override = default;
    int getId() const override;
    const std::string getName() const override;
    void setName(const std::string& name) override;
    void setId(int id) override;

 private:
    int _id;
    std::string _name;
};
}  // namespace Arcade

#endif  // SRC_ECS_ENTITY_ENTITY_HPP_
