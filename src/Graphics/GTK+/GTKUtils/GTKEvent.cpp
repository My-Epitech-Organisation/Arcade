// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKEvent implementation
*/
#include <utility>
#include <memory>
#include <iostream>
#include "GTKUtils/GTKEvent.hpp"
#include "GTK+/GTKKeys.hpp"
#include "Models/KeysType.hpp"

namespace GTK {

GTKEventCallback::GTKEventCallback(GTKEvent* event) : _event(event) {}

void GTKEventCallback::setEvent(GTKEvent* event) {
    _event = event;
}

void GTKEventCallback::keyPressed(GtkEventControllerKey* controller,
guint keyval, guint keycode,
GdkModifierType state, gpointer data) {
    auto callback = static_cast<GTKEventCallback*>(data);
    if (callback && callback->_event) {
        callback->_event->setKeyPressed(keyval);
    }
}

void GTKEventCallback::keyReleased(GtkEventControllerKey* controller,
guint keyval, guint keycode,
GdkModifierType state, gpointer data) {
    auto callback = static_cast<GTKEventCallback*>(data);
    if (callback && callback->_event) {
        callback->_event->setKeyReleased(keyval);
    }
}

void GTKEventCallback::mousePressed(GtkGestureClick* gesture,
int n_press, double x, double y, gpointer data) {
    auto callback = static_cast<GTKEventCallback*>(data);
    if (callback && callback->_event) {
        guint button = gtk_gesture_single_get_current_button(
        GTK_GESTURE_SINGLE(gesture));
        callback->_event->setMouseButtonPressed(button);
    }
}

void GTKEventCallback::mouseReleased(GtkGestureClick* gesture,
int n_press, double x, double y, gpointer data) {
    auto callback = static_cast<GTKEventCallback*>(data);
    if (callback && callback->_event) {
        guint button = gtk_gesture_single_get_current_button(
        GTK_GESTURE_SINGLE(gesture));
        callback->_event->setMouseButtonReleased(button);
    }
}

void GTKEventCallback::mouseMoved(GtkEventControllerMotion* controller,
double x, double y, gpointer data) {
    auto callback = static_cast<GTKEventCallback*>(data);
    if (callback && callback->_event) {
        callback->_event->setMousePosition(x, y);
    }
}

void GTKEvent::disconnectSignals() {
    if (_window) {
        for (gulong id : _signalHandlerIds) {
            if (id > 0) {
                g_signal_handler_disconnect(_window, id);
            }
        }
        _signalHandlerIds.clear();
    }

    _window = nullptr;
}

void GTKEvent::connectSignals() {
    if (!_window) return;

    // Create callback handler if needed
    if (!_callbackHandler) {
        _callbackHandler = std::make_shared<GTKEventCallback>(this);
    } else {
        _callbackHandler->setEvent(this);
    }

    GtkEventController *keyController = gtk_event_controller_key_new();
    gtk_widget_add_controller(_window, keyController);

    gulong id1 = g_signal_connect(keyController, "key-pressed",
        G_CALLBACK(GTKEventCallback::keyPressed), _callbackHandler.get());
    _signalHandlerIds.push_back(id1);

    gulong id2 = g_signal_connect(keyController, "key-released",
        G_CALLBACK(GTKEventCallback::keyReleased), _callbackHandler.get());
    _signalHandlerIds.push_back(id2);

    GtkGesture *clickController = gtk_gesture_click_new();
    gtk_widget_add_controller(_window, GTK_EVENT_CONTROLLER(clickController));

    gulong id3 = g_signal_connect(clickController, "pressed",
        G_CALLBACK(GTKEventCallback::mousePressed), _callbackHandler.get());
    _signalHandlerIds.push_back(id3);

    gulong id4 = g_signal_connect(clickController, "released",
        G_CALLBACK(GTKEventCallback::mouseReleased), _callbackHandler.get());
    _signalHandlerIds.push_back(id4);

    GtkEventController *motionController = gtk_event_controller_motion_new();
    gtk_widget_add_controller(_window, motionController);

    gulong id5 = g_signal_connect(motionController, "motion",
        G_CALLBACK(GTKEventCallback::mouseMoved), _callbackHandler.get());
    _signalHandlerIds.push_back(id5);
}

GTKEvent::GTKEvent(GtkWidget *window) : _window(window),
_mouseX(0),
_mouseY(0) {
    _keyStates.clear();
    _mouseButtonStates.clear();
    _signalHandlerIds.clear();
    _callbackHandler = std::make_shared<GTKEventCallback>(this);
    if (window) {
        _window = window;
        connectSignals();
    }
    // connectSignals();
}

GTKEvent::~GTKEvent() {
    disconnectSignals();
    _callbackHandler.reset();
}

GTKEvent::GTKEvent(GTKEvent&& other) noexcept
: _window(other._window),
_keyStates(std::move(other._keyStates)),
_mouseButtonStates(std::move(other._mouseButtonStates)),
_mouseX(other._mouseX),
_mouseY(other._mouseY),
_signalHandlerIds(std::move(other._signalHandlerIds)) {
    if (_callbackHandler) {
        _callbackHandler->setEvent(this);
    }

    other._window = nullptr;
    other._signalHandlerIds.clear();
}

GTKEvent& GTKEvent::operator=(GTKEvent&& other) noexcept {
    if (this != &other) {
        disconnectSignals();

        _window = other._window;
        _keyStates = std::move(other._keyStates);
        _mouseButtonStates = std::move(other._mouseButtonStates);
        _mouseX = other._mouseX;
        _mouseY = other._mouseY;
        _signalHandlerIds = std::move(other._signalHandlerIds);
        _callbackHandler = std::move(other._callbackHandler);

        if (_callbackHandler) {
            _callbackHandler->setEvent(this);
        }

        other._window = nullptr;
        other._signalHandlerIds.clear();
    }
    return *this;
}

void GTKEvent::handleEvents() {
    while (g_main_context_iteration(nullptr, FALSE)) {}
}

bool GTKEvent::isKeyPressed(int keyCode) const {
    auto arcadeKey = static_cast<Arcade::Keys>(keyCode);
    guint gtkKey = Arcade::GTKKeyMap::getGTKKey(arcadeKey);

    if (gtkKey == 0)
        return false;

    auto it = _keyStates.find(gtkKey);
    if (it != _keyStates.end()) {
        return it->second;
    }
    return false;
}

bool GTKEvent::isMouseButtonPressed(int button) const {
    auto arcadeButton = static_cast<Arcade::MouseButton>(button);
    guint gtkButton = Arcade::GTKKeyMap::getGTKButton(arcadeButton);

    if (gtkButton == 0) {
        return false;
    }

    auto it = _mouseButtonStates.find(gtkButton);
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

void GTKEvent::setKeyPressed(guint keyval) {
    if (keyval > 0 && keyval < 65536) {
        _keyStates[keyval] = true;
    }
}

void GTKEvent::setKeyReleased(guint keyval) {
    if (keyval > 0 && keyval < 65536) {
        _keyStates[keyval] = false;
    }
}

void GTKEvent::setMouseButtonPressed(guint button) {
    _mouseButtonStates[button] = true;
}

void GTKEvent::setMouseButtonReleased(guint button) {
    _mouseButtonStates[button] = false;
}

void GTKEvent::setMousePosition(double x, double y) {
    _mouseX = static_cast<size_t>(x);
    _mouseY = static_cast<size_t>(y);
}

}  // namespace GTK
