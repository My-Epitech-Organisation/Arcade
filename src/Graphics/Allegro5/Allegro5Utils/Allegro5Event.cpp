// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5Event
*/
#include <stdexcept>
#include <iostream>
#include <memory>
#include <utility>
#include "Allegro5Utils/Allegro5Event.hpp"
#include "Allegro5/Allegro5Keys.hpp"
#include "Models/KeysType.hpp"

namespace Allegro {

Allegro5Event::Allegro5Event()
: _eventQueue(nullptr, al_destroy_event_queue), _running(true) {
}

void Allegro5Event::init() {
    _eventQueue = std::unique_ptr<ALLEGRO_EVENT_QUEUE,
        decltype(&al_destroy_event_queue)>(
            al_create_event_queue(), al_destroy_event_queue);
    if (!_eventQueue) {
        throw std::runtime_error("Failed to create event queue");
    }
    al_register_event_source(_eventQueue.get(), al_get_keyboard_event_source());
}

void Allegro5Event::pollEvents() {
    ALLEGRO_EVENT event;
    while (al_get_next_event(_eventQueue.get(), &event)) {
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            _running = false;
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                _running = false;
            }
        }
    }
}

bool Allegro5Event::isRunning() const {
    return _running;
}

void Allegro5Event::registerDisplayEventSource(ALLEGRO_DISPLAY* display) {
    if (_eventQueue && display) {
        al_register_event_source(_eventQueue.get(),
            al_get_display_event_source(display));
    }
}

void Allegro5Event::registerEventSource() const {
    al_get_mouse_state(&_mouseState);
    al_get_keyboard_state(&_keyState);
}

bool Allegro5Event::isKeyPressed(int keyCode) const {
    int allegroKey = Arcade::Allegro5KeyMap::getAllegroKey(
        static_cast<Arcade::Keys>(keyCode));
    return al_key_down(&_keyState, allegroKey);
}

bool Allegro5Event::isMouseButtonPressed(int buttonCode) const {
    int allegroButton = Arcade::Allegro5KeyMap::getAllegroButton(
        Arcade::MouseButton(buttonCode));
    return al_mouse_button_down(&_mouseState, allegroButton);
}

std::pair<size_t, size_t> Allegro5Event::getMousePosition() const {
    int x = 0;
    int y = 0;
    al_get_mouse_cursor_position(&x, &y);
    return {x, y};
}

}  // namespace Allegro
