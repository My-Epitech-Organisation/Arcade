// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLText
*/

#ifndef SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLTEXT_HPP_
    #define SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLTEXT_HPP_
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <ft2build.h>
    #include FT_FREETYPE_H
    #include <map>
    #include <string>
    #include <memory>

namespace GL {

struct Character {
    unsigned int TextureID;
    int Width;
    int Height;
    int Bearing[2];
    unsigned int Advance;
};

class OpenGLText {
 public:
    OpenGLText() : _shader(0) {}
    ~OpenGLText() = default;
    void init();
    void drawText(int x, int y, const std::string& text, int windowWidth, int windowHeight);

 private:
    std::map<char, Character> _characters;
    unsigned int _VAO, _VBO;
    unsigned int _shader;
    unsigned int compileTextShader();
    void loadChar();
};

}  // namespace GL

#endif  // SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLTEXT_HPP_
