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
        void drawEntity(int x, int y, char symbol);
        void drawTexture(int x, int y, const std::string &textureId);
        void drawText(const std::string &text, int x,
            int y, Arcade::Color color);

     public:
        NCursesModule() : _name("NCurses") {}
        ~NCursesModule() override;
        void init(float width, float height) override;
        void stop() override;
        void clearScreen() override;
        void refreshScreen() override;
        void pollEvents() override;
        bool isOpen() const override;
        void drawDrawable(const Arcade::DrawableComponent& drawable) override;
        const std::string& getName() const override;
        int getWidth() const override;
        int getHeight() const override;
        bool isKeyPressed(int keyCode) override;
        bool isMouseButtonPressed(int button) const override;
        std::pair<size_t, size_t> getMousePosition() const override;
    };

#endif  // SRC_GRAPHICS_NCURSES_NCURSES_HPP_
