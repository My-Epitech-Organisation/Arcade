// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKEvent implementation
*/

#include "GTKUtils/GTKEvent.hpp"

namespace GTK {

void GTKEvent::handleEvents() {
    while (g_main_context_iteration(nullptr, FALSE)) {}
}

}  // namespace GTK
