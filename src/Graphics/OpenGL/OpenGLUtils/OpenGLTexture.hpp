// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLTexture
*/

#ifndef SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLTEXTURE_HPP_
    #define SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLTEXTURE_HPP_
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <memory>
    #include <string>
    #include <unordered_map>

namespace GL {

class OpenGLTexture {
 private:
    struct TextureData {
        GLuint id;
        int width;
        int height;
    };

    std::unordered_map<std::string, TextureData> _textures;
    GLuint _VAO;
    GLuint _VBO;
    GLuint _shaderProgram;
    bool _isInitialized;

 public:
    OpenGLTexture();
    ~OpenGLTexture();

    void init();
    void draw(int x, int y, const std::string &textureId);
    bool loadTexture(const std::string &filename, const std::string &id);
};

}  // namespace GL

#endif  // SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLTEXTURE_HPP_
