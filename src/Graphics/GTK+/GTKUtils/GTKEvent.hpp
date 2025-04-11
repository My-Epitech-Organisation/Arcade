// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKEvent
*/

#ifndef SRC_GRAPHICS_GTK__GTKUTILS_GTKEVENT_HPP_
    #define SRC_GRAPHICS_GTK__GTKUTILS_GTKEVENT_HPP_
    #include <gtk/gtk.h>
    #include <memory>
    #include <vector>
    #include <utility>
    #include <map>
    #include <set>

namespace GTK {

class GTKEventCallback;
class GTKEvent {
 private:
    GtkWidget *_window;
    std::map<int, bool> _keyStates;
    std::map<int, bool> _mouseButtonStates;
    size_t _mouseX;
    size_t _mouseY;
    std::vector<gulong> _signalHandlerIds;
    std::shared_ptr<GTKEventCallback> _callbackHandler;
    void connectSignals();

 public:
    explicit GTKEvent(GtkWidget *window = nullptr);
    ~GTKEvent();

    GTKEvent(const GTKEvent&) = delete;
    GTKEvent& operator=(const GTKEvent&) = delete;

    GTKEvent(GTKEvent&& other) noexcept;
    GTKEvent& operator=(GTKEvent&& other) noexcept;

    void handleEvents();
    void resetEvents();
    bool isKeyPressed(int keyCode) const;
    bool isMouseButtonPressed(int button) const;
    std::pair<size_t, size_t> getMousePosition() const;
    void setKeyPressed(guint keyval);
    void setKeyReleased(guint keyval);
    void setMouseButtonPressed(guint button);
    void setMouseButtonReleased(guint button);
    void setMousePosition(double x, double y);
    void disconnectSignals();
    void addSignalHandler(gulong handlerId);
};

class GTKEventCallback : public std::enable_shared_from_this<GTKEventCallback> {
 private:
    std::weak_ptr<GTKEventCallback> _self;
    GTKEvent* _event;

 public:
    explicit GTKEventCallback(GTKEvent* event);
    ~GTKEventCallback() = default;

    void setEvent(GTKEvent* event);

    static void keyPressed(GtkEventControllerKey* controller, guint keyval,
      guint keycode, GdkModifierType state, gpointer data);

    static void keyReleased(GtkEventControllerKey* controller, guint keyval,
      guint keycode, GdkModifierType state, gpointer data);

    static void mousePressed(GtkGestureClick* gesture, int n_press, double x,
      double y, gpointer data);

    static void mouseReleased(GtkGestureClick* gesture, int n_press, double x,
      double y, gpointer data);

    static void mouseMoved(GtkEventControllerMotion* controller, double x,
      double y, gpointer data);
};

}  // namespace GTK

#endif  // SRC_GRAPHICS_GTK__GTKUTILS_GTKEVENT_HPP_
