// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKEvent implementation
*/

#include "GTKUtils/GTKEvent.hpp"
#include <utility>

namespace GTK {

GTKEvent::GTKEvent(GtkWidget *window) : _window(window),
_mouseX(0),
_mouseY(0) {
    _keyStates.clear();
    _mouseButtonStates.clear();
    if (_window) {
        GtkEventController *keyController = gtk_event_controller_key_new();
        gtk_widget_add_controller(_window, keyController);
        g_signal_connect(keyController, "key-pressed",
            G_CALLBACK(+[](GtkEventControllerKey *controller,
                        guint keyval, guint keycode,
                        GdkModifierType state, gpointer data) -> gboolean {
                GTKEvent *self = static_cast<GTKEvent*>(data);
                self->_keyStates[keyval] = true;
                return FALSE;
            }), this);
        g_signal_connect(keyController, "key-released",
            G_CALLBACK(+[](GtkEventControllerKey *controller,
                        guint keyval, guint keycode,
                        GdkModifierType state, gpointer data) -> gboolean {
                GTKEvent *self = static_cast<GTKEvent*>(data);
                self->_keyStates[keyval] = false;
                return FALSE;
            }), this);
        GtkGesture *clickController = gtk_gesture_click_new();
        gtk_widget_add_controller(_window,
            GTK_EVENT_CONTROLLER(clickController));
        g_signal_connect(clickController, "pressed",
            G_CALLBACK(+[](GtkGestureClick *gesture,
                        int n_press, double x, double y, gpointer data) {
                GTKEvent *self = static_cast<GTKEvent*>(data);
                guint button = gtk_gesture_single_get_current_button(
                    GTK_GESTURE_SINGLE(gesture));
                self->_mouseButtonStates[button] = true;
            }), this);
        g_signal_connect(clickController, "released",
            G_CALLBACK(+[](GtkGestureClick *gesture,
                        int n_press, double x, double y, gpointer data) {
                GTKEvent *self = static_cast<GTKEvent*>(data);
                guint button = gtk_gesture_single_get_current_button(
                    GTK_GESTURE_SINGLE(gesture));
                self->_mouseButtonStates[button] = false;
            }), this);
        GtkEventController *motionController
            = gtk_event_controller_motion_new();
        gtk_widget_add_controller(_window, motionController);
        g_signal_connect(motionController, "motion",
            G_CALLBACK(+[](GtkEventControllerMotion *controller,
                        double x, double y, gpointer data) {
                GTKEvent *self = static_cast<GTKEvent*>(data);
                self->_mouseX = static_cast<size_t>(x);
                self->_mouseY = static_cast<size_t>(y);
            }), this);
    }
}

void GTKEvent::handleEvents() {
    while (g_main_context_iteration(nullptr, FALSE)) {}
}

bool GTKEvent::isKeyPressed(int keyCode) const {
    auto it = _keyStates.find(keyCode);
    if (it != _keyStates.end()) {
        return it->second;
    }
    return false;
}

bool GTKEvent::isMouseButtonPressed(int button) const {
    auto it = _mouseButtonStates.find(button);
    if (it != _mouseButtonStates.end()) {
        return it->second;
    }
    return false;
}

std::pair<size_t, size_t> GTKEvent::getMousePosition() const {
    return {_mouseX, _mouseY};
}

void GTKEvent::resetEvents() {
    _keyStates.clear();
    _mouseButtonStates.clear();
}

}  // namespace GTK
