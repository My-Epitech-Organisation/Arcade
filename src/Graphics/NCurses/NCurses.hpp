// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCursesModule
*/

#ifndef SRC_GRAPHICS_NCURSES_NCURSES_HPP_
    #define SRC_GRAPHICS_NCURSES_NCURSES_HPP_
    #include <ncurses.h>
    #include <memory>
    #include <string>
    #include <map>
    #include <utility>
    #include "Interface/Display/IDisplayModule.hpp"
    #include "Models/ColorType.hpp"
    #include "NCursesUtils/NCursesWindow.hpp"
    #include "NCursesUtils/NCursesColor.hpp"
    #include "NCursesUtils/NCursesText.hpp"
    #include "NCursesUtils/NCursesEntity.hpp"
    #include "NCursesUtils/NCursesEvent.hpp"

class NCursesModule : public Arcade::IDisplayModule {
 private:
    std::string _name;
    NCurses::NCursesWindow _window;
    NCurses::NCursesColor _colorManager;
    NCurses::NCursesText _text;
    NCurses::NCursesEntity _entity;
    NCurses::NCursesEvent _event;
    int _windowWidth = 0;
    int _windowHeight = 0;
    bool _running = true;
    float _pixelToCharX = 1.0f;  // Ratio for X-axis conversion
    float _pixelToCharY = 1.0f;  // Ratio for Y-axis conversion
    int _referencePixelWidth = 800;  // Reference pixel width
    int _referencePixelHeight = 600;  // Reference pixel height
    int _verticalSpacing = 2;  // Vertical spacing between text lines
    int _lineHeight = 1;  // Fixed line height in character units
    std::map<int, int> _yPositionMap;  // Maps pixel Y to character Y positions

    // Convert pixel coordinates to character-based coordinates
    int pixelToCharX(int x) const;
    int pixelToCharY(int y);  // Non-const because it might modify the map
    void calculateRatio();

 public:
    NCursesModule() : _name("NCurses") {}
    ~NCursesModule() override;
    void init(float width, float height) override;
    void stop() override;
    void clearScreen() override;
    void refreshScreen() override;
    void drawEntity(int x, int y, char symbol) override;
    void drawTexture(int x, int y, const std::string &textureId) override;
    void drawText(const std::string &text, int x,
        int y, Arcade::Color color) override;
    void pollEvents() override;
    bool isOpen() const override;
    const std::string& getName() const override;
    int getWidth() const override;
    int getHeight() const override;
    bool isKeyPressed(int keyCode) override;
    bool isMouseButtonPressed(int button) const override;
    std::pair<size_t, size_t> getMousePosition() const override;
};

#endif  // SRC_GRAPHICS_NCURSES_NCURSES_HPP_
