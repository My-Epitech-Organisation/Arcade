// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** KeyEvent
*/

#ifndef SRC_SHARED_EVENTMANAGER_KEYEVENT_KEYEVENT_HPP_
    #define SRC_SHARED_EVENTMANAGER_KEYEVENT_KEYEVENT_HPP_

    #include "Shared/Interface/Core/IEvent.hpp"
    #include "Shared/Models/EventType.hpp"
    #include "Shared/Models/KeysType.hpp"

namespace Arcade {
class KeyEvent : public IEvent {
 public:
    KeyEvent(Keys key, EventType type)
        : _key(key), _eventType(type) {}
    EventType getType() const override { return _eventType; }
    Keys getKey() const { return _key; }
    bool isType(EventType type) const {
        return _eventType == type;
    }
    MouseButton getMouseButton() const override { return MouseButton::NONE; }
    bool hasKeyInfo() const { return _key != Keys::NONE; }
    bool hasMouseInfo() const { return false; }

 private:
    Keys _key;
    EventType _eventType;
};

}  // namespace Arcade

#endif  // SRC_SHARED_EVENTMANAGER_KEYEVENT_KEYEVENT_HPP_
