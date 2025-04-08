// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGL
*/

#ifndef SRC_GRAPHICS_OPENGL_OPENGL_HPP_
    #define SRC_GRAPHICS_OPENGL_OPENGL_HPP_
    #include <memory>
    #include <utility>
    #include <string>
    #include "Interface/Display/IDisplayModule.hpp"
    #include "OpenGLUtils/OpenGLWindow.hpp"
    #include "OpenGLUtils/OpenGLEvent.hpp"
    #include "OpenGLUtils/OpenGLText.hpp"
    #include "OpenGLUtils/OpenGLTexture.hpp"
    #include "Models/ColorType.hpp"

class OpenGLModule : public Arcade::IDisplayModule {
 private:
    std::string _name;
    GL::OpenGLWindow _window;
    GL::OpenGLEvent _event;
    GL::OpenGLText _text;
    GL::OpenGLTexture _texture;
    int _windowWidth;
    int _windowHeight;
    bool _running;

 public:
    OpenGLModule();
    ~OpenGLModule() override;
    void init(float width = 800.f, float height = 600.f) override;
    void stop() override;
    void clearScreen() override;
    void refreshScreen() override;
    void drawEntity(int x, int y, char symbol) override;
    void drawTexture(int x, int y, const std::string &textureId) override;
    void drawText(const std::string &text, int x, int y,
        Arcade::Color color) override;
    void pollEvents() override;
    bool isOpen() const override;
    const std::string& getName() const override;
    int getWidth() const override;
    int getHeight() const override;
    bool isKeyPressed(int keyCode) override;
    bool isMouseButtonPressed(int button) const override;
    std::pair<size_t, size_t> getMousePosition() const override;
};

#endif  // SRC_GRAPHICS_OPENGL_OPENGL_HPP_
