// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Paradigm pool
** File description:
** Seminar
*/

#ifndef SRC_SHARED_INTERFACE_IDISPLAYMODULE_HPP_
    #define SRC_SHARED_INTERFACE_IDISPLAYMODULE_HPP_
    #include <string>
    #include <vector>
    #include "Shared/Models/EntityComponentSystem/EntityManager.hpp"
    #include "Interface/IEventManager.hpp"

namespace Arcade {
class IDisplayModule {
 public:
    virtual ~IDisplayModule() = default;
    virtual void init() = 0;
    virtual IEventManager getInput(void) = 0;
    virtual void drawElement(const std::vector<Entity> &entities) = 0;
    virtual void render() = 0;
    virtual std::string getName() const = 0;
};
}  // namespace Arcade
#endif  // SRC_SHARED_INTERFACE_IDISPLAYMODULE_HPP_
