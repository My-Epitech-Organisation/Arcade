// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400-NAN-4-1-arcade-marin.lamy
** File description:
** windowManager
*/
/**
 * @file WindowManager.hpp
 * @brief Manages window properties for the Arcade project.
 *
 * The WindowManager class handles various attributes of a graphical window, 
 * such as its size, title, frame rate, and icon.
 */

#ifndef SRC_SHARED_MODELS_WINDOWMANAGER_WINDOWMANAGER_HPP_
    #define SRC_SHARED_MODELS_WINDOWMANAGER_WINDOWMANAGER_HPP_
    #include <string>

namespace Arcade {
/**
 * @struct Vector2i
 * @brief Represents a 2D vector with integer coordinates.
 *
 * This structure is used for handling 2D positions or sizes.
 */
struct Vector2i {
    int x;  ///< X-coordinate or width.
    int y;  ///< Y-coordinate or height.
    /**
     * @brief Constructs a Vector2i object with default or specified values.
     *
     * @param x The X-coordinate (default is 0).
     * @param y The Y-coordinate (default is 0).
     */
    explicit Vector2i(int x = 0, int y = 0) : x(x), y(y) {}
};

/**
 * @class WindowManager
 * @brief Handles window properties such as size, title, frame rate, and icon.
 *
 * The WindowManager class provides getter and setter methods for managing
 * window dimensions, FPS settings, window title, and associated icon.
 */
class WindowManager {
 public:
    /**
     * @brief Retrieves the height of the window.
     * @return The window height in pixels.
     */
    int getHeight() const;

    /**
     * @brief Retrieves the width of the window.
     * @return The window width in pixels.
     */
    int getWidth() const;

    /**
     * @brief Retrieves the frames per second (FPS) setting.
     * @return The current FPS value.
     */
    float getFps() const;

    /**
     * @brief Retrieves the window title.
     * @return A string containing the window title.
     */
    std::string getTitle() const;

    /**
     * @brief Retrieves the file path of the window icon.
     * @return A string representing the path to the icon file.
     */
    std::string getIconPath() const;

    /**
     * @brief Retrieves the size of the window.
     * @return A Vector2i representing the window dimensions (width, height).
     */
    Vector2i getWindowSize() const;

    /**
     * @brief Sets the height of the window.
     * @param height The new height value in pixels.
     */
    void setHeight(int height);

    /**
     * @brief Sets the width of the window.
     * @param width The new width value in pixels.
     */
    void setWidth(int width);

    /**
     * @brief Sets the frames per second (FPS) value.
     * @param fps The new FPS value.
     */
    void setFps(float fps);

    /**
     * @brief Sets the title of the window.
     * @param title A string containing the new window title.
     */
    void setTitle(const std::string &title);

    /**
     * @brief Sets the file path of the window icon.
     * @param iconPath A string representing the path to the new icon file.
     */
    void setIconPath(const std::string &iconPath);

    /**
     * @brief Sets the window size.
     * @param vec A Vector2i representing the new window dimensions (width, height).
     */
    void setWindowSize(Vector2i vec);

 private:
    int _width;  /// Width of the window in pixels.
    int _height;  /// Height of the window in pixels.
    float _fps;  /// Frames per second (FPS) setting.
    std::string _title;  /// Title of the window.
    std::string _iconPath;  /// Path to the window icon file.
};

}  // namespace Arcade

#endif  // SRC_SHARED_MODELS_WINDOWMANAGER_WINDOWMANAGER_HPP_
