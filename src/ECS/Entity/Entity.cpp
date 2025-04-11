// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Entity Implementation
*/

#include "Entity/Entity.hpp"

namespace Arcade {

Entity::Entity()
    : _id(0), _name("")
{}

Entity::Entity(int id, const std::string& name)
    : _id(id), _name(name)
{}

int Entity::getId() const {
    return _id;
}

const std::string Entity::getName() const {
    return _name;
}

void Entity::setName(const std::string& name) {
    _name = name;
}

void Entity::setId(int id) {
    _id = id;
}

}  // namespace Arcade
