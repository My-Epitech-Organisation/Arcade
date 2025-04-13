// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Window
*/

#ifndef SRC_CORE_WINDOW_WINDOW_HPP_
#define SRC_CORE_WINDOW_WINDOW_HPP_

#include <memory>
#include <string>
#include "Shared/Interface/Core/IWindowModule.hpp"
#include "Shared/Interface/Display/IDisplayModule.hpp"
#include "Shared/Models/KeysType.hpp"
#include "Shared/Models/ColorType.hpp"
#include "Shared/EventManager/KeyEvent/RawInputState.hpp"
#include "Core/EventManager/EventManager.hpp"

/**
 * @file Window.hpp
 * @brief Declaration of the Window class, which manages the main application window.
 */

namespace Arcade {

/**
 * @class Window
 * @brief Represents the main application window and handles window-related operations.
 *
 * The Window class is responsible for creating, managing, and interacting with the
 * application window. It provides functionality for resizing, closing, drawing, and
 * handling events. It also integrates with display and event management modules.
 */
class Window : public IWindowModule {
 public:
    /**
     * @brief Constructs a Window object.
     *
     * @param displayModule A shared pointer to the display module used for rendering.
     * @param eventManager A shared pointer to the event manager for handling input events.
     */
    explicit Window(std::shared_ptr<IDisplayModule> displayModule,
                    std::shared_ptr<IEventManager> eventManager);

    /**
     * @brief Destroys the Window object and releases resources.
     */
    ~Window();

    /**
     * @brief Creates a new window with the specified dimensions and title.
     *
     * @param width The width of the window in pixels.
     * @param height The height of the window in pixels.
     * @param title The title of the window.
     */
    void createWindow(int width, int height, const std::string &title) override;

    /**
     * @brief Resizes the window to the specified dimensions.
     *
     * @param width The new width of the window in pixels.
     * @param height The new height of the window in pixels.
     */
    void resizeWindow(int width, int height) override;

    /**
     * @brief Closes the window.
     */
    void closeWindow() override;

    /**
     * @brief Checks if the window is currently open.
     *
     * @return True if the window is open, false otherwise.
     */
    bool isWindowOpen() const override;

    /**
     * @brief Clears the screen, preparing it for new rendering.
     */
    void clearScreen();

    /**
     * @brief Refreshes the screen, displaying the rendered content.
     */
    void refreshScreen();

    /**
     * @brief Polls and processes input events.
     */
    void pollEvents();

    /**
     * @brief Checks if a specific key is currently pressed.
     *
     * @param keyCode The code of the key to check.
     * @return True if the key is pressed, false otherwise.
     */
    bool isKeyPressed(int keyCode);

    /**
     * @brief Gets the current width of the window.
     *
     * @return The width of the window in pixels.
     */
    int getWidth() const;

    /**
     * @brief Gets the current height of the window.
     *
     * @return The height of the window in pixels.
     */
    int getHeight() const;

    /**
     * @brief Gets the current display module used by the window.
     *
     * @return A pointer to the current display module.
     */
    IDisplayModule* getDisplayModule();

    /**
     * @brief Sets a new display module for the window.
     *
     * @param displayModule A shared pointer to the new display module.
     */
    void setDisplayModule(std::shared_ptr<IDisplayModule> displayModule);

    /**
     * @brief Gets the event manager associated with the window.
     *
     * @return A shared pointer to the event manager.
     */
    std::shared_ptr<IEventManager> getEventManager() const;
    void drawDrawable(const DrawableComponent& drawable);

 private:
    std::shared_ptr<IDisplayModule> _displayModule;
        /// The display module used for rendering.
    std::shared_ptr<IEventManager> _eventManager;
        /// The event manager for handling input events.
    int _width;  /// The width of the window in pixels.
    int _height;  /// The height of the window in pixels.
    std::string _title;  /// The title of the window.
    bool _isShuttingDown;
        /// Indicates if the window is in the process of shutting down.
    void updateMousePosition(std::shared_ptr<RawInputState> state);
    void updateMouseStates(std::shared_ptr<RawInputState> state);
    void updateKeyStates(std::shared_ptr<RawInputState> state);
    void processInputState();
    void initializeNewDisplayModule(
        std::shared_ptr<IDisplayModule> displayModule);
    void cleanupCurrentDisplayModule();
};

}  // namespace Arcade

#endif  // SRC_CORE_WINDOW_WINDOW_HPP_
