// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGL
*/

#ifndef OPENGL_HPP_
    #define OPENGL_HPP_
    #include <memory>
    #include <string>
    #include "../../Shared/Interface/Display/IDisplayModule.hpp"
    #include "OpenGLUtils/OpenGLWindow.hpp"
    #include "OpenGLUtils/OpenGLRenderer.hpp"
    #include "OpenGLUtils/OpenGLTexture.hpp"
    #include "OpenGLUtils/OpenGLText.hpp"

    class OpenGLModule : public Arcade::IDisplayModule {
     private:
        std::string _name;
        GL::OpenGLWindow _window;
        GL::OpenGLRenderer _renderer;
        GL::OpenGLTexture _texture;
        GL::OpenGLText _text;
        int _windowWidth = 0;
        int _windowHeight = 0;
        bool _running = true;

     public:
        OpenGLModule() : _name("OpenGL") {}
        ~OpenGLModule() override;
        void init(float width = 800.f, float height = 600.f) override;
        void stop() override;
        void clearScreen() override;
        void refreshScreen() override;
        void drawEntity(int x, int y, char symbol) override;
        void drawTexture(int x, int y, const std::string &textureId) override;
        void drawText(int x, int y, const std::string &text) override;
        void pollEvents() override;
        bool isOpen() const override;
        const std::string& getName() const override;
    };

#endif /* !OPENGL_HPP_ */
