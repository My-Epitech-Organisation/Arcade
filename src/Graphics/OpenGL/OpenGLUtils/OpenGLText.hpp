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
    #include <string>
    #include <map>
    #include <glm/glm.hpp>
    #include "Models/ColorType.hpp"

namespace GL {
class OpenGLText {
 public:
    struct Character {
        GLuint textureID;
        int width;
        int height;
        int bearingX;
        int bearingY;
        unsigned int advance;
    };

    OpenGLText();
    ~OpenGLText();
    void init();
    void draw(const std::string &text, int x, int y, Arcade::Color color);
 private:
    std::map<char, Character> _characters;
    GLuint _VAO;
    GLuint _VBO;
    GLuint _shaderProgram;
    bool _isInitialized;
    int _fontHeight;
    int _fontAscender;
};

}  // namespace GL

#endif  // SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLTEXT_HPP_
