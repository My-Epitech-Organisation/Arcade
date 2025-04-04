// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** KeyEvent
*/

#ifndef SRC_SHARED_EVENTMANAGER_KEYEVENT_MOUSEEVENT_HPP_
    #define SRC_SHARED_EVENTMANAGER_KEYEVENT_MOUSEEVENT_HPP_

    #include "Shared/Interface/Core/IEvent.hpp"
    #include "Shared/Models/EventType.hpp"
    #include "Shared/Models/MouseButtonType.hpp"

namespace Arcade {
class MouseEvent : public IEvent {
 public:
    MouseEvent(MouseButton button, EventType type, size_t x, size_t y)
        : _button(button), _eventType(type), _x(x), _y(y) {}
    EventType getType() const override { return _eventType; }
    Keys getKey() const override { return Keys::NONE; }
    MouseButton getButton() const { return _button; }
    size_t getX() const { return _x; }
    size_t getY() const { return _y; }
    bool isType(EventType type) const {
        return _eventType == type;
    }
 private:
    MouseButton _button;
    EventType _eventType;
    size_t _x;
    size_t _y;
};
}  // namespace Arcade

#endif  // SRC_SHARED_EVENTMANAGER_KEYEVENT_MOUSEEVENT_HPP_
