// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Allegro5Event
*/

#ifndef SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5EVENT_HPP_
    #define SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5EVENT_HPP_
    #include <allegro5/allegro.h>
    #include <memory>

namespace Allegro {

class Allegro5Event {
 private:
    std::unique_ptr<ALLEGRO_EVENT_QUEUE,
        decltype(&al_destroy_event_queue)> _eventQueue;
    bool _running = true;

 public:
    Allegro5Event();
    ~Allegro5Event() = default;

    void init();
    void pollEvents();
    bool isRunning() const;
    void registerDisplayEventSource(ALLEGRO_DISPLAY* display);
};

}  // namespace Allegro

#endif  // SRC_GRAPHICS_ALLEGRO5_ALLEGRO5UTILS_ALLEGRO5EVENT_HPP_
